#include <iostream>

#include <mlpack/core.hpp>
#include <mlpack/core/data/load_arff.hpp>
#include <mlpack/core/data/map_policies/missing_policy.hpp>

#include <mlpack/core/optimizers/sgd/sgd.hpp>
#include <mlpack/core/optimizers/adam/adam_update.hpp>

#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/rnn.hpp>
#include <mlpack/methods/ann/layer/lstm.hpp>
#include <mlpack/prereqs.hpp>

using namespace mlpack;
using namespace mlpack::ann;
using namespace mlpack::optimization;
using namespace mlpack::data;

using namespace arma;
using namespace std;

// Prepare input of sequence of notes for LSTM
// The LSTM inputs and outputs armadillo cubes, that is a three dimensional tensor
// The rows are the features of our data (we only have 1 feature = note value)
// The columns contain each time step within the music
// The slices contain each sequence of music taken
arma::cube getTrainX(const mat& tempDataset, const unsigned int& sequence_length)
{
    const unsigned int num_notes = tempDataset.n_rows;	
    const unsigned int num_sequences = num_notes - sequence_length;
    cube trainX = cube(1, num_sequences, sequence_length);	
    for (unsigned int i = 0; i < num_sequences; i++)
    {
	for (unsigned int j = 0; j < sequence_length; j++)
	{
		trainX(0,i, j) = tempDataset(i+j,0);
	}
    }
    return trainX;
 }

// Vector of "actual" notes from the training set used to calculate accuracy of training
arma::mat getReal(const mat& tempDataset, const int& sequence_length)
{
    mat real = mat(1,tempDataset.n_rows - sequence_length);
    for (unsigned int i = sequence_length; i < tempDataset.n_rows; i++)
    {
	 real(0,i-sequence_length) = tempDataset(i,0);
    }
    return real;
}	

// Get the "labels" used for training, that is the note that follows each sequence of notes in trainX
arma::cube getTrainY(const mat& tempDataset, const int& sequence_length)
{
    cube trainY = cube(1, tempDataset.n_rows - sequence_length, sequence_length); 
    for (unsigned int i = sequence_length; i < tempDataset.n_rows; i++)
    {
	int note = tempDataset.at(i,0);
	trainY.tube(0,i-sequence_length).fill(note+1);
    }
    return trainY;
}

// The output of the model is a vector that gives the probability of each note at the index representing it
// To get the notes, we take the index with the max probability
arma::mat getNotes(const mat& proba)
{
    unsigned int num_notes = proba.n_cols;
    mat notes = mat(1, num_notes);
    for (unsigned int i = 0; i < num_notes; i++)
    {
        notes(0,i) = arma::as_scalar(arma::find(
          arma::max(proba.col(i)) == proba.col(i), 1)) + 1;
    }
    return notes;
}				   


// Returns the accuracy (percentage of correct notes)
// Note that we expect the accuracy to be low even on a well trained model, 
// we don't want the model to reproduce exactly the music given
double accuracy(arma::mat& predicted, const arma::mat& real)
{
    // Calculating how many predicted notes coincide with actual notes.
    size_t success = 0;
    for (size_t j = 0; j < predicted.n_cols; j++) {
        if (predicted(0,j) == std::round(real(0,j))) {
            ++success;
        }
    }
    
    // Calculating percentage of correctly predicted notes.
    return (double) success / (double)predicted.n_cols * 100.0;
}

void trainModel(RNN<>& model,
                const cube& trainX, const cube& trainY, const mat& real)
{
    // The solution is done in several approaches (CYCLES), so each approach
    // uses previous results as starting point and have a different optimizer
    // options (here the step size is different).
        			      
     // Number of iteration per cycle.
    constexpr int ITERATIONS_PER_CYCLE = 10;

    // Number of cycles.
    constexpr int CYCLES = 500;

    // Step size of an optimizer.
    constexpr double STEP_SIZE = 5e-6;

    // Number of data points in each iteration of SGD
    const int BATCH_SIZE = trainX.n_rows / ITERATIONS_PER_CYCLE;

    // Setting parameters Stochastic Gradient Descent (SGD) optimizer.
    SGD<AdamUpdate> optimizer(
                              // Step size of the optimizer.
                              STEP_SIZE,
                              // Batch size. Number of data points that are used in each iteration.
                              BATCH_SIZE,
                              // Max number of iterations
                              ITERATIONS_PER_CYCLE,
                              // Tolerance, used as a stopping condition. This small number
                              // means we never stop by this condition and continue to optimize
                              // up to reaching maximum of iterations.
                              1e-8,
    			      false,
    			      // Adam update policy.
    			      AdamUpdate(1e-8, 0.9, 0.999));
	
	
    // Cycles for monitoring the process of a solution.
    for (int i = 0; i <= CYCLES; i++)
    {
        // Train neural network. If this is the first iteration, weights are
        // random, using current values as starting point otherwise.
       
       	model.Train(trainX, trainY, optimizer);
       
        // Don't reset optimizer's parameters between cycles.
        optimizer.ResetPolicy() = false;
        
        cube predOut;
        // Getting predictions on training data points.
        model.Predict(trainX, predOut);

        // Calculating accuracy on training data points.
        mat pred = getNotes(predOut.slice(predOut.n_slices - 1));
	cout << "Predicted notes" << pred << endl;
	cout <<	 "Actual notes" << real << endl;    
        double trainAccuracy = accuracy(pred, real);       

        cout << i << " - accuracy = "<< trainAccuracy << "%," << endl;
	
	// Save the model every 10 cycles
	if (i % 10 == 0)
	{
		cout << "Checkpoint at cycle" << i << endl;
   		data::Save("model.xml", "model", model, false);
	}

        
    }
}

int main () {
    
    cout << "Reading data ..." << endl; 
	
    mat tempDataset;

    const int rho = 5;
    
    // load a matrice containing translated music file for training
    data::Load("../utils/training.csv", tempDataset, true); 
    
    // the total number of different notes we have
    const int size_notes = max(tempDataset.row(0)) + 1;
	
    cube trainX = getTrainX(tempDataset, rho);
    cube trainY = getTrainY(tempDataset, rho);
    mat real = getReal(tempDataset, rho);	
    
    RNN<> model(rho);
    model.Add<Linear <> > (trainX.n_rows, rho);
    model.Add<LSTM <> > (rho,512);
    model.Add<Linear <> > (512, 512);
    model.Add<LSTM <> > (512, 512);
    model.Add<Linear <> > (512, 256);
    model.Add<Dropout <> > (0.3); // prevent overfitting
    model.Add<Linear <> > (256, size_notes);
    model.Add<LogSoftMax<> > ();
    
    // Load preexisting weights to continue training, if it exists
    data::Load("model.xml", "model", model);

    cout << "Training ..." << endl;
    trainModel(model, trainX, trainY, real);

    cout << "Saving model ..." << endl;
    data::Save("model.xml", "model", model, false);
    cout << "Saved!" << endl;
    return 0;
}

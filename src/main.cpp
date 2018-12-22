/**
 * Code adapted from https://github.com/mlpack/models/blob/master/Kaggle/DigitRecognizer/src/DigitRecognizer.cpp
 */

#include <iostream>

#include <mlpack/core.hpp>
#include <mlpack/core/data/split_data.hpp>

#include <mlpack/core/optimizers/sgd/sgd.hpp>
#include <mlpack/core/optimizers/adam/adam_update.hpp>

#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/rnn.hpp>
#include <mlpack/methods/ann/layer/lstm.hpp>
#include <mlpack/methods/ann/loss_functions/kl_divergence.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/ann/loss_functions/cross_entropy_error.hpp>
#include <mlpack/prereqs.hpp>

using namespace mlpack;
using namespace mlpack::ann;
using namespace mlpack::optimization;

using namespace arma;
using namespace std;

// Prepare input of sequence of notes for LSTM
arma::cube getTrainX(const mat& tempDataset, const unsigned int& sequence_length)
{
    const unsigned int num_notes = tempDataset.n_rows;	
    const unsigned int num_sequences = (num_notes / sequence_length) + 1;
    cube trainX = cube(1, num_sequences, sequence_length);	
    for (unsigned int i = 0; i < num_sequences; i++)
    {
	for (unsigned int j = 0; j < sequence_length; j++)
	{
		trainX(0,i,j) = tempDataset(i+j,0);
	}
    }
    return trainX;
 }

arma::cube getTrainY(const mat& tempDataset, const int& sequence_length)
{
    cube trainY = cube(1,tempDataset.n_rows - sequence_length, sequence_length);
    for (unsigned int i = sequence_length; i < tempDataset.n_rows; i++)
    {
	 trainY.tube(0,i-sequence_length).fill(tempDataset(i,0));
    }
    return trainY;
}	

// Generate array with 1 in the indice of the note present at a time step
arma::mat getProba(const mat& tempDataset, const int& size_notes, const int& sequence_length)
{
    mat proba = mat(size_notes, tempDataset.n_rows - sequence_length, fill::zeros);
    for (unsigned int i = sequence_length; i < tempDataset.n_rows; i++)
    {
	int note = tempDataset.at(i,0);
	proba.at(note,i-sequence_length) = 1;
    }
    return proba;
}

 /**
 * Returns the accuracy (percentage of correct answers).
 * @param predLabels predicted labels of data points.
 * @param real actual notes (they are double because we usually read them from
 * CSV file that contain many other double values).
 * @return percentage of correct answers.
 */
/**
double accuracy(arma::mat& predLabels, const arma::mat& real)
{
    // Calculating how many predicted notes coincide with actual notes.
    mat pred = index_max(predLabels, 0);
    size_t success = 0;
    for (size_t j = 0; j < real.n_cols; j++) {
        if (pred(0,j) == std::round(real(0,j))) {
            ++success;
        }
    }
    
    // Calculating percentage of correctly predicted notes.
    return (double) success / (double)real.n_cols * 100.0;
}
*/

void trainModel(RNN<MeanSquaredError<>>& model,
                const cube& trainX, const cube& trainY)
{
    // The solution is done in several approaches (CYCLES), so each approach
    // uses previous results as starting point and have a different optimizer
    // options (here the step size is different).
    
    // Number of iteration per cycle.
    constexpr int ITERATIONS_PER_CYCLE = 10000;
    
    // Number of cycles.
    constexpr int CYCLES = 50;
    
    // Step size of an optimizer.
    constexpr double STEP_SIZE = 5e-20;
    
    // Number of data points in each iteration of SGD
    constexpr int BATCH_SIZE = 5;
    
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
        // double trainAccuracy = accuracy(predOut, trainY);       

        // cout << i << " - accuracy: train = "<< trainAccuracy << "%," << endl;
        
    }
}

/**
 * Run the neural network model and predict the class for a
 * set of testing example
 */
void predictClass(RNN<MeanSquaredError<>>& model,
                  const std::string datasetName, const int rho)
{
    
    mat tempDataset;
    data::Load(datasetName, tempDataset, true);
    
    cube test = cube(1,tempDataset.n_cols,rho);
    for (unsigned int i = 0; i < tempDataset.n_cols; i++)
    {
	test(0,i,0) = tempDataset.at(0,i);
    }

    
    cube testPredOut;
    // Getting predictions on test data points .
    model.Predict(test, testPredOut);
    // Generating labels for the test dataset.
    //Row<size_t> testPred = getLabels(testPredOut);
    cout << "Saving predicted labels to \"results.csv\" ..." << endl;
    mat testPred(1,testPredOut.n_cols);
    for (unsigned int i = 0; i < testPredOut.n_cols; i++)
    {
	testPred(0,i) = testPredOut.at(0,i,testPredOut.n_slices-1);
    }
    // Saving results into Kaggle compatibe CSV file.
    data::Save("results.csv", testPred); // testPred or test??
    cout << "Results were saved to \"results.csv\"" << endl;
}

int main () {
    
    cout << "Reading data ..." << endl; 
	
    mat tempDataset;
    const int rho = 8;

    data::Load("../utils/training.csv", tempDataset, true); // read data from this csv file, creates arma matrix with loaded data in tempDataset
    
   
    const int size_notes = max(tempDataset.row(0));
    const int sequence_length = 3;
	
    cube trainX = getTrainX(tempDataset, sequence_length);
    cube trainY = getTrainY(tempDataset, sequence_length);
    cout << trainX << trainY << endl;
    //mat trainY = getProba(trainYP, sequence_length);	

    // According to NegativeLogLikelihood output layer of NN, labels should
    // specify class of a data point and be in the interval from 1 to
    // number of classes (in this case from 1 to 10).
    
    // Specifying the NN model. NegativeLogLikelihood is the output layer that
    // is used for classification problem. RandomInitialization means that
    // initial weights in neurons are generated randomly in the interval
    // from -1 to 1.
	
    RNN<MeanSquaredError<> > model(rho);
    model.Add<Linear <> > (trainX.n_rows, rho);
    model.Add<LSTM <> > (rho,512);
    model.Add<Linear <> > (512, 256);
    model.Add<Dropout <> > (0.3);
    model.Add<Linear <> > (256, size_notes);
    //model.Add<SigmoidLayer <> >();
    model.Add<LogSoftMax<> > ();
    	
    cout << "Training ..." << endl;
    trainModel(model, trainX, trainY);
    
    cout << "Predicting ..." << endl;
    std::string datasetName = "../utils/test.csv";
    predictClass(model, datasetName,rho);
    cout << "Finished" << endl;
    
    return 0;
}

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

/**
 * Returns labels bases on predicted probability (or log of probability)
 * of classes.
 * @param predOut matrix contains probabilities (or log of probability) of
 * classes. Each row corresponds to a certain class, each column corresponds
 * to a data point.
 * @return a row vector of data point's classes. The classes starts from 1 to
 * the number of rows in input matrix.
 */
arma::Row<size_t> getLabels(const arma::cube& predOut)
{
    arma::Row<size_t> pred(predOut.n_cols);
    
    // Class of a j-th data point is chosen to be the one with maximum value
    // in j-th column plus 1 (since column's elements are numbered from 0).
    for (size_t j = 0; j < predOut.n_cols; ++j)
    {
        pred(j) = arma::as_scalar(arma::find(
                                             arma::max(predOut.col(j)) == predOut.col(j), 1)) + 1;
    }
    
    return pred;
}

/**
 * Returns the accuracy (percentage of correct answers).
 * @param predLabels predicted labels of data points.
 * @param real actual notes (they are double because we usually read them from
 * CSV file that contain many other double values).
 * @return percentage of correct answers.
 */
double accuracy(arma::cube predLabels, const arma::cube& real)
{
    cout << "predicted" << predLabels << endl;
    cout << "real" << real << endl;
    // Calculating how many predicted notes coincide with actual notes.
    size_t success = 0;
    for (size_t j = 0; j < real.n_cols; j++) {
        if (predLabels(0,j,predLabels.n_slices-1) == std::round(real.at(0,j,0))) {
            ++success;
        }
    }
    
    // Calculating percentage of correctly predicted notes.
    return (double) success / (double)real.n_cols * 100.0;
}


/**
 * Saves prediction into specifically formated CSV file.
 *
 * @param filename the name of a file.
 * @param header the header in a CSV file.
 *
 * @param predLabels predicted notes. Classes of data points
 * are expected to start from 1. At the same time classes of data points in
 * the file are going to start from 0
<<<<<<< HEAD
 
=======

>>>>>>> e75873b8ad26334b9f8afd227b066e88f019299b
void save(const std::string filename,
        const arma::Row<size_t>& predLabels,
          const arma::cube& inputData,
          const arma::cube& inputLabel)
{
    std::ofstream out(filename);
    out << header << std::endl;
    for (size_t j = 0; j < predLabels.n_cols; ++j)
    {
        out << std::round(predLabels(j)) - 1 << "," <<
        arma::as_scalar(inputData.slice(j).row(0)) <<  "," <<
        arma::as_scalar(inputData.slice(j).row(1)) << "," <<
        arma::as_scalar(inputLabel.slice(j).row(0) - 1);
        
        // to avoid an empty line in the end of the file
        if (j < predLabels.n_cols - 1)
        {
            out << std::endl;
        }
    }
    out.close();
}
*/

void buildLSTMModel(RNN<>& model,
                         const int sizeInputLayer,
                         const int totalClasses)
{
    // The number of neurons in the hidden layers.
    constexpr int H1 = 8;
    constexpr int H2 = 30;
    
    model.Add<Linear<> >(sizeInputLayer, H1);
    // 4 LSTM Layers
    model.Add<LSTM<> >(H1,H2);
    model.Add<LSTM<> >(H2,H2);
    model.Add<LSTM<> >(H2,H2);
    model.Add<LSTM<> >(H2,H2);

    // sigmoid layer.
    model.Add<SigmoidLayer<> >();

    // Intermediate layer.
    model.Add<Linear<> >(H2, totalClasses);
    // LogSoftMax layer is used together with NegativeLogLikelihood for mapping
    // output values to log of probabilities of being a specific class.
    //model.Add<LogSoftMax<> >();
    cout << "Building done" << endl;
}


void trainModel(RNN<MeanSquaredError<>>& model,
                const cube& trainX, const cube& trainY, const cube& validX,
                const cube& validY)
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
    constexpr int BATCH_SIZE = 3;
    
    // Setting parameters Stochastic Gradient Descent (SGD) optimizer.
    StandardSGD optimizer(
                              // Step size of the optimizer.
                              STEP_SIZE,
                              // Batch size. Number of data points that are used in each iteration.
                              BATCH_SIZE,
                              // Max number of iterations
                              ITERATIONS_PER_CYCLE,
                              // Tolerance, used as a stopping condition. This small number
                              // means we never stop by this condition and continue to optimize
                              // up to reaching maximum of iterations.
                              1e-8);
    			      
   
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
	// Row<size_t> predLabels = getLabels(predOut);
        double trainAccuracy = accuracy(predOut, trainY);
	// Getting predictions on validating data points.
        model.Predict(validX, predOut);
        // Calculating accuracy on validating data points.
        double validAccuracy = accuracy(predOut,validY);        

        cout << i << " - accuracy: train = "<< trainAccuracy << "%," <<
        " valid = "<< validAccuracy << "%" <<  endl;
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
    // Dataset is randomly split into validation
    // and training parts with following ratio.
    // constexpr double RATIO = 0.1;
    
    cout << "Reading data ..." << endl; // used in training and validation
       
    // Labeled dataset that contains data for training is loaded from CSV file,
    // rows represent features, columns represent data points.
    mat tempDataset;
    const int rho = 8;

    data::Load("../utils/training.csv", tempDataset, true); // read data from this csv file, creates arma matrix with loaded data in tempDataset
    //cout << "training data" << tempDataset << tempDataset.size() << endl;    
    // Initialize loaded data where row = dimension = 1, column = note values, slice = timestep
	
    // Splitting the dataset on training and validation parts.
    const int ind = (int) 9*tempDataset.n_rows / 10;
    cube trainX = cube(1, ind,rho);
    cube trainY = cube(1,ind,rho);

    cube validX = cube(1, tempDataset.n_rows - ind,rho);
    cube validY = cube(1, tempDataset.n_rows - ind,rho);

    for (int i = 0; i < ind; i++)
    {
	trainX.at(0,i,0) = tempDataset.at(i,0);
	trainY.at(0,i,0) = tempDataset.at(i,1);
    }

    for (unsigned int i = ind; i < tempDataset.n_rows; i++)
    {
	validX.at(0,i-ind,0) = tempDataset.at(i,0);
	validY.at(0,i-ind,0) = tempDataset.at(i,1);
    }

    cout << trainX << trainY << validX << validY << endl;

    // According to NegativeLogLikelihood output layer of NN, labels should
    // specify class of a data point and be in the interval from 1 to
    // number of classes (in this case from 1 to 10).
    
    // Specifying the NN model. NegativeLogLikelihood is the output layer that
    // is used for classification problem. RandomInitialization means that
    // initial weights in neurons are generated randomly in the interval
    // from -1 to 1.
	
    RNN<MeanSquaredError<> > model(rho);
    model.Add<Linear <> > (trainX.n_rows, rho);
    model.Add<LSTM <> > (rho,4);
    model.Add<LSTM <> > (4,4);
    model.Add<LSTM <> > (4,4);
    model.Add<LSTM <> > (4,4);
    model.Add<SigmoidLayer <> >();
    model.Add<Linear <> > (4, 1);
    //model.Add<LogSoftMax<> > ();
    	
    //RNN<CrossEntropyError<>> model(rho);

    //buildLSTMModel(model, trainX.n_rows, 1);
    cout << "Training ..." << endl;
    trainModel(model, trainX, trainY, validX, validY);
    
    cout << "Predicting ..." << endl;
    std::string datasetName = "../utils/test.csv";
    predictClass(model, datasetName,rho);
    cout << "Finished" << endl;
    
    return 0;
}

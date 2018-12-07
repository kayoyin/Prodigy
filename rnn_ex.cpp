/**
 * Code adapted from https://github.com/mlpack/models/blob/master/Kaggle/DigitRecognizer/src/DigitRecognizer.cpp
 */

#include <iostream>

#include <mlpack/core.hpp>
#include <mlpack/core/data/split_data.hpp>

#include <mlpack/core/optimizers/sgd/sgd.hpp> // standard optimization library
#include <mlpack/core/optimizers/adam/adam_update.hpp>

#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/rnn.hpp> // replace with rnn
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
arma::Row<size_t> getLabels(const arma::mat& predOut)
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
 * @param realY real labels (they are double because we usually read them from
 * CSV file that contain many other double values).
 * @return percentage of correct answers.
 */
double accuracy(arma::Row<size_t> predLabels, const arma::mat& realY)
{
    // Calculating how many predicted classes are coincide with real labels.
    size_t success = 0;
    for (size_t j = 0; j < realY.n_cols; j++) {
        if (predLabels(j) == std::round(realY(j))) {
            ++success;
        }
    }
    
    // Calculating percentage of correctly classified data points.
    return (double) success / (double)realY.n_cols * 100.0;
}


/**
 * Saves prediction into specifically formated CSV file.
 *
 * @param filename the name of a file.
 * @param header the header in a CSV file.
 *
 * @param predLabels predicted labels of data points. Classes of data points
 * are expected to start from 1. At the same time classes of data points in
 * the file are going to start from 0 (as Kaggle usually expects)
 */
void save(const std::string filename,
          const std::string header,
          const arma::Row<size_t>& predLabels,
          const arma::mat& inputData,
          const arma::mat& inputLabel)
{
    std::ofstream out(filename);
    out << header << std::endl;
    for (size_t j = 0; j < predLabels.n_cols; ++j)
    {
        out << std::round(predLabels(j)) - 1 << "," <<
        arma::as_scalar(inputData.col(j).row(0)) <<  "," <<
        arma::as_scalar(inputData.col(j).row(1)) << "," <<
        arma::as_scalar(inputLabel.col(j).row(0) - 1);
        
        // to avoid an empty line in the end of the file
        if (j < predLabels.n_cols - 1)
        {
            out << std::endl;
        }
    }
    out.close();
}


void buildModelOneLayer(RNN<NegativeLogLikelihood<>, RandomInitialization>& model,
                        const int sizeInputLayer,
                        const int totalClasses)
{
    // The number of neurons in the first layer.
    constexpr int H1 = 100;
    
    Add<> add(H1);
    Linear<> >(sizeInputLayer, H1); // H1: number of neurons in layer
    SigmoidLayer<> sigmoidLayer;
    Linear<> linear(H1, H1);
    Recurrent<>* recurrent = new Recurrent<>(
                                             add, lookup, linear, sigmoidLayer, rho);
    
    
    model.Add<IdentityLayer<> >();
    // This is intermediate layer that is needed for connection between input
    // data and sigmoid layer. Parameters specify the number of input features
    // and number of neurons in the next layer.
    
    // The first recurrent layer.
    model.Add(recurrent);
    
    model.Add<Linear<> >(H1, totalClasses);
    // LogSoftMax layer is used together with NegativeLogLikelihood for mapping
    // output values to log of probabilities of being a specific class.
    model.Add<LogSoftMax<> >(); // another activation function for final layer
}

/**
 *
 */
void buildModelTwoLayers(RNN<NegativeLogLikelihood<>, RandomInitialization>& model,
                         const int sizeInputLayer,
                         const int totalClasses)
{
    // The number of neurons in the first layer.
    constexpr int H1 = 100;
    // The number of neurons in the second layer.
    constexpr int H2 = 100;
    
    Add<> add(2);
    Linear<> lookup(1, 2);
    SigmoidLayer<> sigmoidLayer;
    Linear<> linear(2, 2);
    Recurrent<>* recurrent = new Recurrent<>(
                                             add, lookup, linear, sigmoidLayer, rho);
    
    // This is intermediate layer that is needed for connection between input
    // data and sigmoid layer. Parameters specify the number of input features
    // and number of neurons in the next layer.
    model.Add<Linear<> >(sizeInputLayer, H1);
    // The first sigmoid layer.
    model.Add<SigmoidLayer<> >();
    // Intermediate layer between sigmoid layers.
    model.Add<Linear<> >(H1, H2);
    // The second sigmoid layer.
    model.Add<SigmoidLayer<> >();
    // Dropout layer for regularization. First parameter is the probability of
    // setting a specific value to 0.
    // model.Add<Dropout<> >(0.3, true);
    // Intermediate layer.
    model.Add<Linear<> >(H2, totalClasses);
    // LogSoftMax layer is used together with NegativeLogLikelihood for mapping
    // output values to log of probabilities of being a specific class.
    model.Add<LogSoftMax<> >();
}


/**
 *
 */
void trainModel(RNN<NegativeLogLikelihood<>, RandomInitialization>& model,
                const mat& trainX, const mat& trainY,
                const mat& validX, const mat& validY)
{
    // The solution is done in several approaches (CYCLES), so each approach
    // uses previous results as starting point and have a different optimizer
    // options (here the step size is different).
    
    // Number of iteration per cycle.
    constexpr int ITERATIONS_PER_CYCLE = 10000;
    
    // Number of cycles.
    constexpr int CYCLES = 20;
    
    // Step size of an optimizer.
    constexpr double STEP_SIZE = 5e-4;
    
    // Number of data points in each iteration of SGD
    constexpr int BATCH_SIZE = 50;
    
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
                              // Shuffle. If optimizer should take random data points from the dataset at
                              // each iteration.
                              true,
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
        
        mat predOut;
        // Getting predictions on training data points.
        model.Predict(trainX, predOut);
        // Calculating accuracy on training data points.
        Row<size_t> predLabels = getLabels(predOut);
        double trainAccuracy = accuracy(predLabels, trainY);
        // Getting predictions on validating data points.
        model.Predict(validX, predOut);
        // Calculating accuracy on validating data points.
        predLabels = getLabels(predOut);
        double validAccuracy = accuracy(predLabels, validY);
        
        cout << i << " - accuracy: train = "<< trainAccuracy << "%," <<
        " valid = "<< validAccuracy << "%" <<  endl;
    }
}

/**
 * Run the neural network model and predict the class for a
 * set of testing example
 */
void predictClass(RNN<NegativeLogLikelihood<>, RandomInitialization>& model,
                  const std::string datasetName)
{
    
    mat tempDataset;
    data::Load(datasetName, tempDataset, true);
    
    // Get rid of the header
    mat noHeaders = tempDataset.submat(0, 1,
                                       tempDataset.n_rows - 1, tempDataset.n_cols - 1);
    
    const mat testX = noHeaders.submat(1, 0, noHeaders.n_rows - 1, noHeaders.n_cols - 1);
    const mat testY = noHeaders.row(0) + 1;
    
    mat testPredOut;
    // Getting predictions on test data points .
    model.Predict(testX, testPredOut);
    // Generating labels for the test dataset.
    Row<size_t> testPred = getLabels(testPredOut);
    cout << "Saving predicted labels to \"results.csv\" ..." << endl;
    
    // Saving results into Kaggle compatibe CSV file.
    save("results.csv", "label,x,y", testPred, testX, testY);
    cout << "Results were saved to \"results.csv\"" << endl;
}

int main () {
    // Dataset is randomly split into validation
    // and training parts with following ratio.
    constexpr double RATIO = 0.1;
    
    cout << "Reading data ..." << endl; // used in training and validation
    
    // Labeled dataset that contains data for training is loaded from CSV file,
    // rows represent features, columns represent data points.
    mat tempDataset;
    
    data::Load("../utils/training.csv", tempDataset, true); // read data from this csv file, creates arma matrix with loaded data in tempDataset
    // warning: armadillo transposes matrix?
    // label 0 1
    // x     3 5
    // y     2 6
    
    // get rid of the header (in Armadillo representation it's the first column) (delete "label x y")
    mat dataset = tempDataset.submat(0, 1,
                                     tempDataset.n_rows - 1, tempDataset.n_cols - 1);
    
    // Splitting the dataset on training and validation parts.
    mat train, valid;
    data::Split(dataset, train, valid, RATIO);
    
    // Getting training and validating dataset with features only.
    const mat trainX = train.submat(1, 0, train.n_rows - 1, train.n_cols - 1);
    const mat validX = valid.submat(1, 0, valid.n_rows - 1, valid.n_cols - 1);
    
    // According to NegativeLogLikelihood output layer of NN, labels should
    // specify class of a data point and be in the interval from 1 to
    // number of classes (in this case from 1 to 10).
    
    // Creating labels for training and validating dataset.
    const mat trainY = train.row(0) + 1;
    const mat validY = valid.row(0) + 1;
    
    // Specifying the NN model. NegativeLogLikelihood is the output layer that
    // is used for classification problem. RandomInitialization means that
    // initial weights in neurons are generated randomly in the interval
    // from -1 to 1.
    FFN<NegativeLogLikelihood<>, RandomInitialization> model;
    buildModelOneLayer(model, trainX.n_rows, 2);
    //buildModelTwoLayers(model, trainX.n_rows, 2);
    
    cout << "Training ..." << endl;
    trainModel(model, trainX, trainY, validX, validY);
    
    cout << "Predicting ..." << endl;
    std::string datasetName = "../utils/test.csv";
    predictClass(model, datasetName);
    cout << "Finished" << endl;
    
    return 0;
}

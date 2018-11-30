//
//  rnn.cpp
//  ProdigyLSTM
//
//  Created by Kayo Yin on 30/11/2018.
//  Copyright © 2018 Kayo Yin. All rights reserved.
//

#include <mlpack/core.hpp>

#include <mlpack/core/optimizers/sgd/sgd.hpp>
#include <mlpack/core/optimizers/rmsprop/rmsprop.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/ann/rnn.hpp>
#include <mlpack/core/data/binarize.hpp>
#include <mlpack/core/math/random.hpp>
#include <mlpack/layer/LSTM.hpp>

#include <boost/test/unit_test.hpp>
#include "test_tools.hpp"
#include "serialization.hpp"
#include "custom_layer.hpp"

#include "RNNConfig.h.in"
#ifdef USE_MYTEST
#include "TestFunctions.h"
#endif

using namespace mlpack;
using namespace mlpack::ann;
using namespace mlpack::optimization;
using namespace mlpack::math;

//Load training set music
arma::mat dataset; // some array of notes with labels and stuff
data::Load("music.csv", dataset, true);

// Split the data from the validation set, actual numbers TBD
arma::mat validation = dataset.submat(dataset.n_rows - 3, 0,
                                           dataset.n_rows - 1, dataset.n_cols - 1);

/**
 * Train the vanilla network on a larger dataset.
 */
BOOST_AUTO_TEST_CASE(SequenceClassificationTest)
{
    // Checking if getting stuck in local minima or not
    
    size_t successes = 0;
    const size_t rho = 10; //max number of backpropagation in RNN
    
    for (size_t trial = 0; trial < 6; ++trial)
    {
        arma::mat input; //input that is an armadillo matrix
    }
    
    /**
     * Construct a network with 1 input unit, 4 hidden units and 10 output
     * units. The hidden layer is connected to itself. The network structure
     * looks like:
     *
     *  Input         Hidden        Output
     * Layer(1)      Layer(4)      Layer(10)
     * +-----+       +-----+       +-----+
     * |     |       |     |       |     |
     * |     +------>|     +------>|     |
     * |     |    ..>|     |       |     |
     * +-----+    .  +--+--+       +-----+
     *            .     .
     *            .     .
     *            .......
     */
    Add<> add(4);
    Linear<> lookup(1,4);
    SigmoidLayer<> sigmoidLayer;
    Linear<> linear(4,4);
    Recurrent<>* recurrent = new Recurrent<>(
        add, lookup, linear, sigmoidLayer, rho); // recurrent = the 4 hidden layers
    
    RNN<> model(rho);
    model.Add<IdentityLayer<> >(); // input layer
    model.Add(recurrent);
    model.Add<Linear<> >(4,10); //output layer from 4 hidden units with 10 output units
    model.Add<LogSoftMax<> >();
    
    StandardSGD opt(0.1, 1, 500 * input.n_cols, -100 ); //stepsize, ??, max iterations, ??
    model.Train(input, labels, opt); // input = preceding sequence of notes, labels = the next notes, opt = type of optimizer we use
    
    arma::mat prediction;
    model.Predict(input, prediction); //predictors, then where we store output to
    
    size_t error = 0;
    for (size_t i = 0; i < prediction.n_cols; ++i)
    {
        const int predictionValue = prediction.col(i);
        
        const int targetValue = validation.col(i);
        
        if (predictionValue == targetValue)
        {
            good++;
        }
    }
    
    double classificationError = 1 - double(good) / prediction.n_cols;
    
    if (classificationError <= 0.2)
    {
        ++successes;
        break;
    }
}
    
    BOOST_REQUIRE_GE(successes, 1);

}

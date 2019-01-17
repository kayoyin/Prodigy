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

using namespace arma;
using namespace std;
using namespace mlpack::data;

class Compose {
public:
  Compose(); // constructor
 
  int compose();
  
  void predictNotes();
  
};

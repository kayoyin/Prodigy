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

void predictNotes(RNN<>& model,
                  const unsigned int sequence_length, const unsigned int size_notes, const unsigned int size_music)
{
    
    cube start = cube(1, 1, sequence_length); // we initialize generation with a sequence of random notes
    for (unsigned int i = 0; i < sequence_length; i++)
    {
	start(0,0,i) = rand() % size_notes + 1; // random integer between 1 and size_notes
    }
	
    mat music = mat(size_music,1, fill::zeros);	
    cube compose;
    for (unsigned int i = 0; i < size_music; i = i + sequence_length)	
    {	    
    	// Getting predictions after starting notes .
    	model.Predict(start, compose);
	    
    	// Fetching the notes from probability vector generated.	    
    	for (unsigned int j = 0; j < sequence_length; j++)
	{
		int note = arma::as_scalar(arma::find(
          arma::max(compose.slice(j).col(0)) == compose.slice(j).col(0), 1)) + 1;
		music(i+j,0) = note;
		start(0,0,j) = note; // update start to continue generation taking into account the sequence just predicted
	}

	
    }
    cout << "Saving predicted notes to \"../utils/sonata.csv\" ..." << endl;
    data::Save("../utils/sonata.csv", music); 
    cout << "Music saved to \"sonata.csv\"" << endl;
}

int main () {

  mat tempDataset;
  const int rho = 5;
  data::Load("../utils/training.csv", tempDataset, true); // read data from this csv file, creates arma matrix with loaded data in tempDataset
  const int size_notes = max(tempDataset.row(0)) + 1;
  const int sequence_length = rho;
  const int size_music = 300; //must be a multiple of sequence_length
    
  cout << "Loading trained model ..." << endl;
  RNN<> model(rho);
  data::Load("model.xml", "model", model);	
  
  cout << "Composing ..." << endl;
  predictNotes(model, sequence_length,size_notes, size_music);
  cout << "Finished :)" << endl;
  
  return 0;
}

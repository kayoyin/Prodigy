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
                  const unsigned int sequence_length, const unsigned int size_music, const int size_notes)
{
    
    cube start = cube(1, 1, sequence_length); // we initialize generation with a sequence of random notes
    mat music = mat(size_music,1, fill::zeros);	
 
    mat startnotes;
    // Load notes from user input
    data::Load("../utils/startnotes.csv", startnotes, true);
	
    // protecting against insufficient number of notes from user input	
    for (unsigned int i = 0; i < sequence_length; i++)
    {
    	int note = rand() % size_notes + 1; // random integer between 1 and size_notes
	start(0,0,i) = note;
	music(i,0) = note;
    }
	

    for (unsigned int i = 0; i < startnotes.n_rows; i++)
    {
	if( i == sequence_length) {
         // terminate the loop
        break;
	}
	start(0,0,i) = startnotes(i,0);
	music(i,0) = startnotes(i,0);
      
    }
    
	
    cube compose;
    for (unsigned int i = sequence_length; i < size_music; i = i + sequence_length)	
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
    cout << "Saving predicted notes to \"sonata.csv\" ..." << endl;
    data::Save("../utils/sonata.csv", music); 
    cout << "Music saved to \"/utils/sonata.csv\"" << endl;
}

int main () {

  mat tempDataset;
  const int rho = 20; // must be the same rho as the one used to train the model
  const int sequence_length = rho; // length of sequence taken into consideration during training
  const int size_music = 300; //number of notes in composed music: must be a multiple of sequence_length
  const int size_notes = 72; //size of dictionary used
    
  cout << "Loading trained model ..." << endl;
  RNN<> model(rho); 
  data::Load("modelgui.xml", "model", model);	
  
  cout << "Composing from user input ..." << endl;
  predictNotes(model, sequence_length,size_music, size_notes);
  cout << "Finished :)" << endl;
  
  return 0;
}

#Prodigy

##Table of Contents

    Project Description.
    Usage Instructions
    Translating
    Neural Network
    Other

## 1. Project Description

Our aim is to train a machine learning algorithm for music composition. For this, we used mlpack, a C++ library, to build a recurrent neural network with a LSTM layer. Since training is more optimal when using number, we wrote an algorithm (with the help of external libraries) (is this true tranlating people?) that translates and retranslates MIDI files into csv files containing integers. After the LSTM was build and translating processes was completed, we trained our network using Bach MIDI files located in the folder, insert here. A sample composition is uploaded in insert here and the some training weights is saved in the folder insert here if the user wants readily to compose music.

For the course of the project we thereby divided ourselves into two teams, the "Translators" and the "Builders". For more information on the respective parts, see the translating and neural network sections.

section about GUI if applicable

## 2. Usage Instructions
Installing the dependencies

After cloning from the project, you first need to install mlpack and its dependencies. To do so, simply run the following command:

$> bash install_mlpack.bash 

Build and compile

To build the project, run the following commands:

$> mkdir build
$> cd build
$> cmake ../

Then, to compile the project, enter the build folder and type make.

$> cd build
$> make

Running the project

Now, the project will have two executables train and compose which you can execute with the commands

$> ./train
$> ./compose

You can either train a model from scratch or continue training on a saved model saved /utils/model.xml by executing train. You will need to have a training file /utils/training.csv that is a vector of integers corresponding to translated music.

Then, you can generate compositions from the saved model by executing compose.

Now in order to be able to use ./train and ./compose from midi files that are added by the user, notice that there are to script that are given in the main folder: translatescript and backscript. To change them to executable, the commands for both are :

$ chmod +x translatescript
$ chmod +x backscript

Now translatescript's description is within the file translatescript and similarly for backscript.

To use both of them, simply run them like any executable through the following command:
$./translatescript
$./backscript




## 3. Translating

### Decision-making on the filetype:

To begin with, we had to make a choice on what kind of audio file we wanted to use, and what kind of storing file for sheets we wanted to use. And finally we made up our mind on using midi files for audio and csv files for sheet writing purpose. Now stating the reasons: midi files are easy to find which implies more training data; for the latter, we were familiar with csv files, but most importantly, csv files were the input for the Neural Network and also its output.

### Dealing with filetype:

Now in order to get the midi files in a human-readable csv file, we had to look for an external library on github that happened to be written in C. Fortunately, it was possible for us to still compile it using standard g++ command. Inside this external package, there was two main functions that we fetched: midicsv and csvmidi. Both functions' use are self-explanatory, moreover both functions receive two arguments, which are (depending on which function) a csv filename and a midi filename.
Csv files outputted from the functions have a specific format which is a header, the main body and an ending line. The main parts has the following general format: "Track, Tick, Note_ON/Note_Off, instrument,note,velocity". A remark: among thoses, Tick is the absolute time value, and the velocity when 0 means that the note is not benig played. 

### Translating Process: 

Now for the csv given by the function midicsv to yield a csv that is fit for the Neural Network. We had to pull out from the CSV only the main informations which are the tick, the notes, and the velocity. The assumptions that were noticeable, that we made were:
- Considering the tempo to be regular, and thus not truly considering the difference between ticks.
- In the csv newly written, we assume a note that is played continuously for say 3 intervals, to be played 3 times at each interval.
- Getting rid of parts of the header that were not necessarly useful.
Now, specifically, in order to parse in the csv we had to use a class CSVReader, to get every lines in a vector of vector, and then reuse it. 

For the translating back process, this time around, we made others practical assumptions or fixed rules:
- Unlike the potential appearance of Note_Off in the firstly created csv, there is no Note_off in the translated back one
- We consider as said a continuous note on an interval to be played multiple times on that interval
- velocities were constant, either 0 when the note is not played and 100 when it is
- tick as well were incremented constantly, an incrementing that is written in the code, to make the notes regular.
- In the ouput of translate back, every header are the same (we had made some test to see whether the header changed drastically the midi output, which it did not).

Practical detail: In order to be a bit more efficient, we decided not to fix a given dictionary (the dictionary must be obviously the same for both translate, and translateback), but to create it when translating and having it created in translateback to use it.(creating the dictionary the same way we did before). The dictionary creation follows an easy principle which is to map a new note to the image of the last seen note + 1. 

### Mechanical for the purpose of training:

To make the training of Neural Network easier, we had to find a way to be able to translate multiple midi files and then "merge" them to get one sole csv file that is given as input to the Nerual Network. And therefore we created a merge.cpp which executable is called merge that as mentioned, merges csv files that were transformed from midi files beforehand. Merge receives as an argument a csv file, that will append to the final file. 

### Also talk here about translate algorithm accuracy evaluation

## 4. Neural Network
### Music composition with recurrent neural networks (RNN)

We chose to implement an artificial neural network for its sheer prediction power after enough training. Because of the sequential nature of music, the dependency of the value of the notes at a certain time step to the music that precedes it, a recurrent neural network rather than a feedforward neural network is the necessary choice.

More specifically, we implemented a Long short-term memory network (LSTM). This article gives a through explanation of LSTM networks, but the main idea is that LSTM units in our RNN will be able to recognize and learn long-time patterns, which is what we need for music composition.

We decided to use an external C++ library for machine learning, mlpack to build and train our LSTM network.
Structure of our model

We defined two hidden LSTM layers with 512 memory units, and a dropout layer of probability 0,3 which helps avoid overfitting. The output layer uses the softmax activation function to output the log of the probability prediction for each notes present in our model. The problem can be defined as a single integer classification problem with each note being a possible class, therefore training is minimizing the negative log-likelihood, which maximizes the likelihood that the output of the model produces the data actually observed. We also use the ADAM optimization algorithm for speed.
Format of our data

In general, LSTM networks expect input data with different features, time step and points. Specifically, in the mlpack library, the LSTM layer takes in an armadillo cube where each row corresponds to a "feature", in our case we only have one feature that is the note, each column corresponds to a "time step" which is the point in time within our sequence of music, and each slice (the third dimension of our tensor) corresponds to a point, or the specific sequence of notes at the time step considered.

The training labels or what the model is expected to output is defined as the single note that follows each sequence of notes considered at each time step. Here, it is an armadillo cube again with the same dimensions as the training data, where each column or time step stores the note corresponding to the time step. We use the entirety the music data for training, we do not have a validation set as it does not make sense in the context where we want the model to learn the probabilities of notes given a musical sequence.

The output of our model is a cube with one row, as many columns as the total number of different notes, and as many slices as the length of the sequence considered at each time step. To extract the notes given a probability vector, we simply look for the index with the maximum probability and choose that as the note predicted by our model.

We measure accuracy after each cycle of training by calculating the percentage of notes from the prediction that coincide with the note from the training set at the respective time step. Note that this accuracy is only one indicator to keep track of training, and we expect the percentage to stay low as the model should still have some creativity. This accuracy mostly ensures no overfitting happens, as we do not want the model to reproduce exactly the given training music.
### Generating music after training

Once we have trained a model, we can save the model with the trained weights for later use. We have chosen to have the model be saved automatically after each 20 cycles of training.

To have the network generate music, we then load the previously trained model. Essentially, the trained network is a prediction model, so it needs a starting point for composition. We use a randomly generated short sequence of notes as the seed sequence which we feed into the prediction method of the model. From then on, we feed the new sequence predicted by the model to obtain the next predicted sequence, and we continue this step until we get a music sequence of our desired length.

## 5. What now?

Here are some extension ideas that have not been explored that could possibly improve our implementation:

    Adding a temperature layer before the logsoftmax layer, to control the randomness of predictions by the LSTM.

Temperature will rescale the logits before putting them through the softmax function, where high temperature will give similar probabilities to each notes and low temperature will give higher probability to the most expected note. We can therefore think of temperature as the parameter of the LSTM's creativity, where a model with low temperature will be more "creative".

    Single-note training

In our project, each combination of notes from the training set were encoded by a unique integer, which limits the combinations of notes the model can produce to ones it has already seen during training. Another way to encode music would be to give each note a unique character, then encode combinations of notes as combinations of these characters. This encoding is more complex but would be a more realistic modelization of true music composition.

Furthermore, we can add complexity by finding a better way to encode rhytm. Currently, the scope of our training music is limited to music containing notes with lower velocities. This is due to the way we translation of the MIDI files. In particularly the tick like representation of music and the duration of time that note is help adds complexity to the translating dictionary. translating hu,man verify the above/make more complete please

    Improve measure used to keep track of training

The accuracy measure we implemented is a very naive measure where we simply calculate the percentage of notes the model outputs given the training set, to the actual notes from the training set. This measure is misleading and not well-suited in the scope of this project, as a model we would consider very good at producing music does not, and should not even have a high accuracy during training. This leads to a fundamental question of how to classify what music is considered as "good". Answering such a question might require developing a criteria based on musical theory.

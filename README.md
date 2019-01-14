# Prodigy

# Table of Contents 

1. [ Project Description. ](#desc)
2. [ Usage Instructions ](#usage)
3. [ Translating ](#trans)
4. [ Neural Network](#network)
5. [ Other ](#other)

<a name="desc"></a>
## 1. Project Description

Our aim is to train a machine learning algorithm for music composition. For this, we used mlpack, a C++ library, to build a recurrent neural network with a LSTM layer. Since training is more optimal when using number, we wrote an algorithm (with the help of external libraries) (*is this true tranlating people?*) that translates and retranslates MIDI files into csv files containing integers. After then LSTM was build and translating processes was completed, we trained our network using Bach MIDI files located in the folder, *insert here*. A sample composition is uploaded in *insert here* and the some training weights is saved in the folder *insert here* if the user wants readily to compose music. 

For the course of the project we thereby divided ourselves into two teams, the "Translators" and the "Builders". For more information on the respective parts, see the translating (*link*) and neural network(*link*) sections. 

Link to possible website? 

<a name="usage"></a>
## 2. Usage Instructions

### Installing the dependencies
After cloning from the project, you first need to install [mlpack](https://www.mlpack.org) and its dependencies.
To do so, simply run the following command:
```
$> bash install_mlpack.bash 
```

### Build and compile
To build the project, run the following commands:
```
$> mkdir build
$> cd build
$> cmake ../
```

Then, to compile the project, enter the `build` folder and type make.
```
$> cd build
$> make
```

### Running the project
Now, the project will have two executables `train` and `compose` which you can execute with the commands 
```
$> ./train
$> ./compose
```
You can either train a model from scratch or continue training on a saved model saved `/utils/model.xml` by executing `train`.
You will need to have a training file `/utils/training.csv` that is a vector of integers corresponding to translated music.

Then, you can generate compositions from the saved model by executing `compose`.

<a name="trans"></a>
## 3. Translating 

- explain basic process behind your code - 

old description - 
The first team, "Translators", will define how music will be represented by text: what notes are mapped to which characters, how time/ticks are defined through text... They will also write an algorithm who takes as input MIDI files and outputs a text file with relevant elements in defined format. 

Also talk here about evaluating translate algorithm accuracy evaluation 

<a name="network"></a>
## 4. Neural Network 
### Music composition with recurrent neural networks (RNN)
We chose to implement an artificial neural network for its sheer prediction power after enough training. Because of the sequential nature of music, the dependency of the value of the notes at a certain time step to the music that precedes it, a recurrent neural network rather than a feedforward neural network is the necessary choice.

More specifically, we implemented a Long short-term memory network (LSTM). This [article](http://colah.github.io/posts/2015-08-Understanding-LSTMs/) gives a through explanation of LSTM networks, but the main idea is that LSTM units in our RNN will be able to recognize and learn long-time patterns, which is what we need for music composition.

We decided to use an external C++ library for machine learning, [mlpack](https://www.mlpack.org) to build and train our LSTM network.

### Structure of our model
We defined two hidden LSTM layers with 512 memory units, and a dropout layer of probability 0,3 which helps avoid overfitting. The output layer uses the softmax activation function to output the log of the probability prediction for each notes present in our model.
The problem can be defined as a single integer classification problem with each note being a possible class, therefore training is minimizing the [negative log-likelihood](https://ljvmiranda921.github.io/notebook/2017/08/13/softmax-and-the-negative-log-likelihood/), which maximizes the likelihood that the output of the model produces the data actually observed. We also use the ADAM optimization algorithm for speed.

### Format of our data
In general, LSTM networks expect input data with different features, time step and points. Specifically, in the mlpack library, the LSTM layer takes in an armadillo cube where each row corresponds to a "feature", in our case we only have one feature that is the note, each column corresponds to a "time step" which is the point in time within our sequence of music, and each slice (the third dimension of our tensor) corresponds to a point, or the specific sequence of notes at the time step considered.

The training labels or what the model is expected to output is defined as the single note that follows each sequence of notes considered at each time step. Here, it is an armadillo cube again with the same dimensions as the training data, where each column or time step stores the note corresponding to the time step.
We use the entirety the music data for training, we do not have a validation set as it does not make sense in the context where we want the model to learn the probabilities of notes given a musical sequence.

The output of our model is a cube with one row, as many columns as the total number of different notes, and as many slices as the length of the sequence considered at each time step. To extract the notes given a probability vector, we simply look for the index with the maximum probability and choose that as the note predicted by our model. 

We measure accuracy after each cycle of training by calculating the percentage of notes from the prediction that coincide with the note from the training set at the respective time step. Note that this accuracy is only one indicator to keep track of training, and we expect the percentage to stay low as the model should still have some creativity. This accuracy mostly ensures no overfitting happens, as we do not want the model to reproduce exactly the given training music. 

### Generating music after training
Once we have trained a model, we can save the model with the trained weights for later use. We have chosen to have the model be saved automatically after each 20 cycles of training.

To have the network generate music, we then load the previously trained model. Essentially, the trained network is a prediction model, so it needs a starting point for composition. We use a randomly generated short sequence of notes as the seed sequence which we feed into the prediction method of the model. From then on, we feed the new sequence predicted by the model to obtain the next predicted sequence, and we continue this step until we get a music sequence of our desired length.

<a name="extra"></a>
## 5. What now?

Here are some extension ideas that have not been explored that could possibly improve our implementation:
* Adding a temperature layer before the logsoftmax layer, to control the randomness of predictions by the LSTM. 

Temperature will rescale the logits before putting them through the softmax function, where high temperature will give similar probabilities to each notes and low temperature will give higher probability to the most expected note. We can therefore think of temperature as the parameter of the LSTM's creativity, where a model with low temperature will be more "creative".
* Single-note training

In our project, each combination of notes from the training set were encoded by a unique integer, which limits the combinations of notes the model can produce to ones it has already seen during training. Another way to encode music would be to give each note a unique character, then encode combinations of notes as combinations of these characters. This encoding is more complex but would be a more realistic modelization of true music composition.








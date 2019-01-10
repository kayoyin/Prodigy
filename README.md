# Prodigy

# Table of Contents 

1. [ Project Description. ](#desc)
2. [ Usage Instructions ](#usage)
3. [ Translating ](#trans)
4. [ Neural Network](#network)
5. [ Blah blah](#other)

<a name="desc"></a>
## 1. Project Description

sometext

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
You can either train a model from scratch or continue training on a model saved in `/utils` by executing `train`.
You will need to have a training file `/utils/training.csv` that is a vector of integers corresponding to translated music.

Then, you can generate compositions from the saved model by executing `compose`.

<a name="trans"></a>
## 3. Translating 

sometext

<a name="network"></a>
## 4. Neural Network 

sometext

<a name="extra"></a>
## 5.Blah blah



## Project description:

Our aim is to train a machine learning algorithm for music composition. For this, we will use an existing LSTM library on C++ that we will use to train. Training will be done using text files, so we will also need to write an algorithm to convert MIDI files into usable text.

For the first 3 weeks of the project, we divided ourselves into two teams.

The first team, "Translators", will define how music will be represented by text: what notes are mapped to which characters, how time/ticks are defined through text... They will also write an algorithm who takes as input MIDI files and outputs a text file with relevant elements in defined format.

The second team, "Builders", will construct the AI used for training. They will grasp the theory and functioning of neural networks, determine which library to use, understand how to use it for music generation, and from the third week start training using translated MIDI files of Mozart's music.



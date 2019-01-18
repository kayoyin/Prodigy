# Prodigy

## Table of Contents


1. [Project Description.](#project)
2. [Usage Instructions](#usage)
3. [Translating](#trans)
4. [Neural Network](#network)
5. [The GUI](#gui)
6. [What Next?](#ext)


<a name="project"></a>
## 1. Project Description 

Our aim is to train a machine learning algorithm for music composition. For this, we used mlpack, a C++ library, to build a recurrent neural network with a LSTM layer. Since training is more optimal when using number, we wrote an algorithm (with the help of external libraries)  that translates and retranslates MIDI files into csv files containing integers. After the LSTM was build and translating processes was completed, we trained our network using Bach MIDI files. You can listen to an album of music produced by different models we trained [here](https://soundcloud.com/kayo-yin/sets/prodigy)

Since we had some time left at the end of the project, we took on an additional challange of complementing the project by created a GUI where users play a tune on a digital piano which the model uses as the beginning of its composition. This is clearly an extension and indepednt from our initial goal. Since this was not part of our original design, the code was not optimized for this use. Though, we thought it could be a fun  and pursued it nonetheless. Our GUI is therefore still in its initial stages and has potential erros and bugs. Since we spent time on it and we think it's a creative extension, we will include it as a idea to be perfected later on nonetheless. 

For the course of the project we divided ourselves into two teams, the "Translators" and the "Builders". For more information on the respective parts, see the translating and neural network sections.





<a name="usage"></a>
## 2. Usage Instructions 

There are two possible ways of composing music using project prodigy. In the first option (more technically advanced), the user has the freedom of choosing whichever music he or she likes to train the model on. In theory, the music the model produces will have similar stylistics features as the training data. To evualate the "friendliness to translation" of the music see, the user can use our translating evaluation algorithm *see end of the translating section for more information*.   Our second, more creative option is designed for the musically inclided users with a graphics user interface. By running the interface using QT, the user is able to compose a short extract using a digital keyboard, which the model will use as the beginning of the music it composes

See the respective sections for user information on method of use. But first, it is necessary to install mlpack (the library used to implement the neural network) and all its dependencies. 

## Installing the dependencies

After cloning the repository, the user first needs to install mlpack and its dependencies. To do so, simply run the following command in terminal:

    $> bash install_prodigy.bash 



## Running the project

### Method 1: Starting from Scratch

Firstly, the user must ensure that his/her selected MIDI files is stored in the folder called, *music_gen* in his/her local copy of the repository. From here, it is necessary to make use of our provided translating algorithms to create a csv file which the model can train on. Notice that there are two scripts that are given in the main folder: translatescript and backscript. These two scripts are designed to ensure fluidity for the user and automate the translating and retranslating process. 


Running two following commands will change `translatescript` to an `executable`, and then make use of the translating script. 

    $> chmod +x translatescript
    $> ./translatescript

The `translatescript` translates and merges all the midi files in a format ready to use for the neural network. Moreover, it saves the output csv file in the utils folder. From there on, the training function automatically loads the data. Since all the data is properly formatted for use, the user can start traing the model. To do this, simply run the following command in terminal: 

    $> ./train
    
Once training is complete (do not be alarmed if this takes time), simply run the following command to start composing music: 

    $> ./compose


Now, the last thing to do is translate the output of the neural network (.csv) into an audiofile (.mid). The backscript automatically loads the output file and saves the audio file as *final.mid* in the main folder of the repository. Run the following commands to execute this: 

    $> chmod +x backscript
    $> ./backscript

If the user is experiencing any trouble playing the midi file on their machine, the following website can be used to convert to mp3: https://www.onlineconverter.com/midi-to-mp3

Explanation on steps taken by translatescript:
- We are here making the executables first
- Making sure to put the midi files that the user added in the right directory
- Creating all the csvs from the midi files that were moved accordingly
- Now as said in the ReadMe, merging all the csvs to a merged one
- Moving the merged csv to the right directory then translating it + other details. The translation will be an actual csv file which is stored in utils

Explanation on steps taken by backscript:
- Going into the right directory
- After having fetched the ouptut from Neural network, we put it trough translateback which outputs a final csv.
- Having the right final csv, we output using csvmidi the final midi file that is kept in the main folder


### Method 2: 

To use the GUI ensure you have QT creator installed on your machine. From there, open the `piano.pro` file (located in the `pianoGUI` folder) in QT and run it using the QT interface. This will create a build folder in your local copy of the repository called "build", open the folder and click the on the file named `piano.exe`, from here, user instructions are self explanatory. 

<a name="trans"></a>
## 3. Translating 

### Decision-making on the filetype:

To begin with, we had to make a choice on what kind of audio file we wanted to use, and what kind of storing file for sheets we wanted to use. And finally we made up our mind on using midi files for audio and csv files for sheet writing purpose. Now stating the reasons: midi files are easy to find which implies more training data; for the latter, we were familiar with csv files, but most importantly, csv files were the input for the Neural Network and also its output.

### Dealing with filetype:

Now in order to get the midi files in a human-readable csv file, we had to look for an external library on github that happened to be written in C. Fortunately, it was possible for us to still compile it using standard g++ command. Inside this external package, there was two main functions that we fetched: midicsv and csvmidi. Both functions' use are self-explanatory, moreover both functions receive two arguments, which are (depending on which function) a csv filename and a midi filename.
Csv files outputted from the functions have a specific format which is a header, the main body and an ending line. The main part has the following general format: "Track, Tick, Note_ON/Note_Off, instrument,note,velocity". A remark: among thoses, Tick is the absolute time value, and the velocity when 0 means that the note is not being played. 

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

Practical detail: In order to be a bit more efficient, we decided not to fix a given dictionary (the dictionary must be obviously the same for both translate, and translateback), but to create it when translating and having it created in translateback to use it (creating the dictionary the same way we did before). The dictionary creation follows an easy principle which is to map a new note to the image of the last seen note + 1. 

### Mechanical for the purpose of training:

To make the training of the neural network easier, we had to find a way to be able to translate multiple midi files and then "merge" them to get one sole csv file that is given as input to the Nerual Network. And therefore we created a merge.cpp which executable is called merge that as mentioned, merges csv files that were transformed from midi files beforehand. Merge receives as an argument a csv file, that will append to the final file. 

### Evaluating Translating Accuracy
Once we had our translation algorithms, we decided to formally assess the accuracy of our codes. testtrans.cpp takes as input a csv file of a music translated by “midicsv” (not transformed) and a csv file translated both ways by translate.cpp and translateback.cpp. It compares the number of ordered similar notes and computes the average difference of the time between two consecutive ticks and the average difference in velocity between the files original and trans. We noticed that some notes from the original piece of music were removed during translation, creating some shifts between the two files. Qualitatively, it doesn't seem to alter the music.

This algorithm was a way to quantify the lost but also to compare composers like Mozart and Bach, and see which one fitted our project the best. We obtained more accuracy with Bach since it was the best compromise between lack of accuracy and good quality of music. Indeed, its translation didn't alter its tick interval - and therefore the rhythm - too much.

To run the algorithm, open it to modify the names of the file you want to use: the original one for original and original1 and the translated one for trans. Then compile and run it using the following lines in the terminal:

         $> g++ -std=c++11 testtrans.cpp -o ttrans
 
         $> ttrans


<a name="network"></a>
## 4. Neural Network 
### Music composition with recurrent neural networks (RNN)

We chose to implement an artificial neural network for its sheer prediction power after enough training. Because of the sequential nature of music, the dependency of the value of the notes at a certain time step to the music that precedes it, a recurrent neural network rather than a feedforward neural network is the necessary choice.

More specifically, we implemented a Long short-term memory network (LSTM). This [article](http://colah.github.io/posts/2015-08-Understanding-LSTMs/) gives a through explanation of LSTM networks, but the main idea is that LSTM units in our RNN will be able to recognize and learn long-time patterns, which is what we need for music composition.

We decided to use an external C++ library for machine learning, [mlpack](https://www.mlpack.org) to build and train our LSTM network.

### Structure of our model

We defined two hidden LSTM layers with 512 memory units, and a dropout layer of probability 0,3 which helps avoid overfitting. The output layer uses the softmax activation function to output the log of the probability prediction for each notes present in our model. The problem can be defined as a single integer classification problem with each note being a possible class, therefore training is minimizing the negative log-likelihood, which maximizes the likelihood that the output of the model produces the data actually observed. We also use the ADAM optimization algorithm for speed.

The problem can be defined as a single integer classification problem with each note being a possible class, therefore training is minimizing the [negative log-likelihood](https://ljvmiranda921.github.io/notebook/2017/08/13/softmax-and-the-negative-log-likelihood/), which maximizes the likelihood that the output of the model produces the data actually observed. We also use the ADAM optimization algorithm for speed.

### Format of our data

In general, LSTM networks expect input data with different features, time step and points. Specifically, in the mlpack library, the LSTM layer takes in an armadillo cube where each row corresponds to a "feature", in our case we only have one feature that is the note, each column corresponds to a "time step" which is the point in time within our sequence of music, and each slice (the third dimension of our tensor) corresponds to a point, or the specific sequence of notes at the time step considered.

The training labels or what the model is expected to output is defined as the single note that follows each sequence of notes considered at each time step. Here, it is an armadillo cube again with the same dimensions as the training data, where each column or time step stores the note corresponding to the time step. We use the entirety the music data for training, we do not have a validation set as it does not make sense in the context where we want the model to learn the probabilities of notes given a musical sequence.

The output of our model is a cube with one row, as many columns as the total number of different notes, and as many slices as the length of the sequence considered at each time step. To extract the notes given a probability vector, we simply look for the index with the maximum probability and choose that as the note predicted by our model.

We measure accuracy after each cycle of training by calculating the percentage of notes from the prediction that coincide with the note from the training set at the respective time step. Note that this accuracy is only one indicator to keep track of training, and we expect the percentage to stay low as the model should still have some creativity. This accuracy mostly ensures no overfitting happens, as we do not want the model to reproduce exactly the given training music.
### Generating music after training

Once we have trained a model, we can save the model with the trained weights for later use. We have chosen to have the model be saved automatically after each 20 cycles of training.

To have the network generate music, we then load the previously trained model. Essentially, the trained network is a prediction model, so it needs a starting point for composition. We use a randomly generated short sequence of notes as the seed sequence which we feed into the prediction method of the model. From then on, we feed the new sequence predicted by the model to obtain the next predicted sequence, and we continue this step until we get a music sequence of our desired length.

<a name="gui"></a>
## 5. The GUI 

For the User interface ,with the help of Qt creayor, we tried to make it as user friendly as possible. with thorough steps and guidelines on the main window that a user can read while running the program. 

There are 3 main parts of the GUI:

- The virtual piano. 
which is a 3 octaves virtual piano that we implemented with an accurate sound of every notes will be played when pressed.
- A list view.
this list view will help the user to remember the history of all the notes that they pressed. This box will also be added with various actions that the user did (e.g. "start / stop recording" , "training", etc)
- Buttons
These buttons will aid and lead the user to eventually play their own made .mid file



<a name="ext"></a>
## 6. What Next? 

Here are some extension ideas that have not been explored that could possibly improve our implementation:

### Adding a temperature layer before the logsoftmax layer, to control the randomness of predictions by the LSTM.

Temperature will rescale the logits before putting them through the softmax function, where high temperature will give similar probabilities to each notes and low temperature will give higher probability to the most expected note. We can therefore think of temperature as the parameter of the LSTM's creativity, where a model with low temperature will be more "creative".

### Single-note training

In our project, each combination of notes from the training set were encoded by a unique integer, which limits the combinations of notes the model can produce to ones it has already seen during training. Another way to encode music would be to give each note a unique character, then encode combinations of notes as combinations of these characters. This encoding is more complex but would be a more realistic modelization of true music composition.

Furthermore, we can add complexity by finding a better way to encode rhythm. Currently, the scope of our training music is limited to music containing notes with lower velocities. This is due to the way we translate MIDI files. In particularly the tick like representation of music and the duration of time that note is help adds complexity to the translating dictionary. !!!! translating human verify the above/make more complete please, talk about shifts?

### Make the trained models less dependent on the training data

This issue is related to the above idea. In our current implementation, one significant contraint is that each trained model can only be used with the specific dictionary map between integers and notes related to its training set. This is why although we have several different trained models, we have only provided one for the user to play around with in the scope of this project. We would like to have a more generalized trained models that would all use the same dictionary mapping, so that with the same translation codes, the user will be able to test several different models.

For completeness, we have still uploaded some of our trained models in this repository [here](https://github.com/kayoyin/ProdigyModels), which you can clone, if you have git lfs installed, by running 
```
$> git lfs clone https://github.com/kayoyin/ProdigyModels.git
```
*note that this will take several minutes due to the large file sizes*

### Accuracy measure used to keep track of training

Initially, we implemented a very naive accuracy measure naive measure where we simply calculate the percentage of notes the model outputs given the training set, to the actual notes from the training set. This measure is misleading and not well-suited in the scope of this project, as a model we would consider very good at producing music does not, and should not even have a high accuracy during training. This leads to a fundamental question of how to classify what music is considered as "good". Answering such a question might require developing a criteria based on musical theory.



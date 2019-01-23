# Prodigy

## Table of Contents


1. [Project Description](#project)
2. [Usage Instructions](#usage)
3. [Translating](#trans)
4. [Neural Network](#network)
5. [Bonus: The GUI](#gui)
6. [What Next?](#ext)
7. [The Team](#team)


<a name="project"></a>
## 1. Project Description 

Our aim is to train a machine learning algorithm for music composition. For this, we used mlpack, a C++ library, to build a recurrent neural network with a LSTM layer. Since training is more optimal when using integers, we wrote an algorithm (with the help of external libraries)  that translates and retranslates MIDI files into csv files containing integers. After the LSTM was built and translating processes were completed, we trained our network first using Bach MIDI files. To complete this project, the team was split into two groups, "Builders" and "Translators". 

You can listen to an album of music produced by different models we trained [here](https://soundcloud.com/kayo-yin/sets/prodigy). Notice how the quality of music the model has created increasingly improves from the first hit "Failing is Passing" to later, particularly noting "Love is a Deadly Cup of Tea". 


<a name="usage"></a>
## 2. Usage Instructions 

In our design, the user has the freedom of choosing whichever music he or she likes to train the model on. In theory, the music the model produces will have similar stylistics features as the training data. To evaluate the "friendliness to translation" of the music, the user can use our translating evaluation algorithm (*see end of the translating section for more information*).  

Before running the project, it is necessary to install mlpack (the library used to implement the neural network) and its dependencies. Make sure you have a stable internet connection for installation, and we suggest at least 20 GB of free storage on your machine to run all features of the project.

## Installing the dependencies

After cloning the project in a local repository named `Prodigy`, the user first needs to install mlpack and its dependencies. To do so, enter the local repository and simply run the following command in terminal:

    $> bash install_prodigy.bash 

The above script will equally make and build the project, creating the necessary executables, and download a pre-trained model.

## Running the project


Firstly, the user must ensure that his/her selected MIDI files is stored in the folder called, `music_gen` in the local repository. It is necessary to make use of our provided translating algorithms to create a csv file which the model can train on. Notice that there are two scripts that are given in the folder script: translatescript and backscript. These two scripts are designed to ensure fluidity for the user and automate the translating and retranslating process. 


Running three following commands will change `translatescript` to an executable. 

    $> cd script
    $> chmod +x translatescript
    $> ./translatescript

`translatescript` translates and merges all midi files in a format ready to use for the neural network. Moreover, it saves the output csv file in the `utils` folder. From there on, the training function automatically loads the data and the user can start training the model. To do this, simply run the following commands: 

    $> cd ../build
    $> ./train
    
The first time the above command is run, there might be a message `[WARN] Unable to open file...` which you can ignore. Because in our implementation, 100 data points are taken at each iteration of training, training data with less than 100 notes will lead to a runtime error, and besides, such small data will not be effective for training.

Training is configured to run 500 training cycles for each model, but the user can safely pause and resume training once the model is saved, as long as the same training data is used. Every 5 cycles, the model is automatically saved, which you can check with the message `Checkpoint at cycle _`

Training to a reasonable model will take a lot of time, we recommend training on a remote machine through `tmux` sessions for extended periods of training.

Once training is complete, simply run the following command to start composing music: 

    $> ./compose


Now, the last thing to do is translate the output of the neural network (.csv) into an audiofile (.mid). The backscript automatically loads the output file and saves the audio file as `final.mid` in the main folder of the repository. Run the following commands to execute this: 

    $> cd ../script
    $> chmod +x backscript
    $> ./backscript

If the user is experiencing any trouble playing the midi file on their machine, [this website](https://www.onlineconverter.com/midi-to-mp3) can be used to convert to mp3. 

Below is further explanation on how each translating scripts work. 

`translatescript`:
- We make the executables first
- Making sure to put the midi files that the user added in the right directory
- Creating all the csvs from the midi files that were moved accordingly
- Merging all the csvs to a single one
- Moving the merged csv to the right directory then translating it + other details. The translation will be a csv file stored in `utils`

`backscript`:
- Go into the right directory
- After having fetched the output from the model, we put it through `translateback` which outputs a final csv.
- We output using `csvmidi` the final midi file to the main folder

We provide two sample midi files in `music_gen`, as well as the already translated file `/utils/training.csv` and other files that could be used for training under `/samples`, and a composition from a trained model `sonata.csv` for users who would like to run some features without going through all the steps before.

<a name="trans"></a>
## 3. Translating 

### Deciding on the filetype:

To begin with, we had to make a choice on what kind of audio file and storing file for sheets we wanted to use. We made up our mind on using midi files for audio and csv files for sheet writing purpose. Midi files are easy to find fo more training data, and csv files are simple files compatible with the input and output of the neural network model.

### Dealing with filetype:

Now in order to get the midi files in a human-readable csv file, we found an external library on GitHub written in C. We use two main functions from this package: `midicsv` and `csvmidi`. Both functions' use are self-explanatory, and receive two arguments (depending on which function): a csv filename and a midi filename.
Csv files outputted from the functions have a specific format: header, the main body and an ending line. The main part has the following general format: `Track, Tick, Note_ON/Note_Off, instrument,note,velocity`. Among these, `Tick` is the absolute time value, and 0 `velocity` shows the note is not being played. 

### Translating Process: 

Now for the csv given by `midicsv` to yield one fit for the neural network, we extracted only the necessary informations: the tick, the notes, and the velocity. 
The assumptions we made are:
- Considering the tempo to be regular, thus not truly considering the difference between ticks.
- In the csv newly written, we assume a note that is played continuously for say 3 intervals, to be played 3 times at each interval.
- Parts of the header we got rid of are not useful.
In order to parse in the csv we used the class `CSVReader`, to get every lines in a vector of vector, and then reuse it. 

For the translating back process, we made other practical assumptions and fixed rules:
- There is no `Note_off` in the csv translated back
- We consider a continuous note on an interval to be played multiple times on that interval
- Velocities are binary: 0 when the note is not played and 100 when it is
- Ticks are incremented constantly to make the notes regular.
- In the output of translate back, every header is the same (we tested whether the header changed drastically between midi outputs, which it did not).

Practical detail: for simplicity, we decided not to fix a universal dictionary (the dictionary must be obviously the same for  translate and translateback), but to create it during translating and having it created in translateback to use it (creating the dictionary the same way we did before). The dictionary creation follows an easy principle which is to map a new note to the image of the last seen note + 1. 

### Mechanical for the purpose of training:

To make the training of the model easier, we found a way to be able to translate multiple midi files and then "merge" them to get one sole csv file to give as input of the neural network. We therefore created a `merge.cpp` which merges csv files that were transformed from midi files. 

### Evaluating Translating Accuracy
Once we had our translation algorithms, we decided to formally assess the accuracy of our codes. `testtrans.cpp` takes as input a csv file of a music translated by `midicsv` (not transformed) and a csv file translated both ways by `translate.cpp` and `translateback.cpp`. It compares the number of ordered similar notes and computes the average difference of the time between two consecutive ticks and the average difference in velocity between the original and translated files. We noticed that some notes from the original piece of music were removed during translation, creating some shifts between the two files. Qualitatively, it does not seem to alter the music.

This algorithm was a way to quantify the loss but also to compare composers like Mozart and Bach, and see which one fitted our project the best. We obtained more accuracy with Bach since it was the best compromise between lack of accuracy and good quality of music. Its translation didn't alter its tick interval - and therefore the rhythm - too much.

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

We defined two hidden `LSTM` layers with 512 memory units, and a `Dropout` layer of probability 0,3 which helps avoid overfitting. The output layer uses the `softmax` activation function to output the log of the probability prediction for each notes present in our model.

The problem can be defined as a single integer classification problem with each note being a possible class, therefore training is minimizing the [negative log-likelihood](https://ljvmiranda921.github.io/notebook/2017/08/13/softmax-and-the-negative-log-likelihood/), which maximizes the likelihood that the output of the model produces the data actually observed. We also use the ADAM optimization algorithm for speed.

### Format of our data

In general, LSTM networks expect input data with different features, time step and points. Specifically, in the mlpack library, the LSTM layer takes in an armadillo cube where each row corresponds to a "feature", in our case we only have one feature that is the note, each column corresponds to a "time step" which is the point in time within our sequence of music, and each slice (the third dimension of our tensor) corresponds to a point, or the specific sequence of notes at the time step considered.

The training labels, or what the model is expected to output, is defined as the single note that follows each sequence of notes considered at each time step. Here, it is an armadillo cube again with the same dimensions as the training data, where each column or time step stores the note corresponding to the time step. We use the entirety the music data for training, we do not have a validation set as it does not make sense in the context where we want the model to learn the probabilities of notes given a musical sequence, and there is no "wrong answer" for creativity.

The output of our model is a cube with one row, as many columns as the total number of different notes, and as many slices as the length of the sequence considered at each time step. To extract the notes given a probability vector, we simply look for the index with the maximum probability and choose that as the note predicted by our model.

### Generating music after training

Once we have trained a model, we can save the model with the trained weights for later use. We have chosen to have the model be saved automatically after each 5 cycles of training, to be able to pause and resume training easily while not wasting too much time on saving the models each cycle.

To have the network generate music, we then load the previously trained model. Essentially, the trained network is a prediction model, so it needs a starting point for composition. We use a randomly generated short sequence of notes as the seed sequence which we feed into the prediction method of the model. From then on, we feed the new sequence predicted by the model to obtain the next predicted sequence, and we continue this step until we get a music sequence of our desired length.

<a name="gui"></a>
## 5. Bonus: The GUI 

Since we had some time left at the end of the project, we took on an additional challenge of complementing the project by a GUI where users play a tune on a digital piano to give the model the starting point for its composition. This is clearly an extension and independent from our initial goal. Since this was not part of our original design, our code is not optimized for this use. Though, we thought it could be a fun add-on and pursued it nonetheless. Our GUI is therefore still in its initial stages and has potential errors and bugs. Since we spent time on it and we think it's a creative extension, we will include it as a idea to be perfected later.

The idea is to create a more creative option, designed for the musically inclided users. By running the interface using QT, the user is able to compose a short extract using a digital keyboard, which the model will use as the beginning of the music it composes. To see a preface of the GUI ensure you have Qt creator installed on your machine. From there, open the `piano.pro` file (located in the `pianoGUI` folder) in Qt and configure run the project using the Qt interface. From here, user instructions will be self explanatory. This shows the idea behind the GUI we had in mind though it still does not function perfectly! 

With the help of Qt creator, we tried to make it as user friendly as possible with thorough steps and guidelines on the main window that the user can read while running the program. 

There are 3 main parts of the GUI:

- The virtual piano. 
A 3 octaves virtual piano that we implemented where an accurate sound of each note will be played when pressed.
- A list view.
This list view will help the user to remember the history of all the notes that they pressed. This box will also be added with various actions that the user did (e.g. "start / stop recording" , "training", etc)
- Buttons
These buttons will aid and lead the user to eventually play their own made .mid file


<a name="ext"></a>
## 6. What Next? 

Here are some limits to our project and extension ideas that have not been explored, that will possibly improve our implementation:

### Adding a temperature layer before the logsoftmax layer, to control the randomness of predictions by the LSTM.

Temperature will rescale the logits before putting them through the softmax function, where high temperature will give similar probabilities to each notes and low temperature will give higher probability to the most expected note. We can therefore think of temperature as the parameter of the LSTM's creativity, where a model with low temperature will be more "creative" and give less conservative outpus.

### Single-note training

In our project, each combination of notes from the training set were encoded by a unique integer, which limits the combinations of notes the model can produce to ones it has already seen during training. Another way to encode music would be to give each note a unique character, then encode combinations of notes as combinations of these characters. This encoding is more complex but would be a more realistic modelization of true music composition.

Furthermore, we can add complexity by finding a better way to encode rhythm. Currently, the scope of our training music is limited to music containing notes with lower velocities. This is due to the way we translate MIDI files. In particularly the tick like representation of music and the duration of time that note is help adds complexity to the translating dictionary. 

### Make the trained models less dependent on the training data

This issue is related to the above idea. In our current implementation, one significant contraint is that each trained model can only be used with the specific dictionary map between integers and notes related to its training set. This is why although we have several different trained models, we have only provided one for the user to play around with in the scope of this project. We would like to have a more generalized trained models that would all use the same dictionary mapping, so that with the same translation codes, the user will be able to test several different models.

For completeness, we have still uploaded some of our trained models in [this repository](https://github.com/kayoyin/ProdigyModels), which you can clone with `git lfs` by running 
```
$> git lfs clone https://github.com/kayoyin/ProdigyModels.git
```
Some of the training data used for these models can be found [here](https://github.com/RayDiab/Prodigy/tree/master/training%20data), for users who would like to extract the associated dictionary and use the models.
*note that cloning the entire repository will take up at least 1GB of storage and will take several minutes due to the large file sizes*

### Accuracy measure used to keep track of training

Initially, we implemented a very naive accuracy measure where we simply calculate the percentage of notes the model outputs that coincides with actual notes from the training set. This measure is misleading and not well-suited in the scope of this project, as a model we would consider very good at producing music does not, and should not even have a high accuracy during training. We therefore removed this measure, and judge our models qualitatively. Evaluating the accuracy of the music leads to a fundamental question of how to classify what music is considered as "good". Answering such a question might require developing a criteria based on musical theory.


<a name="team"></a>
## 7. The Team

Project Leader: [Kayo Yin](https://github.com/kayoyin)

Translators team: [Raymond Diab](https://github.com/RayDiab) (Team Leader), [Michael Chan](https://github.com/aka9), [Anaëlle Touré](https://github.com/AnaToure)

Builders team: [Kayo Yin](https://github.com/kayoyin) (Team Leader), [Elzette Janse van Rensburg](https://github.com/elzette-janse), [Nicholas Setijabudiharsa](https://github.com/Nicholas-t)

GUI: [Nicholas Setijabudiharsa](https://github.com/Nicholas-t) (Team Leader), [Junki Hong](https://github.com/pencilpal2)

Data and testing: Harriet Hughes, [Junki Hong](https://github.com/pencilpal2)

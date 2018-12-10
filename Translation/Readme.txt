Here, we work on the translation algorithms.

translate.cpp takes csv files given by the MIDIcsv code (in C - see midicsv by robbie-cao on github) and translates them into a vector of integer values using
a unique bijective map. Each integer coresponds to one and only one note or chord. 
Feel free to add anything to it. 
A problem we would like to discuss is the nom-uniformity of the time difference between consecutive notes played/cancelled. This means that while we sometimes have a difference of two MIDI ticks between two consecutive notes, this difference is sometimes equal to 100 MIDI ticks for some other notes.

Ideally, the we would now need thw backward-translation algorithm to translate it back into MIDI and see if our approximations still conserve quite well the quality of the music.
Note that you need to run the programs on c++ 11. To do so with g++, copy the following line:
g++ -std=c++11 translate.cpp -o translate

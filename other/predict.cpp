//
//  main.cpp
//  Prodigy
//
//  Created by Kayo Yin on 26/11/2018.
//  Copyright © 2018 Kayo Yin. All rights reserved.
//

#include <iostream>
#include <valarray>

int sequences(int notes []){
    // Generate random sequence used to initialize composition by machine
    int sequence_length = 100;
    int network_input [] = {};
    int output [] = {};
    int sequence_in [] = {};
    
    for (int i = 0; i < sizeof(*notes)-sequence_length; i+= 1){
    }
    
    return 0;
}

int [] generate_notes(model, network_input, pitchnames, n_vocab){
    // Generate notes from trained model
    // model: LSTM model with trained weights
    // network_input: input of sequence of notes the model trains on
    // pitchnames: list of all the different notes we have
    // n_vocab: number of different pitches
    
    // pick random sequence of notes as starting point for composition
    int start [] =
}

int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n";
    return 0;
}

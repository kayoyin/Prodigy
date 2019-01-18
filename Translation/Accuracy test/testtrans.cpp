//
//  testtrans.cpp
//
//
//  Created by Anaëlle Touré on 09/12/2018.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <iterator>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <set>
#include <tuple>
#include "translate.hpp"



//The goal of this algorithm is to quantify the precision of our translation codes by looking at ticks, velocity and notes.


int main() {
   
    int count; //counter of similar notes in order
    int c; //new counter for shifted notes

    c = 0;
    count = 0;
    std::vector< int > V; //velocity vector
    std::vector< int > T; //tick difference vector

//The first step is to open both files to get matrices with files' lines in rows and vectors [1, tick, Note_on/Pitch_benc etc, Instrument, Note, Velocity] in lines.

    //For original 
    std::ifstream original("bach.csv"); //insert the name of your original file here
    

    std::vector<std::vector<std::string> > dataList1;
    
    std::string line1 = "";
    std::string del = ",";

    
    while (getline(original, line1))
    {
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line1, boost::is_any_of(del));
        dataList1.push_back(vec);
        
    }
    
    original.close();

            
    //For trans,I don't need to get the big matrix since velocities and ticks are fixed.

//Now I iterate on my matrices to append values to V and T.


    bool check = false;
    std::string prev = "0"; 
    for (int y=0;y<dataList1.size();){

        check = false;

        if (dataList1.at(y).at(2) == " Note_on_c") check = true;

        if (check == false) y++;

        if (check == true){

            if (dataList1.at(y).at(5) == " 0") y++; 
            
            else if (dataList1.at(y).at(5) != " 0"){
                if (dataList1.at(y).at(2) == " Note_on_c"){
    
                T.push_back(abs( abs(std::stoi(dataList1.at(y).at(1)) - std::stoi(prev))-100)); 
                //bc 100 is the tick step we chose for the translated file. Be careful it is not always the case!
                V.push_back(abs(std::stoi(dataList1.at(y).at(5)) - 100));//
                //appends the absolute value of the difference of the velocity between original and trans to the velocity vector
                        
                prev = dataList1.at(y).at(1);   
                  }
//we solve problem to strings by using stoi: it turns them into integers
       
                        y ++;
                        }}}
                                             
                        
    float average1 = accumulate( T.begin(), T.end(), 0)/T.size(); //computes the average of the vector
    float average2 = accumulate( V.begin(), V.end(), 0)/V.size();




//Second step: compare notes. We use the class CSVreader created in translated.hpp to create vector of notes

//For original
    CSVReader original1("bach.csv"); //insert the name of your original file here
    
    std::tuple<std::set<std::string>,std::vector<std::string>> other = original1.getData();
    std::set<std::string> pitches = std::get<0>(other);
    std::vector<std::string> notes = std::get<1>(other);


//For trans
    CSVReader trans1("tbach.csv"); //insert the name of your translated file here
    
    std::tuple<std::set<std::string>,std::vector<std::string>> other1 = trans1.getData();
    std::set<std::string> pitches1 = std::get<0>(other1);
    std::vector<std::string> notes1 = std::get<1>(other1);



///Now compare them. 

//remove silences. 

    for (int i=0; i<notes.size();i++) {
        if ((notes[i]) == "()") {
        notes.erase(notes.begin()+i);
        }
    }

    for (int j=0; j<notes1.size();j++) {
        if ((notes1[j]) == "()") {
            notes1.erase(notes1.begin()+j);
        }
    }



//Since they don't have the same size, we iterate over the smallest one.

    int size; //smallest
    int sizE; //biggest
    if (notes.size() < notes1.size()) {
        size = notes.size();
        sizE = notes1.size();
    }
    else {
        size = notes1.size();
        sizE = notes.size();
    }




//enter this loop if notes is the biggest vector
std::string S;
if (sizE == notes.size()) {
     S = "original";
    int j; //counts the shifts
    j = 0;
    bool checkN = false;
    std::string prevN = notes1[0];

    for (int i=0;i<size;){
        
        checkN = false;
        
        if (notes[i+j] == prevN) checkN = true;
        
        if( checkN == true) {
            count += 1;
            prevN = notes1[i+1];   
            i++;


            
        }
        
       if (checkN == false){
            
            
            for(int j; j<sizE-i;){
                if (notes[i+j] == prevN) {
                    c += 1;
                    prevN = notes1[i+1];
                    i++;
                    checkN = true;
                    break;

                }
                //            std::cout<<"ok";     
                else if (notes[i+j] != prevN) j++;
        
            

            }
            }}}
            

//enter this loop if notes1 is the biggest vector


if (sizE == notes1.size()) {

    S = "trans";
    int j;
    j = 0;
    bool checkN = false;
    std::string prevN = notes[0];

    for (int i=0;i<size;){
        
        checkN = false;
        
        if (notes1[i+j] == prevN) checkN = true;
        
        if( checkN == true) {
            count += 1;
            prevN = notes[i+1];
            i++;

            
        }
        
       if (checkN == false){
            
            
            for(int j; j<sizE-i;){
                if (notes1[i+j] == prevN) {
                    c += 1;
                    prevN = notes[i+1];
                    i++;
                    checkN = true;
                    break;

                }
                
                else if (notes1[i+j] != prevN) j++;
        
            

            }
            }}}
            

    
    
    
//Print the results.
    std::cout << count + c <<" similar notes out of " << size << std::endl;
    std::cout << "However, there are " << sizE - size << " additional notes in " <<  S<< std::endl;
    std::cout << (sizE - size)*100/size << " % of shifts." << std::endl;

    std::cout << "On average, the velocity of notes in original and trans differs by " << average2 << std::endl;
    std::cout <<"On average, the time between two consecutive ticks in original and in trans differs by " << average1 << std::endl;

    return 0;

}

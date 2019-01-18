
//  translate.cpp
//  
// Raymond Diab and Michael Chan
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <tuple>
#include <map>
#include "translate.hpp"

class CSVWriter
{
    std::tuple<std::map<std::string, int>,std::vector<int>> tup;
    std::map<std::string, int> trans;
    std::vector<int> translated;
    std::map<int,std::string> reverse;

public:
    CSVWriter(std::tuple<std::map<std::string, int>,std::vector<int>> tup, std::string delm = ","){
    trans = std::get<0>(tup);
    translated = std::get<1>(tup);
    }
    
    // Function to write data from a CSV File
/*
 * Parses through text file line by line and rewrite on csv line by line
 */
void writeData()
{

    
    // the different files that we open
    std::ofstream newfile("otranslateb.csv");
    std::ifstream header("header.csv");

    // declaring variable
    std::string dummy;
    std::string number;
    std::set<int> keep;
    std::set<int> cur;
    bool check = false;
    bool ok = true;
    std::string line = "";

    for (std::map<std::string, int>::iterator i = trans.begin(); i != trans.end(); ++i){
        reverse[i->second] = i->first;
    }


    // write header 
    while (getline(header, line))
    {
        newfile << line << std::endl;
    }



    // For input of csv
    //writing main 
    int tick = 1325;

    for (std::vector<int>::iterator it = translated.begin() ; it != translated.end(); ++it){
        dummy = reverse[*it];
        tick = tick + 70;
        for (std::string::iterator bit = dummy.begin() ; bit < dummy.end() ; ){
            while (*bit == '0' || *bit == '1' || *bit == '2' || *bit == '3' || *bit == '4' || *bit == '5' || *bit == '6'|| *bit == '7' || *bit == '8' || *bit == '9' ) {
                number = number + *bit; 
                bit++ ;
            }

            cur.insert(atoi(number.c_str()));
            bit++;
            int n = atoi(number.c_str());
            for (std::set<int>::iterator nb = keep.begin(); nb != keep.end(); nb++){
                if (*nb == n) ok = false;
            }
            if (number != "" && ok == true) newfile << "2" << ", " << tick << ", " <<  "Note_on_c" << ", " << "0" << ", " << number << ", " << "100" << std::endl;
            
            ok = true;

            keep.insert(atoi(number.c_str()));
            number = "";


            
            }

        for (std::set<int>::iterator nb = keep.begin(); nb != keep.end(); nb++){
                for (std::set<int>::iterator cb = cur.begin(); cb!= cur.end(); cb++){
                    if (*nb == *cb) check = true;
                }
                if (check == false)  {
                    newfile << "2" << ", " << tick << ", " <<  "Note_on_c" << ", " << "0" << ", " << *nb << ", " << "0" << std::endl;
                    keep.erase(*nb);
                }
                check = false;
        }
        cur.clear();

    }

    // write footer
    tick = tick+1;
    newfile << "2" <<"," <<  tick  <<"," <<  "End_track"<<std::endl;
    newfile << "0" <<"," << "0" << "," << "End_of_file";

    header.close();
    newfile.close();

    


}
};



int main()
{
    // Creating an object of CSVWriter
    CSVReader reader("test.csv");

    // Get the data from CSV File

    std::tuple<std::set<std::string>,std::vector<std::string>> other = reader.getData();
    std::set<std::string> pitches = std::get<0>(other);
    std::vector<std::string> notes = std::get<1>(other);
    std::map<std::string, int> trans = bijection(pitches);
    std::vector<int> translated = translation(notes, trans);
    
    CSVReader part("sonata.csv");
    std::vector<int> partition = part.read_input();
    std::tuple<std::map<std::string, int>,std::vector<int>> tup = std::make_tuple(trans,partition);
    
    // Creating an object of CSVWriter
    CSVWriter writer(tup);
    // outputting the csv file named, "blank.csv"
    writer.writeData();
    
    return 0;
    
}

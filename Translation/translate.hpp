//
//  translate.hpp
//  
//
// By Raymond Diab and Michael Chan
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <set>
#include <tuple>
#include <map>
#include <sstream>

class CSVReader
{
    std::string fileName;
    std::string delimeter;
    
public:
    CSVReader(std::string filename, std::string delm = ",") :
    fileName(filename), delimeter(delm)
    { }
    
    // Function to fetch data from a CSV File
   // std::vector<std::string> getData();

/*
 * Parses through csv file line by line and returns the data
 */
std::tuple<std::set<std::string>,std::vector<std::string>>  getData()
{
    std::ifstream file(fileName);
    
    std::vector<std::vector<std::string> > dataList;
    
    std::string line = "";
    // Iterate through each line and split the content using delimeter
    
    while (getline(file, line))
    {
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
        dataList.push_back(vec);    
        
    }
    // Close the File
    file.close();


    // Diff
    std::ofstream newfile("translated.txt");

    std::set<std::string> pitches;
    std::vector<std::string> notes;

    std::set<std::string> memo;

    std::string prev = "0";
    std::string word = "";

    std::vector<std::string>::iterator pend;
    bool check = false;


    for (int y=0;y<dataList.size();){

        if (dataList.at(y).at(2) == " Note_on_c" || dataList.at(y).at(2) == " Note_off_c" ){
            check = true;
        }

        if (check == false) y++;


        if (check == true){

            if (dataList.at(y).at(5) == " 0" || dataList.at(y).at(2) == " Note_off_c"){

                memo.erase(dataList.at(y).at(4));
                y++;
            }

            else if (dataList.at(y).at(5) != " 0" && dataList.at(y).at(2) != " Note_off_c"){
                while (dataList.at(y).at(1) == prev){
                   if(dataList.at(y).at(5) != " 0" && dataList.at(y).at(2) != " Note_off_c" ) memo.insert(dataList.at(y).at(4));
                    else if (dataList.at(y).at(2) == " Note_off_c") memo.erase(dataList.at(y).at(4));
                    else memo.erase(dataList.at(y).at(4));
                    y++;
                }

                prev = dataList.at(y).at(1);
           
                word = "(";
                for (std::set<std::string>::iterator it = memo.begin() ; it != memo.end(); ++it){
                    if (word == "("){
                        word = word + *it;
                    }
                    else word = word + *it;
                }

                word = word + ")";
                notes.push_back(word);
                pitches.insert(word);
                word = ""; 
            }

        }
        check = false;
    }


    return std::make_tuple(pitches,notes);
}

int sciToint(const std::string& str) {
   std::stringstream ss(str);
   double d = 0;
   ss >> d;
   return (int) d;
}

//creates the list of integers from the new partition
std::vector<int> read_input(){
    std::ifstream file(fileName);
    std::vector<std::vector<std::string> > dataList;
    
    std::string line = "";
    // Iterate through each line and split the content using delimeter
    
    while (getline(file, line))
    {
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
        dataList.push_back(vec);    
        
    }
    // Close the File
    file.close();
    std::vector<int> partition;
    int dummy;
    for (std::vector<std::string>::iterator it = dataList.at(0).begin() ; it != dataList.at(0).end(); ++it){
	    dummy = sciToint(*it);
        partition.push_back(dummy);
    }
    return partition;
}

};

std::map<std::string, int> bijection (std::set<std::string> &myset) { // Returns the associated translation map
    std::map<std::string, int> trans; // initialize translation map
    int c = 0;
    for(std::set<std::string>::iterator it=myset.begin(); it!=myset.end(); ++it){
            if(trans[*it] == 0){
                c++;
                trans[*it] = c; //update translation map
            } 
    }
    return trans;
}

std::vector<int> translation (std::vector<std::string> &notes , std::map<std::string, int> &trans){
    std::vector<int> translated;
    for (std::vector<std::string>::iterator it = notes.begin() ; it != notes.end(); ++it){
        translated.push_back(trans[*it]);
    }
    return translated;
}


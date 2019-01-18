//merge.cpp
//Raymond Diab and Michael Chan

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <vector>



    


void  merging(std::string fileName)
{
    std::ifstream file(fileName.c_str());
    std::ofstream output("test.csv",std::ios_base::app);
    bool check;

    std::vector<std::vector<std::string> > dataList;
    std::string line = "";
    std::string delm = ",";
    
    while (getline(file, line))
    {
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line, boost::is_any_of(delm));
        dataList.push_back(vec);    
    }

    for (int y=0;y<dataList.size();y++){

        if (dataList.at(y).at(2) == " Note_on_c" || dataList.at(y).at(2) == " Note_off_c") {
            output << dataList.at(y).at(0) << "," << dataList.at(y).at(1) << "," << dataList.at(y).at(2) << "," << dataList.at(y).at(3) << "," << dataList.at(y).at(4) << "," << dataList.at(y).at(5) << std::endl;
        }


    }

    // Close the File
    file.close();
    output.close();
}

int main(int argc, char* file[]){
    std::string str(file[1]);
    std::cout << file[1];
    merging(str);
}



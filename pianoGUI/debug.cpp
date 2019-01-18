#include <fstream>
#include <QVector>

#include <QDir>

int main(){
QVector<int> input;
std::ofstream outFile;   //outFile is csv of csvAudio of AI composed music
outFile.open(QDir::currentPath().toStdString()+"/sonata.csv", std::ios::out);
std::ifstream endFile(QDir::currentPath().toStdString()+"/notes.csv");  //endFile is csv of integer from AI
std::string c;
if (endFile.is_open()){
    while (getline(endFile,c, ',')){
                input.push_back(std::stoi(c));
    }
}       //ADDING CONTENTS IN ENDNOTES.csv FROM AI TO QVECTOR INPUT




//TO ADD AI Algorithm and make music .mid
//----------------------------------------------------------------------
outFile <<"0,0, Header,1,20,240,,,,,,,,\n"<<
         "1,0, Start_track,,,,,,,,,,,\n"<<
         "1,0, Tempo,491803,,,,,,,,,,\n"<<
         "1,0, Time_signature,4,2,24,8,,,,,,,\n"<<
         "1,0, Text_t,Mozart Sonate a moll K.V.310 (1st.mov.)  Play by Katsuhiro Oguri,,,,,,,,,,\n"<<
         "1,0, End_track,,,,,,,,,,,\n"<<
         "2,0, Start_track,,,,,,,,,,,\n"<<
         "2,0, Time_signature,4,2,24,8,,,,,,,\n"<<
         "2,0, Title_t,PIANO                               ,,,,,,,,,,\n"<<
         "2,0, System_exclusive,10,65,16,66,18,64,0,127,0,65,247\n"<<
         "2,0, System_exclusive,10,65,16,66,18,64,0,4,100,88,247\n"<<
         "2,170, System_exclusive,10,65,16,66,18,64,1,48,3,12,247\n"<<
         "2,170, System_exclusive,10,65,16,66,18,64,1,51,80,60,247\n"<<
         "2,170, System_exclusive,10,65,16,66,18,64,1,52,60,79,247\n"<<
         "2,372, Control_c,0,0,0,,,,,,,,\n"<<
         "2,374, Control_c,0,32,1,,,,,,,,\n"<<
         "2,376, Program_c,0,0,,,,,,,,,\n"<<
         "2,378, Control_c,0,7,100,,,,,,,,\n"<<
         "2,474, Control_c,0,11,100,,,,,,,,\n"<<
         "2,570, Control_c,0,10,64,,,,,,,,\n"<<
         "2,666, Control_c,0,1,0,,,,,,,,\n"<<
         "2,666, Control_c,0,91,80,,,,,,,,\n"<<
         "2,675, Control_c,0,99,1,,,,,,,,\n"<<
         "2,677, Control_c,0,98,32,,,,,,,,\n"<<
         "2,679, Control_c,0,6,62,,,,,,,,\n"<<
         "2,681, Control_c,0,99,1,,,,,,,,\n"<<
         "2,683, Control_c,0,98,33,,,,,,,,\n"<<
         "2,685, Control_c,0,6,54,,,,,,,,\n"<<
         "2,687, Control_c,0,99,1,,,,,,,,\n"<<
         "2,689, Control_c,0,98,99,,,,,,,,\n"<<
         "2,691, Control_c,0,6,62,,,,,,,,\n"<<
         "2,693, Control_c,0,99,1,,,,,,,,\n"<<
         "2,695, Control_c,0,98,100,,,,,,,,\n"<<
         "2,697, Control_c,0,6,60,,,,,,,,\n"<<
         "2,699, Control_c,0,99,1,,,,,,,,\n"<<
         "2,701, Control_c,0,98,102,,,,,,,,\n"<<
         "2,703, Control_c,0,6,70,,,,,,,,\n"<<
         "2,1300, Control_c,0,64,63,,,,,,,,\n"<<
         "2,1305, Control_c,0,64,127,,,,,,,,\n"; //appending header.csv
//---------------------------------------------------------------------------------------------
int j = 0;

for (int i=0; i<input.size();i++){
    j+=5;
    outFile << "2,"<< j << ",note_on_c,1," << input.at(i)<<",100\n";
    j+=5;
    outFile << "2,"<< j << ",note_on_c,1," << input.at(i)<<",0\n";

}

outFile <<"2, "<< j <<",End_track\n";
outFile <<"2,0,End_of_file\n";

endFile.close();
outFile.close();
}

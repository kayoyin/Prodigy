#include "piano.h"
#include "ui_piano.h"
#include <vector>
#include <fstream>
#include <iostream>

#include <QVector>
#include <QStringList>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <string>
#include <QPushButton>
#include <QSound>
#include <map>
#include <QVectorIterator>
#include <cstdio>
#include <QMediaPlayer>
#include <QDesktopServices>
#include <QUrl>
#include <../Translation/translate.hpp>

bool rec = 0;

QStringList lis;
bool csv1=0;
bool csv2=0;

piano::piano(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::piano)
{
    ui->setupUi(this);
    ui->Db->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Eb->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Gb->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Ab->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Bb->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->C->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->D->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->E->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->F->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->G->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->A->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->B->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->Db_2->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Eb_2->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Gb_2->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Ab_2->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Bb_2->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->C_2->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->D_2->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->E_2->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->F_2->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->G_2->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->A_2->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->B_2->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->Db_3->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Eb_3->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Gb_3->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Ab_3->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->Bb_3->setStyleSheet("QPushButton {background-color: rgb(64,64,64); color:white}");
    ui->C_3->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->D_3->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->E_3->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->F_3->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->G_3->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->A_3->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->B_3->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
    ui->C_4->setStyleSheet("QPushButton {background-color: rgb(255,255,255)}");
}

piano::~piano()
{
    delete ui;
}

void piano::on_C_clicked()
{
    ui->listWidget->addItem("C");
    if (rec == 1){
        lis<<"C";
    }
    QSound::play(":/new/prefix1/c3.wav");
}


void piano::on_Db_clicked()
{
    ui->listWidget->addItem("Db");
    if (rec == 1){
        lis<<"Db";
    }
    QSound::play(":/new/prefix1/db3.wav");
}

void piano::on_D_clicked()
{
    ui->listWidget->addItem("D");
    if (rec == 1){
        lis<<"D";
    }
    QSound::play(":new/prefix1/d3.wav");
}

void piano::on_Eb_clicked()
{
    ui->listWidget->addItem("Eb");
    if (rec == 1){
        lis<<"Eb";
    }
    QSound::play(":new/prefix1/eb3.wav");
}

void piano::on_E_clicked()
{
    ui->listWidget->addItem("E");
    if (rec == 1){
        lis<<"E";
    }
    QSound::play(":new/prefix1/e3.wav");
}

void piano::on_F_clicked()
{
    ui->listWidget->addItem("F");
    if (rec == 1){
        lis<<"F";
    }
    QSound::play(":new/prefix1/f3.wav");
}

void piano::on_Gb_clicked()
{
    ui->listWidget->addItem("Gb");
    if (rec == 1){
        lis<<"Gb";
    }
    QSound::play(":new/prefix1/gb3.wav");
}

void piano::on_G_clicked()
{
    ui->listWidget->addItem("G");
    if (rec == 1){
        lis<<"G";
    }
    QSound::play(":new/prefix1/g3.wav");
}

void piano::on_Ab_clicked()
{
    ui->listWidget->addItem("Ab");
    if (rec == 1){
        lis<<"Ab";
    }
    QSound::play(":new/prefix1/ab3.wav");
}

void piano::on_A_clicked()
{
    ui->listWidget->addItem("A");
    if (rec == 1){
        lis<<"A";
    }
    QSound::play(":new/prefix1/a3.wav");
}

void piano::on_Bb_clicked()
{
    ui->listWidget->addItem("Bb");
    if (rec == 1){
        lis<<"Bb";
    }
    QSound::play(":new/prefix1/bb3.wav");
}

void piano::on_B_clicked()
{
    ui->listWidget->addItem("B");
    if (rec == 1){
        lis<<"B";
    }
    QSound::play(":new/prefix1/b3.wav");
}

void piano::on_C_2_clicked()
{
    ui->listWidget->addItem("C_2");
    if (rec == 1){
        lis<<"C_2";
    }
    QSound::play(":new/prefix1/c4.wav");
}



void piano::on_Db_2_clicked()
{
    ui->listWidget->addItem("Db_2");
    if (rec == 1){
        lis<<"Db_2";
    }
    QSound::play(":/new/prefix1/db4.wav");
}

void piano::on_D_2_clicked()
{
    ui->listWidget->addItem("D_2");
    if (rec == 1){
        lis<<"D_2";
    }
    QSound::play(":new/prefix1/d4.wav");
}

void piano::on_Eb_2_clicked()
{
    ui->listWidget->addItem("Eb_2");
    if (rec == 1){
        lis<<"Eb_2";
    }
    QSound::play(":new/prefix1/eb4.wav");
}

void piano::on_E_2_clicked()
{
    ui->listWidget->addItem("E_2");
    if (rec == 1){
        lis<<"E_2";
    }
    QSound::play(":new/prefix1/e4.wav");
}

void piano::on_F_2_clicked()
{
    ui->listWidget->addItem("F_2");
    if (rec == 1){
        lis<<"F_2";
    }
    QSound::play(":new/prefix1/f4.wav");
}

void piano::on_Gb_2_clicked()
{
    ui->listWidget->addItem("Gb_2");
    if (rec == 1){
        lis<<"Gb_2";
    }
    QSound::play(":new/prefix1/gb4.wav");
}

void piano::on_G_2_clicked()
{
    ui->listWidget->addItem("G_2");
    if (rec == 1){
        lis<<"G_2";
    }
    QSound::play(":new/prefix1/g4.wav");
}

void piano::on_Ab_2_clicked()
{
    ui->listWidget->addItem("Ab_2");
    if (rec == 1){
        lis<<"Ab_2";
    }
    QSound::play(":new/prefix1/ab4.wav");
}

void piano::on_A_2_clicked()
{
    ui->listWidget->addItem("A_2");
    if (rec == 1){
        lis<<"A_2";
    }
    QSound::play(":new/prefix1/a4.wav");
}

void piano::on_Bb_2_clicked()
{
    ui->listWidget->addItem("Bb_2");
    if (rec == 1){
        lis<<"Bb_2";
    }
    QSound::play(":new/prefix1/bb4.wav");
}

void piano::on_B_2_clicked()
{
    ui->listWidget->addItem("B_2");
    if (rec == 1){
        lis<<"B_2";
    }
    QSound::play(":new/prefix1/b4.wav");
}

void piano::on_C_3_clicked()
{
    ui->listWidget->addItem("C_3");
    if (rec == 1){
        lis<<"C_3";
    }
    QSound::play(":new/prefix1/c5.wav");
}



void piano::on_Db_3_clicked()
{
    ui->listWidget->addItem("Db_3");
    if (rec == 1){
        lis<<"Db_3";
    }
    QSound::play(":/new/prefix1/db5.wav");
}

void piano::on_D_3_clicked()
{
    ui->listWidget->addItem("D_3");
    if (rec == 1){
        lis<<"D_3";
    }
    QSound::play(":new/prefix1/d5.wav");
}

void piano::on_Eb_3_clicked()
{
    ui->listWidget->addItem("Eb_3");
    if (rec == 1){
        lis<<"Eb_3";
    }
    QSound::play(":new/prefix1/eb5.wav");
}

void piano::on_E_3_clicked()
{
    ui->listWidget->addItem("E_3");
    if (rec == 1){
        lis<<"E_3";
    }
    QSound::play(":new/prefix1/e5.wav");
}

void piano::on_F_3_clicked()
{
    ui->listWidget->addItem("F_3");
    if (rec == 1){
        lis<<"F_3";
    }
    QSound::play(":new/prefix1/f5.wav");
}

void piano::on_Gb_3_clicked()
{
    ui->listWidget->addItem("Gb_3");
    if (rec == 1){
        lis<<"Gb_3";
    }
    QSound::play(":new/prefix1/gb5.wav");
}

void piano::on_G_3_clicked()
{
    ui->listWidget->addItem("G_3");
    if (rec == 1){
        lis<<"G_3";
    }
    QSound::play(":new/prefix1/g5.wav");
}

void piano::on_Ab_3_clicked()
{
    ui->listWidget->addItem("Ab_3");
    if (rec == 1){
        lis<<"Ab_3";
    }
    QSound::play(":new/prefix1/ab5.wav");
}

void piano::on_A_3_clicked()
{
    ui->listWidget->addItem("A_3");
    if (rec == 1){
        lis<<"A_3";
    }
    QSound::play(":new/prefix1/a5.wav");
}

void piano::on_Bb_3_clicked()
{
    ui->listWidget->addItem("Bb_3");
    if (rec == 1){
        lis<<"Bb_3";
    }
    QSound::play(":new/prefix1/bb5.wav");
}

void piano::on_B_3_clicked()
{
    ui->listWidget->addItem("B");
    if (rec == 1){
        lis<<"B";
    }
    QSound::play(":new/prefix1/b5.wav");
}

void piano::on_C_4_clicked()
{
    ui->listWidget->addItem("C_2");
    if (rec == 1){
        lis<<"C_2";
    }
    QSound::play(":new/prefix1/c6.wav");
}

void piano::on_commandLinkButton_clicked()
{
    rec = 1;
    ui->listWidget->addItem("start recording");
}

void piano::on_commandLinkButton_2_clicked()
{
    rec = 0;
    ui->listWidget->addItem("stop recording");
    QMessageBox msgBox;
    msgBox.setWindowTitle("save");
    msgBox.setText("Do you want to save this set of notes?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if(msgBox.exec() == QMessageBox::Yes){
        ui->listWidget->addItem("Saved");
    }
        else {
        ui->listWidget->addItem("Not saved");
        lis.clear();
    }
 }


void piano::on_commandLinkButton_3_clicked()
{
    //ui->listWidget->addItems(list);
    csv1 = 1;
    std::ofstream myFile;
    myFile.open(QDir::currentPath().toStdString()+"/midicsv-csvmidi/chords.csv", std::ios::out);
    QListIterator<QString> i(lis);
    for(int i = 0; i < lis.size(); i++) {
             myFile << lis.at(i).toStdString()<<"," ;
             ui->listWidget->addItem(lis.at(i));
    }
    myFile.close();
    if(lis.empty()){
        QMessageBox::warning(this, tr("Warning"),tr("nothing typed"));
    }
    else {
            ui->listWidget->addItem("csv file created");
    }
}

void piano::on_commandLinkButton_5_clicked()
{
    lis.clear();
    ui->listWidget->addItem("recorded list is cleared");
}



void piano::on_commandLinkButton_4_clicked() //train
{
    csv2 = 1;
    if(lis.empty()|csv1==0){
        QMessageBox::warning(this, tr("Warning"),tr("nothing typed"));
    }
    else {
        ui->listWidget->addItem("composing.......");

        std::ofstream numFile;  //numFile is csv of user input


        numFile.open(QDir::currentPath().toStdString()+"/startnotes.csv", std::ios::out);

        std::map<std::string, int> mapOfWords;
        //C:48, Db:49, D:50 ...
        mapOfWords.insert(std::make_pair("C",48));
        mapOfWords.insert(std::make_pair("Db",49));
        mapOfWords.insert(std::make_pair("D",50));
        mapOfWords.insert(std::make_pair("Eb",51));
        mapOfWords.insert(std::make_pair("E",52));
        mapOfWords.insert(std::make_pair("F",53));
        mapOfWords.insert(std::make_pair("Gb",54));
        mapOfWords.insert(std::make_pair("G",55));
        mapOfWords.insert(std::make_pair("Ab",56));
        mapOfWords.insert(std::make_pair("A",57));
        mapOfWords.insert(std::make_pair("Bb",58));
        mapOfWords.insert(std::make_pair("B",59));
        mapOfWords.insert(std::make_pair("C_2",60));
        mapOfWords.insert(std::make_pair("Db_2",61));
        mapOfWords.insert(std::make_pair("D_2",62));
        mapOfWords.insert(std::make_pair("Eb_2",63));
        mapOfWords.insert(std::make_pair("E_2",64));
        mapOfWords.insert(std::make_pair("F_2",65));
        mapOfWords.insert(std::make_pair("Gb_2",66));
        mapOfWords.insert(std::make_pair("G_2",67));
        mapOfWords.insert(std::make_pair("Ab_2",68));
        mapOfWords.insert(std::make_pair("A_2",69));
        mapOfWords.insert(std::make_pair("Bb_2",70));
        mapOfWords.insert(std::make_pair("B_2",71));
        mapOfWords.insert(std::make_pair("C_3",72));
        mapOfWords.insert(std::make_pair("Db_3",73));
        mapOfWords.insert(std::make_pair("D_3",74));
        mapOfWords.insert(std::make_pair("Eb_3",75));
        mapOfWords.insert(std::make_pair("E_3",76));
        mapOfWords.insert(std::make_pair("F_3",77));
        mapOfWords.insert(std::make_pair("Gb_3",78));
        mapOfWords.insert(std::make_pair("G_3",79));
        mapOfWords.insert(std::make_pair("Ab_3",80));
        mapOfWords.insert(std::make_pair("A_3",81));
        mapOfWords.insert(std::make_pair("Bb_3",82));
        mapOfWords.insert(std::make_pair("B_3",83));
        mapOfWords.insert(std::make_pair("C_4",84));

        std::vector<int> out;
        std::vector<int> numlis;
        for (int i=0;i<lis.size();i++){
            std::map<std::string, int>::iterator it;
            for ( it = mapOfWords.begin(); it != mapOfWords.end(); it++ )
            {
                if (it->first == lis.at(i).toStdString()){
                    numlis.push_back(it->second);
                }
                else{}
            }

        }


        CSVReader reader("../Translation/test.csv");

        std::tuple<std::set<std::string>, std::vector<std::string>> other = reader.getData();
        std::set<std::string> pitches = std::get<0>(other);
        std::vector<std::string> notes = std::get<1>(other);
        std::map<std::string, int> trans = bijection(pitches);



        QString dum="( ";
        QString dum2 = ")";
        std::vector<std::string> noteslist;

        for (int i=0; i< numlis.size(); i++){
            std::string h=(dum+QString::number(numlis.at(i))+dum2).toStdString();
           noteslist.push_back(h);
           ui->listWidget->addItem(QString::fromStdString(h));
        }
        for (int i=0; i< noteslist.size(); i++) {// iterate on listnotes
            std::map<std::string, int>::iterator it;

            for (it = trans.begin(); it != trans.end(); ++it )  //iterate on trans bijection
            {
                //ui->listWidget->addItem(QString::fromStdString(it->first));
                //ui->listWidget->addItem(QString::number(it->second));
                if (noteslist[i]== it->first){
                    out.push_back(it->second);
                    ui->listWidget->addItem(" ------------------------------------- ");
                    ui->listWidget->addItem(QString::fromStdString(it->first));
                    ui->listWidget->addItem(" was translated to ");
                    ui->listWidget->addItem(QString::number(it->second));
                    ui->listWidget->addItem(" ------------------------------------- ");
                }
                else{
                    //ut.push_back(1);

                }
            }
        }


        for(int i=0; i < out.size(); i++) {
            int c;
            numFile << out[i] <<",";
            //ui->listWidget->addItem(QString::number(out[i]));
          }

            numFile<<"\n";
            numFile.close();

            ui->listWidget->addItem("csv file created");
            //
            //
            //

            std::string command="mv startnotes.csv ../utils/startnotes.csv";
            system(command.c_str());




            QMessageBox msgBox;
            msgBox.setWindowTitle("play");


            msgBox.setText("SURPRISE! We are not done yet, we like our users to be more hands on programmer, so now run command prompt in Prodigy/build and run \n \"./composegui\" this will call our AI Buddy BOB to start helping you compose your first music");
            QAbstractButton* pButtonYes = msgBox.addButton(tr("Ok!"), QMessageBox::YesRole);
            msgBox.addButton(tr("Whatever"), QMessageBox::NoRole);
            msgBox.exec();

            if (msgBox.clickedButton()==pButtonYes) {

            }
            else{
                lis.clear();
                }
        }
}



void piano::on_commandLinkButton_6_clicked()
{



    //std::string command="cd .. && ./backscript";
    //system(command.c_str());

    /*
     *
     std::vector<int> input;
     std::vector<int> out;
     std::ofstream outFile;   //outFile is csv of csvAudio of AI composed music
     outFile.open("sonata.csv", std::ios::out);
     //std::string command="mv ../utils/notes.csv notes.csv";
     std::ifstream endFile("notes.csv");  //endFile is csv of integer from AI
     std::string c;
     if (endFile.is_open()){
         while (getline(endFile,c, ',')){
                     input.push_back(std::atoi(c.c_str()));
         }
     }       //ADDING CONTENTS IN ENDNOTES.csv FROM AI TO QVECTOR INPUT
     ui->listWidget->addItem("composed csv is :");
     QStringList test;
     foreach(int i, input){
         test << QString::number(i);
     }
     ui->listWidget->addItems(test);
    //asdafsdgsgd
     //abfsdfius uif
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
               "2, 0, Program_c, 1, 19"<<
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
     int j = 10;
     for (int i=0; i<out.size();i++){
         j+=5;
         outFile << "2,"<< j << ",note_on_c,1," << out[i]<<",100\n";
         j+=5;
         outFile << "2,"<< j << ",note_off_c,1," << out[i]<<",0\n";
     }
     outFile <<"2, "<< j <<",End_track\n";
     outFile <<"0,0,End_of_file\n";
     outFile.close();
     endFile.close();
    std::string command3="mv sonata.csv ../midicsv-csvmidi/sonata.csv";
    system(command3.c_str());
    std::string command2="cd ../midicsv-csvmidi/ &&  ./csvmidi sonata.csv sonata.mid";
    system(command2.c_str());
    std::string command4="mv ../midicsv-csvmidi/sonata.mid ../sonata.mid";
    system(command4.c_str());
    */
    QMessageBox msgBox;
    msgBox.setWindowTitle("play");


    msgBox.setText("YOU ARE ALMOST THERE! Now in the run command prompt from Prodigy folder and run \" ./translatescript \" then \" ./backscript \" ");
    QAbstractButton* pButtonYes = msgBox.addButton(tr("Ok!"), QMessageBox::YesRole);
    msgBox.addButton(tr("Whatever"), QMessageBox::NoRole);
    msgBox.exec();

    if (msgBox.clickedButton()==pButtonYes) {
        QMessageBox msgBox2;
        msgBox2.setWindowTitle("play");


        msgBox2.setText("So now your .mid file is in the main folder ! CONGRATULATIONS! You just collaborated with AI for music! now play your mid file with the button below! if you dont have a midi player installed, you can convert it online to mp3 in this site \"https://www.onlineconverter.com/midi-to-mp3 \"");
        QAbstractButton* pButtonYes2 = msgBox2.addButton(tr("OK :)"), QMessageBox::YesRole);
        msgBox2.addButton(tr("I'll Pass :("), QMessageBox::NoRole);
        msgBox2.exec();
    }
    else{
        lis.clear();
        }
}

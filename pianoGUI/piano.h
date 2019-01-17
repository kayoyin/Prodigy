#ifndef PIANO_H
#define PIANO_H

#include <fstream>
#include <QMainWindow>
#include <QStringList>
#include <QDir>


namespace Ui {
class piano;
}

class piano : public QMainWindow
{
    Q_OBJECT

public:
    explicit piano(QWidget *parent = nullptr);
    ~piano();

private slots:
    void on_C_clicked();

    void on_Db_clicked();

    void on_D_clicked();

    void on_Eb_clicked();

    void on_E_clicked();

    void on_F_clicked();

    void on_Gb_clicked();

    void on_G_clicked();

    void on_Ab_clicked();

    void on_A_clicked();

    void on_Bb_clicked();

    void on_B_clicked();

    void on_C_2_clicked();

    void on_Db_2_clicked();

    void on_D_2_clicked();

    void on_Eb_2_clicked();

    void on_E_2_clicked();

    void on_F_2_clicked();

    void on_Gb_2_clicked();

    void on_G_2_clicked();

    void on_Ab_2_clicked();

    void on_A_2_clicked();

    void on_Bb_2_clicked();

    void on_B_2_clicked();

    void on_C_3_clicked();

    void on_Db_3_clicked();

    void on_D_3_clicked();

    void on_Eb_3_clicked();

    void on_E_3_clicked();

    void on_F_3_clicked();

    void on_Gb_3_clicked();

    void on_G_3_clicked();

    void on_Ab_3_clicked();

    void on_A_3_clicked();

    void on_Bb_3_clicked();

    void on_B_3_clicked();

    void on_C_4_clicked();

    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();

    void on_commandLinkButton_3_clicked();

    void on_commandLinkButton_4_clicked();

    void on_commandLinkButton_5_clicked();


    void on_commandLinkButton_6_clicked();

private:
    Ui::piano *ui;
};

#endif // PIANO_H

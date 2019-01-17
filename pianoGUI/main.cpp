#include "piano.h"
#include <QApplication>
#include<QSplashScreen>
#include<QTimer>
#include <QDesktopWidget>
#include <QWidget>


int main(int argc, char *argv[])
{
    int WIDTH = 900;
    int HEIGHT = 1000;
    int screenWidth;
    int screenHeight;
    int x, y;

    QApplication app(argc, argv);
    piano w;
    QDesktopWidget *desktop = QApplication::desktop();

    screenWidth = desktop->width();
    screenHeight = desktop->height();

    x = (screenWidth - WIDTH) / 2;
    y = (screenHeight - HEIGHT) / 2;

    w.resize(WIDTH, HEIGHT);
    w.move( x, y );

    QApplication a(argc, argv);

    QSplashScreen *splash=new QSplashScreen;
    splash->setPixmap(QPixmap(":new/prefix1/picture.png"));
    splash->show();

    //piano w;
    QTimer::singleShot(2000,splash,SLOT(close()));
    QTimer::singleShot(2000,&w,SLOT(show()));
    //w.show();

    return a.exec();
}

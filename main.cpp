#include "Connexion_Bd/connexion_bd.h"
#include <QApplication>
#include "mainwindow.h"
//#include "Gestion_Personnel/gestion_personnel.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //Gestion_Personnel p;
    connexion_bd c;

    if(c.createconnect()){
        w.show();
    }
    else{
        cout<<"error";
    }

    return a.exec();
}

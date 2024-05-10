#include "dons.h"

dons::dons()
{
    destination = "";
    date = QDate();
    montant = 0.0;
}

void dons::setdestination(QString n){
    destination = n;
}

void dons::setdate(const QDate n){
    date = n;
}

void dons::setmontant(double n){
    montant = n;
}

QString dons::getdestination(){
    return destination;
}

QDate dons::getdate(){
    return date;
}

double dons::getmontant(){
    return montant;
}



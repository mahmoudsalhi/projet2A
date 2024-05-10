#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <iostream>
#include <QString>
#include <QSqlQueryModel>
#include <QDate>
#include <QDebug>
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include "Gestion_Personnel/gestion_personnel.h"



using namespace std;
class employee
{


public:
    employee();
    employee(int,QString,QString,QDate,QString,QString,int,QString,QString,QString,float,QString,QDate);
    ~employee();

    static QSqlQueryModel*  afficher_Employees();
    employee afficher_Employee(int id);
    bool Ajouter_Employee();
    bool Modifier_Employee(QTableWidgetItem *,QString,int);
    bool Effacer_Employee(int id);
    static bool Effacer_Employee_CIN(int CIN);
    bool Existance_Employee(QString);
    bool Existance_Employee_CIN(QString);
    bool Existance_Employee_ID(int id);

    QString getNom(){return this->Nom;}
    QString getPrenom(){return this->Prenom;}
    QString getEmail(){return this->Email;}
    QString getAdresse(){return this->Adresse;}
    QString getSexe(){return this->Sexe;}
    QString getMdp(){return this->Mdp;}
    QDate getAge(){return this->Age;}
    QDate getDateEmb(){return this->DateEmb;}
    int getNumTel(){return this->NumTel;}
    QString getCIN(){return this->CIN;}
    float getSalaire(){return this->Salaire;}
    QString getRole(){return this->Role;}

    void setNom(QString Nom){this->Nom=Nom;}
    void setPrenom(QString Prenom){this->Prenom=Prenom;}
    void setEmail(QString Email){this->Email=Email;}
    void setAdresse(QString Adresse){this->Adresse=Adresse;}
    void setSexe(QString Sexe){this->Sexe=Sexe;}
    void setMdp(QString Mdp){this->Mdp=Mdp;}
    void setAge(QDate Age){this->Age=Age;}
    void setDateEmb(QDate DateEmb){this->DateEmb=DateEmb;}
    void setNumTel(int NumTel){this->NumTel=NumTel;}
    void setCIN(QString CIN){this->CIN=CIN;}
    void setSalaire(float Salaire){this->Salaire=Salaire;}
    void setRole(QString Role){this->Role=Role;}

private:
    QString Nom,Prenom,Email,Adresse,Sexe,Mdp,Role,CIN;
    int Id_Emp,NumTel;
    QDate Age,DateEmb;
    float Salaire;
};


#endif // EMPLOYEE_H

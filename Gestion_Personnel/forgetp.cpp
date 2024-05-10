#include "forgetp.h"
#include "gestion_personnel.h"
#include "ui_forgetp.h"
#include <QProcess>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <QDebug>
using namespace std;
forgetP::forgetP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::forgetP)
{
    ui->setupUi(this);
}

forgetP::~forgetP()
{
    delete ui;
}
void forgetP::on_confirm_clicked(){
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    QString email= ui->email->text();

    query.prepare("SELECT email,mdp FROM employee WHERE email = :email;");
    query.bindValue(":email", email);
    model->setQuery(query);
    if (query.exec()) {


        if (query.next()) {
            QStringList arguments;
            QString Code=generateVerificationCode();
            arguments << "C:/Users/azizz/Downloads/ProjetC++/PythonScripts/oublie.py" <<email<<Code;

            QProcess::startDetached("python", arguments);

            QMessageBox::information(this, "System", "mail envoye avec succes.");
            ui->email->setText("");
            ui->confirm->setEnabled(false);
            ui->confirm->lower();
            ui->confirmP->lower();
            ui->label_3->setText("Verification code: ");
            ui->label_25->setText("");
            ui->email->setPlaceholderText("Verification code");
            ui->confirmC->setEnabled(true);
            ui->confirmC->raise();
            connect(ui->confirmC, &QPushButton::clicked, this, [=]() {
                confirmCode(Code,email);
            });


        } else {
            QMessageBox::critical(this, "Erreur", "Veuillez vérifier votre adresse e-mail.");
        }
    }

}

QString forgetP::generateVerificationCode() {
    const QString possibleCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const int length = 9;
    QString verificationCode;

    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(nullptr)));

    for (int i = 0; i < length; ++i) {
        int index = rand() % possibleCharacters.length();
        verificationCode.append(possibleCharacters.at(index));
    }

    return verificationCode;
}


bool forgetP::confirmP(QString email){
    QSqlQuery query;
    QString mdp=ui->email->text();
    query.prepare("UPDATE employee SET mdp=:mdp WHERE email=:email;");
    query.bindValue(":email",email);
    query.bindValue(":mdp",mdp);
    return query.exec();
}
void forgetP::confirmCode(QString code,QString email){
    //ui->email->setText(code);
    if(code==ui->email->text()){
        ui->email->setText("");
        ui->confirmC->setEnabled(false);
        ui->label_3->setText("Nouveau mot de passe: ");
        ui->label_25->setText("");
        ui->email->setPlaceholderText("Nouveau mot de passe");
        ui->confirmP->setEnabled(true);
        ui->confirmP->raise();

        connect(ui->confirmP, &QPushButton::clicked, this, [=]() {
            Gestion_Personnel w;
            if(w.valideMdp(ui->email->text())){
                if(confirmP(email)){
                    QMessageBox::information(this, "System", "Mot de passe modifie avec succes.");
                    this->close();
                }
            }
            else{
                QMessageBox::critical(this, "System", "Mot de passe est faible.");
            }

        });
    }
    else{
        QMessageBox::critical(this, "System", "Code Incorrect.");
    }

}


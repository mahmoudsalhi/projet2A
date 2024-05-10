#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//BEGIN LOGIN
void MainWindow::on_login_clicked() {
    QString email = ui->email->text();
    QString mdp = ui->mdp->text();
    QSqlQuery query;
    query.prepare("SELECT role FROM employee WHERE mdp = :mdp AND email = :email;");
    query.bindValue(":email", email);
    query.bindValue(":mdp", mdp);

    if (query.exec()) {
        if (query.next()) {
            if(query.value(0).toString()=="Gestion_Personnels"){
                Menu* w = new Menu();
                w->show();
                this->close();
            }
            else if (query.value(0).toString()=="Gestion_Participants") {
                gestion_participant* w = new gestion_participant();
                w->show();
                this->close();
            }
            else if (query.value(0).toString()=="Gestion_Oeuvres") {
                gestion_oeuvre* w = new gestion_oeuvre();
                w->show();
                this->close();
            }
            else if (query.value(0).toString()=="Gestion_Charites") {
                gestion_charite* w = new gestion_charite();
                w->show();
                this->close();
            }
            else if (query.value(0).toString()=="Gestion_Evenements") {
                gestion_evenement* w = new gestion_evenement();
                w->show();
                this->close();
            }
            else if (query.value(0).toString()=="Employee") {
                gestion_Achat* w = new gestion_Achat();
                w->show();
                this->close();
            }
        } else {
            QMessageBox::critical(this, "Erreur", "Veuillez vérifier votre adresse e-mail et votre mot de passe.");
        }
    }
}
//END LOGIN

//BEGIN FACE_SCAN
void MainWindow::on_FACE_SCAN_clicked() {

    QProcess process;
    A.write_to_arduino("AAAAAAActiver la camera");
    QString pythonCommand = "python";
    QStringList scriptArgs;
    scriptArgs << "C:/Users/azizz/Downloads/ProjetC++/PythonScripts/face/main.py";


    process.start(pythonCommand, scriptArgs);

    process.waitForFinished(-1);
    QString role = process.readAllStandardOutput();
    if(role.contains("hahaha")){
        QTextToSpeech *tts = new QTextToSpeech(this);
        tts->say(role);
    }
    if(role.contains("Gestion_Personnels")){
        Menu* w = new Menu();
        w->show();
        this->close();
        A.write_to_arduino("visage trouve");
    }
    else if (role.contains("Gestion_Participants")) {
        gestion_participant* w = new gestion_participant();
        w->show();
        this->close();
        A.write_to_arduino("vvvvvvvisage trouve");
    }
    else if (role.contains("Gestion_Oeuvres")) {
        gestion_oeuvre* w = new gestion_oeuvre();
        w->show();
        this->close();
        A.write_to_arduino("visage trouve");
    }
    else if (role.contains("Gestion_Charites")) {
        gestion_charite* w = new gestion_charite();
        w->show();
        this->close();
        A.write_to_arduino("visage trouve");
    }
    else if (role.contains("Gestion_Evenements")) {
        gestion_evenement* w = new gestion_evenement();
        w->show();
        this->close();
        A.write_to_arduino("visage trouve");
    }
    else if (role.contains("Employee")) {
        gestion_Achat* w = new gestion_Achat();
        w->show();
        this->close();
        A.write_to_arduino("visage trouve");
    }
    else if (role.contains("Unkown")){
        QMessageBox::critical(this, "System", "visage non reconnu!!");
    }
}
//END FACE_SCAN

//END FORGET
void MainWindow::on_oublier_clicked(){
    forgetP* w = new forgetP();
    w->show();
    this->close();
}
//END FORGET

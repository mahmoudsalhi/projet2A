#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QMessageBox>
#include "Menu/menu.h"
#include "Gestion_Charite/gestion_charite.h"
#include "Gestion_Evenement/gestion_evenement.h"
#include "Gestion_Oeuvre/gestion_oeuvre.h"
#include "Gestion_Participants/gestion_participant.h"
#include "Gestion_Personnel/gestion_personnel.h"
#include "Gestion_Personnel/forgetp.h"
#include "His_Achat/gestion_Achat.h"
#include "Arduino/arduino.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Arduino A;

private slots:
    void on_login_clicked();
    void on_oublier_clicked();
    void on_FACE_SCAN_clicked();
};
#endif // MAINWINDOW_H

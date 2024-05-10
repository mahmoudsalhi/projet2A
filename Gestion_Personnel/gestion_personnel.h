#ifndef GESTION_PERSONNEL_H
#define GESTION_PERSONNEL_H

#include <QDialog>
#include <QSqlQueryModel>
#include "../mainwindow.h"
#include "employee.h"
#include "Gestion_Personnel/tasks.h"
#include "Gestion_Personnel/statistics.h"
#include "Menu/menu.h"
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QTextToSpeech>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QProcess>


namespace Ui {
class Gestion_Personnel;
}

class Gestion_Personnel : public QDialog
{
    Q_OBJECT

public:
    explicit Gestion_Personnel(QWidget *parent = nullptr);
    ~Gestion_Personnel();
    void filterTable(const QString &text);
    void detectAfficher(int code);
    void afficherPersonnels(QSqlQueryModel* m);
    void ajouterPersonnel();
    void DeleteEmployee();
    bool valideEmail(QString email);
    bool valideCIN(QString CIN);
    bool valideTEL(QString tel);
    bool valideSalaire(QString salaire);
    bool valideMdp(QString password);
    bool valideDateNaiss(QDate dateNaiss);
    bool valideDate(QDate DateEmb);
    void showError(QLineEdit* EditLine);
    void hideError(QLineEdit* EditLine);
    void downloadPDF();
    void statistic();
    void statisticOFF();
    void assistance();
    void tasks();
    void scan_CIN();
    void scan_Face();
    void logout();

private:
    Ui::Gestion_Personnel *ui;

};

#endif // GESTION_PERSONNEL_H

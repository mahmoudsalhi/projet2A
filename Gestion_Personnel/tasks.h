#ifndef TASKS_H
#define TASKS_H

#include <QDialog>
#include <QTextToSpeech>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include "employee.h"
#include "Gestion_Personnel/gestion_personnel.h"

namespace Ui {
class Tasks;
}

class Tasks : public QDialog
{
    Q_OBJECT

public:
    explicit Tasks(QWidget *parent = nullptr);
    ~Tasks();
    void assistance();

private:
    Ui::Tasks *ui;
};

#endif // TASKS_H

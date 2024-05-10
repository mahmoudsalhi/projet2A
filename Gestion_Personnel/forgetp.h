#ifndef FORGETP_H
#define FORGETP_H

#include <QDialog>

namespace Ui {
class forgetP;
}

class forgetP : public QDialog
{
    Q_OBJECT

public:
    explicit forgetP(QWidget *parent = nullptr);
    ~forgetP();
    QString generateVerificationCode();
    void confirmCode(QString code,QString email);
    bool confirmP(QString email);

private:
    Ui::forgetP *ui;
private slots:
    void on_confirm_clicked();


};

#endif // FORGETP_H

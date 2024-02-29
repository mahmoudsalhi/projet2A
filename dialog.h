#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "Produit.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    void setProduit(Produit p);
    ~Dialog();

private:
    Ui::Dialog *ui1;
};

#endif // DIALOG_H

#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui1(new Ui::Dialog)
{
    ui1->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui1;
}


void Dialog::setProduit(Produit p){
    ui1->lineEdit_Produit_name->setText(p.get_produit());
    double textPrice = p.get_total_price();
    QString Price = QString::number(textPrice);
    ui1->lineEdit_price->setText(Price);
}

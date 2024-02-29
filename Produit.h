#ifndef PRODUIT_H
#define PRODUIT_H
#include<QString>
class Produit
{
public:
    void setprice(double n);
    void setquantity(int n);
    void setproduit(QString n);
    void settotal_price(double n, int y);
    void setpimageroduit(QString n);

    double get_total_price();
    double get_price();
    int get_quantity();
    QString get_produit();
    QString get_imageproduit();
    Produit();

private:
    double price, total_price;
    int quantity;
    QString produit, image_produit;
};

#endif // PRODUIT_H

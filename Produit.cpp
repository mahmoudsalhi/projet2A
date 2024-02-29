#include "Produit.h"

Produit::Produit() {
    price = 0.0;
    produit = "";
    quantity = 0;
    image_produit = "";
}

void Produit::setquantity(int n){
    quantity = n;
}
void Produit::setproduit(QString n){
    produit = n;
}
void Produit::setprice(double n){
    price = n;
}
void Produit::settotal_price(double n, int y){
    total_price = n*y;
}
void Produit::setpimageroduit(QString n){
    image_produit = n;
}


double Produit::get_total_price(){
    return total_price;
}
double Produit::get_price(){
    return price;
}
QString Produit::get_produit(){
    return produit;
}
int Produit::get_quantity(){
    return quantity;
}
QString Produit::get_imageproduit(){
    return image_produit;
}

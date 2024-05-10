#ifndef DONS_H
#define DONS_H
#include<QString>
#include<QDate>
class dons
{
    public:
        void setdestination(QString n);
        void setmontant(double n);
        void setdate(const QDate date);

        QString getdestination();
        QDate getdate();
        double getmontant();
        dons();
    private :
        QString destination;
        QDate date;
        double montant;
};

#endif // DONS_H

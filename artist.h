#ifndef ARTIST_H
#define ARTIST_H
#include<QString>


class Artist
{
public:
    void setnom(QString n);
    void setadr(QString n);
    void setNumTel(int n);
    QString getnom();
    QString getadr();
    int getNumTel();

    Artist();
   private:
    QString nom,adr;
    int NumTel;
};

#endif // ARTIST_H


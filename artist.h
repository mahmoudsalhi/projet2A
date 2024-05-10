#ifndef ARTIST_H
#define ARTIST_H
#include<QString>


class Artist
{
public:
    void setnom(QString n);
    void setadr(QString n);
    void setNumTel(QString n);
    QString getnom();
    QString getadr();
    QString getNumTel();

    Artist();
   private:
    QString nom,adr,NumTel;

};

#endif // ARTIST_H


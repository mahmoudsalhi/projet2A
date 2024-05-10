#ifndef ARTIST_H
#define ARTIST_H
#include<QString>


class Artist
{
public:
    void setnom(QString n);
    void setadr(QString n);
    void setNumTel(QString n);
    void setcodepin(QString n);
    QString getnom();
    QString getadr();
    QString getNumTel();
    QString getcodepin();
    Artist();
   private:
    QString nom,adr,NumTel,codepin;

};

#endif // ARTIST_H


#include "artist.h"

Artist::Artist()
{


        nom="";
        adr="";
        NumTel="";

 }


    void Artist :: setnom(QString n){nom=n;}
    void Artist :: setadr(QString n){adr=n;}
    void Artist :: setNumTel(QString n){NumTel=n;}

    QString Artist :: getnom(){return nom;}
    QString Artist :: getadr(){return adr;}
    QString Artist :: getNumTel(){return NumTel;}


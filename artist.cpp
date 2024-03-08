#include "artist.h"

Artist::Artist()
{


        nom="";
        adr="";
        NumTel=0;

 }


    void Artist :: setnom(QString n){nom=n;}
    void Artist :: setadr(QString n){adr=n;}
    void Artist :: setNumTel(int n){NumTel=n;}

    QString Artist :: getnom(){return nom;}
    QString Artist :: getadr(){return adr;}
    int Artist :: getNumTel(){return NumTel;}


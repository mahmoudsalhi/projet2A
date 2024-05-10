#include "artist.h"

Artist::Artist()
{


        nom="";
        adr="";
        NumTel="";
        codepin="";

 }


    void Artist :: setnom(QString n){nom=n;}
    void Artist :: setadr(QString n){adr=n;}
    void Artist :: setNumTel(QString n){NumTel=n;}
    void Artist :: setcodepin(QString n){codepin=n;}
    QString Artist :: getnom(){return nom;}
    QString Artist :: getadr(){return adr;}
    QString Artist :: getNumTel(){return NumTel;}
    QString Artist :: getcodepin(){return codepin;}


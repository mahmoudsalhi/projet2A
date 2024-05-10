/********************************************************************************
** Form generated from reading UI file 'forgetp.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORGETP_H
#define UI_FORGETP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_forgetP
{
public:
    QPushButton *confirm;
    QLineEdit *email;
    QLabel *label_3;
    QLabel *label;
    QPushButton *confirmC;
    QLabel *label_25;
    QPushButton *confirmP;
    QLabel *label_2;

    void setupUi(QDialog *forgetP)
    {
        if (forgetP->objectName().isEmpty())
            forgetP->setObjectName(QStringLiteral("forgetP"));
        forgetP->resize(695, 343);
        confirm = new QPushButton(forgetP);
        confirm->setObjectName(QStringLiteral("confirm"));
        confirm->setGeometry(QRect(210, 200, 161, 34));
        confirm->setStyleSheet(QLatin1String("QPushButton {\n"
"    padding: 5px;\n"
"    font-size: 16px;\n"
"    text-align: center;\n"
"    text-decoration: none;\n"
"    border: 2px solid #792382;\n"
"    color: #792382;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #43aff7;\n"
"}\n"
"\n"
"\n"
""));
        email = new QLineEdit(forgetP);
        email->setObjectName(QStringLiteral("email"));
        email->setGeometry(QRect(310, 130, 271, 33));
        email->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    padding: 5px;\n"
"    border: 2px solid #792382;\n"
"    border-radius: 5px;\n"
"    font-size: 14px;\n"
"    color: #333;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border-color: #2ecc71; /* Couleur de la bordure lorsqu'il est activ\303\251 */\n"
"}\n"
""));
        label_3 = new QLabel(forgetP);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 130, 271, 33));
        label_3->setStyleSheet(QLatin1String("QLabel {\n"
"    font-size: 22px;\n"
"    color: #792382; /* Couleur du texte */\n"
"    font-weight: bold; /* Gras */\n"
"    /* Ajoutez d'autres styles selon vos besoins */\n"
"}\n"
""));
        label = new QLabel(forgetP);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(-380, -10, 1081, 611));
        label->setStyleSheet(QLatin1String("background-image: url(\"images/back.jpg\");\n"
""));
        confirmC = new QPushButton(forgetP);
        confirmC->setObjectName(QStringLiteral("confirmC"));
        confirmC->setEnabled(false);
        confirmC->setGeometry(QRect(210, 200, 161, 34));
        confirmC->setStyleSheet(QLatin1String("QPushButton {\n"
"    padding: 5px;\n"
"    font-size: 16px;\n"
"    text-align: center;\n"
"    text-decoration: none;\n"
"    border: 2px solid #792382;\n"
"    color: #792382;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #43aff7;\n"
"}\n"
"\n"
"\n"
""));
        label_25 = new QLabel(forgetP);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(210, 20, 261, 33));
        label_25->setStyleSheet(QLatin1String("QLabel {\n"
"    font-size: 22px;\n"
"    color: #792382; /* Couleur du texte */\n"
"    font-weight: bold; /* Gras */\n"
"    /* Ajoutez d'autres styles selon vos besoins */\n"
"}\n"
""));
        confirmP = new QPushButton(forgetP);
        confirmP->setObjectName(QStringLiteral("confirmP"));
        confirmP->setEnabled(false);
        confirmP->setGeometry(QRect(210, 200, 161, 34));
        confirmP->setStyleSheet(QLatin1String("QPushButton {\n"
"    padding: 5px;\n"
"    font-size: 16px;\n"
"    text-align: center;\n"
"    text-decoration: none;\n"
"    border: 2px solid #792382;\n"
"    color: #792382;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #43aff7;\n"
"}\n"
"\n"
"\n"
""));
        label_2 = new QLabel(forgetP);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(560, 0, 121, 91));
        label_2->setStyleSheet(QLatin1String("\n"
"    background-image: url(\"D:/ESPRIT/c++/Projet/untitled/images/logo.png\");"));
        label_2->setPixmap(QPixmap(QString::fromUtf8("../../../Documents/interface \303\240 bechir/interface \342\224\234\303\241 bechir/images/logo.png")));
        confirmC->raise();
        confirmP->raise();
        label->raise();
        confirm->raise();
        email->raise();
        label_3->raise();
        label_25->raise();
        label_2->raise();

        retranslateUi(forgetP);

        QMetaObject::connectSlotsByName(forgetP);
    } // setupUi

    void retranslateUi(QDialog *forgetP)
    {
        forgetP->setWindowTitle(QApplication::translate("forgetP", "Dialog", Q_NULLPTR));
        confirm->setText(QApplication::translate("forgetP", "confirmer", Q_NULLPTR));
        email->setPlaceholderText(QApplication::translate("forgetP", "email", Q_NULLPTR));
        label_3->setText(QApplication::translate("forgetP", "Saisir votre mail  ", Q_NULLPTR));
        label->setText(QString());
        confirmC->setText(QApplication::translate("forgetP", "confirmer", Q_NULLPTR));
        label_25->setText(QApplication::translate("forgetP", "Mot de passe oublie?", Q_NULLPTR));
        confirmP->setText(QApplication::translate("forgetP", "confirmer", Q_NULLPTR));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class forgetP: public Ui_forgetP {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORGETP_H

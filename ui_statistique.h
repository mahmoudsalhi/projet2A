/********************************************************************************
** Form generated from reading UI file 'statistique.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATISTIQUE_H
#define UI_STATISTIQUE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Statistique
{
public:
    QGraphicsView *graphicsView;
    QLabel *label_8;
    QPushButton *retour;

    void setupUi(QDialog *Statistique)
    {
        if (Statistique->objectName().isEmpty())
            Statistique->setObjectName(QStringLiteral("Statistique"));
        Statistique->resize(940, 522);
        graphicsView = new QGraphicsView(Statistique);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(-5, 1, 991, 611));
        label_8 = new QLabel(Statistique);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 10, 121, 81));
        label_8->setPixmap(QPixmap(QString::fromUtf8("../../../../PROJET C++/logo sghayer .png")));
        retour = new QPushButton(Statistique);
        retour->setObjectName(QStringLiteral("retour"));
        retour->setGeometry(QRect(850, 40, 81, 34));
        retour->setStyleSheet(QLatin1String("QPushButton {\n"
"    display: inline-block;\n"
"    /*padding: 10px 20px;*/\n"
"    padding: 5px;\n"
"    font-size: 16px;\n"
"    text-align: center;\n"
"    text-decoration: none;\n"
"    cursor: pointer;\n"
"    border: 2px solid #792382;\n"
"    color: #792382;\n"
"    border-radius: 5px;\n"
"    transition: background-color 0.3s, color 0.3s;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #43aff7;\n"
"    color: #ffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    transform: translateY(1px);\n"
"}\n"
""));

        retranslateUi(Statistique);

        QMetaObject::connectSlotsByName(Statistique);
    } // setupUi

    void retranslateUi(QDialog *Statistique)
    {
        Statistique->setWindowTitle(QApplication::translate("Statistique", "Dialog", Q_NULLPTR));
        label_8->setText(QString());
        retour->setText(QApplication::translate("Statistique", "Retour", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Statistique: public Ui_Statistique {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATISTIQUE_H

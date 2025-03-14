/********************************************************************************
** Form generated from reading UI file 'secondwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECONDWINDOW_H
#define UI_SECONDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SecondWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblSecond;

    void setupUi(QWidget *SecondWindow)
    {
        if (SecondWindow->objectName().isEmpty())
            SecondWindow->setObjectName(QString::fromUtf8("SecondWindow"));
        SecondWindow->resize(1024, 600);
        verticalLayout = new QVBoxLayout(SecondWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lblSecond = new QLabel(SecondWindow);
        lblSecond->setObjectName(QString::fromUtf8("lblSecond"));
        lblSecond->setStyleSheet(QString::fromUtf8("font: 87 32pt \"Noto Sans CJK KR\";\n"
"font-weight: 800;"));
        lblSecond->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblSecond);


        retranslateUi(SecondWindow);

        QMetaObject::connectSlotsByName(SecondWindow);
    } // setupUi

    void retranslateUi(QWidget *SecondWindow)
    {
        SecondWindow->setWindowTitle(QCoreApplication::translate("SecondWindow", "Form", nullptr));
        lblSecond->setText(QCoreApplication::translate("SecondWindow", "Waiting for the other user...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SecondWindow: public Ui_SecondWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECONDWINDOW_H

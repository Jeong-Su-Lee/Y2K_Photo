/********************************************************************************
** Form generated from reading UI file 'endingwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENDINGWINDOW_H
#define UI_ENDINGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EndingWindow
{
public:
    QPushButton *btnEnding;

    void setupUi(QWidget *EndingWindow)
    {
        if (EndingWindow->objectName().isEmpty())
            EndingWindow->setObjectName(QString::fromUtf8("EndingWindow"));
        EndingWindow->resize(1024, 600);
        btnEnding = new QPushButton(EndingWindow);
        btnEnding->setObjectName(QString::fromUtf8("btnEnding"));
        btnEnding->setGeometry(QRect(780, 470, 146, 50));
        btnEnding->setStyleSheet(QString::fromUtf8("background-color: #009DFF;\n"
"font: 16pt \"Noto Sans CJK KR\";\n"
"border-radius: 25px;\n"
"color: white;\n"
"border: none;\n"
"outline: none;"));
        btnEnding->setFlat(true);

        retranslateUi(EndingWindow);

        QMetaObject::connectSlotsByName(EndingWindow);
    } // setupUi

    void retranslateUi(QWidget *EndingWindow)
    {
        EndingWindow->setWindowTitle(QCoreApplication::translate("EndingWindow", "Form", nullptr));
        btnEnding->setText(QCoreApplication::translate("EndingWindow", "Exit... 9", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EndingWindow: public Ui_EndingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENDINGWINDOW_H

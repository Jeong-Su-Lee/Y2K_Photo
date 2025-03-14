/********************************************************************************
** Form generated from reading UI file 'firstwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIRSTWINDOW_H
#define UI_FIRSTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FirstWindow
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *btnFirst;

    void setupUi(QWidget *FirstWindow)
    {
        if (FirstWindow->objectName().isEmpty())
            FirstWindow->setObjectName(QString::fromUtf8("FirstWindow"));
        FirstWindow->resize(1024, 600);
        verticalLayout = new QVBoxLayout(FirstWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btnFirst = new QPushButton(FirstWindow);
        btnFirst->setObjectName(QString::fromUtf8("btnFirst"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnFirst->sizePolicy().hasHeightForWidth());
        btnFirst->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QString::fromUtf8("Noto Sans CJK KR"));
        font.setPointSize(32);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(99);
        btnFirst->setFont(font);
        btnFirst->setFocusPolicy(Qt::TabFocus);
        btnFirst->setStyleSheet(QString::fromUtf8("font: 87 32pt \"Noto Sans CJK KR\";\n"
"font-weight: 800;\n"
"outline: none;\n"
"border: none;"));
        btnFirst->setFlat(true);

        verticalLayout->addWidget(btnFirst);


        retranslateUi(FirstWindow);

        btnFirst->setDefault(false);


        QMetaObject::connectSlotsByName(FirstWindow);
    } // setupUi

    void retranslateUi(QWidget *FirstWindow)
    {
        FirstWindow->setWindowTitle(QCoreApplication::translate("FirstWindow", "Form", nullptr));
        btnFirst->setText(QCoreApplication::translate("FirstWindow", "Please Touch the Screen", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FirstWindow: public Ui_FirstWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRSTWINDOW_H

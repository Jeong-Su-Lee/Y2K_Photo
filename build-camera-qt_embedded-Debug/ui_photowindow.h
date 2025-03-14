/********************************************************************************
** Form generated from reading UI file 'photowindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PHOTOWINDOW_H
#define UI_PHOTOWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PhotoWindow
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QFrame *frame_3;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *lblCount;
    QFrame *frame_4;
    QGridLayout *gridLayout_2;
    QLabel *lbl4;
    QLabel *lbl2;
    QLabel *lbl8;
    QLabel *lbl9;
    QLabel *lbl3;
    QLabel *lbl7;
    QLabel *lbl5;
    QLabel *lbl12;
    QLabel *lbl6;
    QLabel *lbl11;
    QLabel *lbl10;
    QLabel *lbl1;
    QLabel *lbl13;
    QLabel *lbl14;
    QLabel *lbl15;
    QLabel *lbl16;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblTime;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblImg1;
    QLabel *lblImage2;

    void setupUi(QWidget *PhotoWindow)
    {
        if (PhotoWindow->objectName().isEmpty())
            PhotoWindow->setObjectName(QString::fromUtf8("PhotoWindow"));
        PhotoWindow->resize(1024, 600);
        verticalLayout = new QVBoxLayout(PhotoWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        frame = new QFrame(PhotoWindow);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setStyleSheet(QString::fromUtf8("border: none;"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(360);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 9);
        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setLayoutDirection(Qt::RightToLeft);
        frame_3->setStyleSheet(QString::fromUtf8("border:none;"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_3);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(frame_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 25 28pt \"Noto Sans CJK KR\";\n"
"color: gray;"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lblCount = new QLabel(frame_3);
        lblCount->setObjectName(QString::fromUtf8("lblCount"));
        lblCount->setStyleSheet(QString::fromUtf8("font: 25 28pt \"Noto Sans CJK KR\";\n"
"color: black;"));
        lblCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lblCount, 0, 1, 1, 1);


        horizontalLayout->addWidget(frame_3);

        frame_4 = new QFrame(frame);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setEnabled(false);
        frame_4->setStyleSheet(QString::fromUtf8("border:1px black;"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_4);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        lbl4 = new QLabel(frame_4);
        lbl4->setObjectName(QString::fromUtf8("lbl4"));
        lbl4->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl4, 0, 3, 1, 1);

        lbl2 = new QLabel(frame_4);
        lbl2->setObjectName(QString::fromUtf8("lbl2"));
        lbl2->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;\n"
"border-bottom: 0;\n"
"background: #AEE1EF;"));

        gridLayout_2->addWidget(lbl2, 0, 1, 1, 1);

        lbl8 = new QLabel(frame_4);
        lbl8->setObjectName(QString::fromUtf8("lbl8"));
        lbl8->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl8, 1, 3, 1, 1);

        lbl9 = new QLabel(frame_4);
        lbl9->setObjectName(QString::fromUtf8("lbl9"));
        lbl9->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl9, 2, 0, 1, 1);

        lbl3 = new QLabel(frame_4);
        lbl3->setObjectName(QString::fromUtf8("lbl3"));
        lbl3->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl3, 0, 2, 1, 1);

        lbl7 = new QLabel(frame_4);
        lbl7->setObjectName(QString::fromUtf8("lbl7"));
        lbl7->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl7, 1, 2, 1, 1);

        lbl5 = new QLabel(frame_4);
        lbl5->setObjectName(QString::fromUtf8("lbl5"));
        lbl5->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl5, 1, 0, 1, 1);

        lbl12 = new QLabel(frame_4);
        lbl12->setObjectName(QString::fromUtf8("lbl12"));
        lbl12->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl12, 2, 3, 1, 1);

        lbl6 = new QLabel(frame_4);
        lbl6->setObjectName(QString::fromUtf8("lbl6"));
        lbl6->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl6, 1, 1, 1, 1);

        lbl11 = new QLabel(frame_4);
        lbl11->setObjectName(QString::fromUtf8("lbl11"));
        lbl11->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl11, 2, 2, 1, 1);

        lbl10 = new QLabel(frame_4);
        lbl10->setObjectName(QString::fromUtf8("lbl10"));
        lbl10->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;\n"
"border-bottom: 0;"));

        gridLayout_2->addWidget(lbl10, 2, 1, 1, 1);

        lbl1 = new QLabel(frame_4);
        lbl1->setObjectName(QString::fromUtf8("lbl1"));
        lbl1->setAutoFillBackground(false);
        lbl1->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;\n"
"border-bottom: 0;\n"
"background: #AEE1EF;"));

        gridLayout_2->addWidget(lbl1, 0, 0, 1, 1);

        lbl13 = new QLabel(frame_4);
        lbl13->setObjectName(QString::fromUtf8("lbl13"));
        lbl13->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;"));

        gridLayout_2->addWidget(lbl13, 3, 0, 1, 1);

        lbl14 = new QLabel(frame_4);
        lbl14->setObjectName(QString::fromUtf8("lbl14"));
        lbl14->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;"));

        gridLayout_2->addWidget(lbl14, 3, 1, 1, 1);

        lbl15 = new QLabel(frame_4);
        lbl15->setObjectName(QString::fromUtf8("lbl15"));
        lbl15->setStyleSheet(QString::fromUtf8("border: 1px solid gray;\n"
"border-right: 0;"));

        gridLayout_2->addWidget(lbl15, 3, 2, 1, 1);

        lbl16 = new QLabel(frame_4);
        lbl16->setObjectName(QString::fromUtf8("lbl16"));
        lbl16->setStyleSheet(QString::fromUtf8("border: 1px solid gray;"));

        gridLayout_2->addWidget(lbl16, 3, 3, 1, 1);


        horizontalLayout->addWidget(frame_4);

        frame_5 = new QFrame(frame);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setStyleSheet(QString::fromUtf8("border:none;"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_5);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lblTime = new QLabel(frame_5);
        lblTime->setObjectName(QString::fromUtf8("lblTime"));
        lblTime->setStyleSheet(QString::fromUtf8("font: 25 28pt \"Noto Sans CJK KR\";\n"
"color: black;\n"
"font-weight: 400;"));
        lblTime->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lblTime);


        horizontalLayout->addWidget(frame_5);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);

        verticalLayout->addWidget(frame);

        frame_2 = new QFrame(PhotoWindow);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setStyleSheet(QString::fromUtf8("border: none;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lblImg1 = new QLabel(frame_2);
        lblImg1->setObjectName(QString::fromUtf8("lblImg1"));

        horizontalLayout_2->addWidget(lblImg1);

        lblImage2 = new QLabel(frame_2);
        lblImage2->setObjectName(QString::fromUtf8("lblImage2"));

        horizontalLayout_2->addWidget(lblImage2);


        verticalLayout->addWidget(frame_2);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 5);

        retranslateUi(PhotoWindow);

        QMetaObject::connectSlotsByName(PhotoWindow);
    } // setupUi

    void retranslateUi(QWidget *PhotoWindow)
    {
        PhotoWindow->setWindowTitle(QCoreApplication::translate("PhotoWindow", "Form", nullptr));
        label->setText(QCoreApplication::translate("PhotoWindow", "/8", nullptr));
        lblCount->setText(QCoreApplication::translate("PhotoWindow", "1", nullptr));
        lbl4->setText(QString());
        lbl2->setText(QString());
        lbl8->setText(QString());
        lbl9->setText(QString());
        lbl3->setText(QString());
        lbl7->setText(QString());
        lbl5->setText(QString());
        lbl12->setText(QString());
        lbl6->setText(QString());
        lbl11->setText(QString());
        lbl10->setText(QString());
        lbl1->setText(QString());
        lbl13->setText(QString());
        lbl14->setText(QString());
        lbl15->setText(QString());
        lbl16->setText(QString());
        lblTime->setText(QCoreApplication::translate("PhotoWindow", "59", nullptr));
        lblImg1->setText(QString());
        lblImage2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PhotoWindow: public Ui_PhotoWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHOTOWINDOW_H

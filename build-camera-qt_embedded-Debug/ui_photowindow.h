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
        horizontalLayout->setSpacing(40);
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
        frame_4->setStyleSheet(QString::fromUtf8("border:none;"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);

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
        horizontalLayout->setStretch(1, 6);
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

/********************************************************************************
** Form generated from reading UI file 'guideselectwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIDESELECTWINDOW_H
#define UI_GUIDESELECTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GuideSelectWindow
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblGuide;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *toolButton_4;

    void setupUi(QWidget *GuideSelectWindow)
    {
        if (GuideSelectWindow->objectName().isEmpty())
            GuideSelectWindow->setObjectName(QString::fromUtf8("GuideSelectWindow"));
        GuideSelectWindow->resize(1024, 600);
        verticalLayout = new QVBoxLayout(GuideSelectWindow);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        frame_2 = new QFrame(GuideSelectWindow);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setStyleSheet(QString::fromUtf8("outline: none; border: none;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lblGuide = new QLabel(frame_2);
        lblGuide->setObjectName(QString::fromUtf8("lblGuide"));
        lblGuide->setStyleSheet(QString::fromUtf8("font: 87 24pt \"Noto Sans CJK KR\";\n"
"font-weight: 800;"));
        lblGuide->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lblGuide);


        verticalLayout->addWidget(frame_2);

        frame = new QFrame(GuideSelectWindow);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setEnabled(true);
        frame->setStyleSheet(QString::fromUtf8("outline: none; border: none;"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(32);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(60, 44, 60, 120);
        toolButton = new QToolButton(frame);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(toolButton->sizePolicy().hasHeightForWidth());
        toolButton->setSizePolicy(sizePolicy);
        toolButton->setStyleSheet(QString::fromUtf8("border-radius: 32px;\n"
"background-color: white;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/none.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(100, 100));

        horizontalLayout->addWidget(toolButton);

        toolButton_2 = new QToolButton(frame);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        sizePolicy.setHeightForWidth(toolButton_2->sizePolicy().hasHeightForWidth());
        toolButton_2->setSizePolicy(sizePolicy);
        toolButton_2->setStyleSheet(QString::fromUtf8("border-radius: 32px;\n"
"background-color: white;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/heart.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_2->setIcon(icon1);
        toolButton_2->setIconSize(QSize(100, 100));

        horizontalLayout->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(frame);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));
        sizePolicy.setHeightForWidth(toolButton_3->sizePolicy().hasHeightForWidth());
        toolButton_3->setSizePolicy(sizePolicy);
        toolButton_3->setStyleSheet(QString::fromUtf8("border-radius: 32px;\n"
"background-color: white;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/star.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_3->setIcon(icon2);
        toolButton_3->setIconSize(QSize(100, 100));

        horizontalLayout->addWidget(toolButton_3);

        toolButton_4 = new QToolButton(frame);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));
        sizePolicy.setHeightForWidth(toolButton_4->sizePolicy().hasHeightForWidth());
        toolButton_4->setSizePolicy(sizePolicy);
        toolButton_4->setStyleSheet(QString::fromUtf8("border-radius: 32px;\n"
"background-color: white;"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/lg.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_4->setIcon(icon3);
        toolButton_4->setIconSize(QSize(100, 100));

        horizontalLayout->addWidget(toolButton_4);


        verticalLayout->addWidget(frame);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 3);

        retranslateUi(GuideSelectWindow);

        QMetaObject::connectSlotsByName(GuideSelectWindow);
    } // setupUi

    void retranslateUi(QWidget *GuideSelectWindow)
    {
        GuideSelectWindow->setWindowTitle(QCoreApplication::translate("GuideSelectWindow", "Form", nullptr));
        lblGuide->setText(QCoreApplication::translate("GuideSelectWindow", "Choose a guildline shape", nullptr));
        toolButton->setText(QCoreApplication::translate("GuideSelectWindow", "None", nullptr));
        toolButton_2->setText(QCoreApplication::translate("GuideSelectWindow", "Heart", nullptr));
        toolButton_3->setText(QCoreApplication::translate("GuideSelectWindow", "Star", nullptr));
        toolButton_4->setText(QCoreApplication::translate("GuideSelectWindow", "LG logo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GuideSelectWindow: public Ui_GuideSelectWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIDESELECTWINDOW_H

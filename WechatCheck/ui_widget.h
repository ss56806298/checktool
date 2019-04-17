/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextBrowser *logBrowser;
    QPushButton *startButton;
    QLineEdit *checkTimerEdit;
    QLabel *checkTimerLabel;
    QPushButton *targetStartButton;
    QLabel *label;
    QLineEdit *gameHallIdEdit;
    QLabel *label_2;
    QLineEdit *secondEdit;
    QPushButton *modifyStartButton;
    QLabel *label_3;
    QLineEdit *modifyHallEdit;
    QLabel *label_4;
    QLineEdit *secondEdit2;
    QLabel *modifyDelayLabel;
    QLabel *allCheckDelayLable;
    QLabel *targetCheckDelayLable;
    QPushButton *tableBCheckButton;
    QLabel *label_5;
    QLineEdit *tableBCheckEdit;
    QLabel *tableBCheckDelayLabel;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(416, 468);
        logBrowser = new QTextBrowser(Widget);
        logBrowser->setObjectName(QStringLiteral("logBrowser"));
        logBrowser->setGeometry(QRect(10, 140, 391, 321));
        startButton = new QPushButton(Widget);
        startButton->setObjectName(QStringLiteral("startButton"));
        startButton->setGeometry(QRect(30, 20, 75, 23));
        checkTimerEdit = new QLineEdit(Widget);
        checkTimerEdit->setObjectName(QStringLiteral("checkTimerEdit"));
        checkTimerEdit->setGeometry(QRect(270, 20, 31, 20));
        checkTimerLabel = new QLabel(Widget);
        checkTimerLabel->setObjectName(QStringLiteral("checkTimerLabel"));
        checkTimerLabel->setGeometry(QRect(130, 20, 131, 20));
        targetStartButton = new QPushButton(Widget);
        targetStartButton->setObjectName(QStringLiteral("targetStartButton"));
        targetStartButton->setGeometry(QRect(30, 50, 75, 23));
        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(110, 50, 61, 16));
        gameHallIdEdit = new QLineEdit(Widget);
        gameHallIdEdit->setObjectName(QStringLiteral("gameHallIdEdit"));
        gameHallIdEdit->setGeometry(QRect(180, 50, 21, 20));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(210, 50, 81, 16));
        secondEdit = new QLineEdit(Widget);
        secondEdit->setObjectName(QStringLiteral("secondEdit"));
        secondEdit->setGeometry(QRect(290, 50, 51, 20));
        modifyStartButton = new QPushButton(Widget);
        modifyStartButton->setObjectName(QStringLiteral("modifyStartButton"));
        modifyStartButton->setGeometry(QRect(30, 80, 75, 23));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(110, 80, 61, 16));
        modifyHallEdit = new QLineEdit(Widget);
        modifyHallEdit->setObjectName(QStringLiteral("modifyHallEdit"));
        modifyHallEdit->setGeometry(QRect(180, 80, 21, 20));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(210, 80, 81, 16));
        secondEdit2 = new QLineEdit(Widget);
        secondEdit2->setObjectName(QStringLiteral("secondEdit2"));
        secondEdit2->setGeometry(QRect(290, 80, 51, 20));
        modifyDelayLabel = new QLabel(Widget);
        modifyDelayLabel->setObjectName(QStringLiteral("modifyDelayLabel"));
        modifyDelayLabel->setGeometry(QRect(350, 80, 41, 20));
        allCheckDelayLable = new QLabel(Widget);
        allCheckDelayLable->setObjectName(QStringLiteral("allCheckDelayLable"));
        allCheckDelayLable->setGeometry(QRect(350, 20, 41, 20));
        targetCheckDelayLable = new QLabel(Widget);
        targetCheckDelayLable->setObjectName(QStringLiteral("targetCheckDelayLable"));
        targetCheckDelayLable->setGeometry(QRect(350, 50, 41, 20));
        tableBCheckButton = new QPushButton(Widget);
        tableBCheckButton->setObjectName(QStringLiteral("tableBCheckButton"));
        tableBCheckButton->setGeometry(QRect(30, 110, 75, 23));
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(110, 110, 101, 16));
        tableBCheckEdit = new QLineEdit(Widget);
        tableBCheckEdit->setObjectName(QStringLiteral("tableBCheckEdit"));
        tableBCheckEdit->setGeometry(QRect(220, 110, 51, 20));
        tableBCheckDelayLabel = new QLabel(Widget);
        tableBCheckDelayLabel->setObjectName(QStringLiteral("tableBCheckDelayLabel"));
        tableBCheckDelayLabel->setGeometry(QRect(350, 110, 21, 16));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        startButton->setText(QApplication::translate("Widget", "\346\243\200\346\265\213\345\220\257\345\212\250", Q_NULLPTR));
        checkTimerEdit->setText(QApplication::translate("Widget", "20", Q_NULLPTR));
        checkTimerLabel->setText(QApplication::translate("Widget", "\350\267\235\347\246\273\344\270\213\346\254\241\346\243\200\346\237\245\347\232\204\346\227\266\351\227\264(\347\247\222)", Q_NULLPTR));
        targetStartButton->setText(QApplication::translate("Widget", "\345\256\232\346\227\266\346\243\200\346\237\245", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "GameHallId", Q_NULLPTR));
        gameHallIdEdit->setText(QApplication::translate("Widget", "1", Q_NULLPTR));
        label_2->setText(QApplication::translate("Widget", "\345\256\232\346\227\266\346\227\266\351\227\264(\347\247\222)", Q_NULLPTR));
        secondEdit->setText(QApplication::translate("Widget", "120", Q_NULLPTR));
        modifyStartButton->setText(QApplication::translate("Widget", "\345\256\232\346\227\266\346\233\264\346\215\242", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", "GameHallId", Q_NULLPTR));
        modifyHallEdit->setText(QApplication::translate("Widget", "1", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "\345\256\232\346\227\266\346\227\266\351\227\264(\347\247\222)", Q_NULLPTR));
        secondEdit2->setText(QApplication::translate("Widget", "120", Q_NULLPTR));
        modifyDelayLabel->setText(QApplication::translate("Widget", "1", Q_NULLPTR));
        allCheckDelayLable->setText(QApplication::translate("Widget", "1", Q_NULLPTR));
        targetCheckDelayLable->setText(QApplication::translate("Widget", "1", Q_NULLPTR));
        tableBCheckButton->setText(QApplication::translate("Widget", "B\350\241\250\346\243\200\346\237\245", Q_NULLPTR));
        label_5->setText(QApplication::translate("Widget", "\345\256\232\346\227\266\346\243\200\346\237\245\346\227\266\351\227\264(\347\247\222)", Q_NULLPTR));
        tableBCheckEdit->setText(QApplication::translate("Widget", "600", Q_NULLPTR));
        tableBCheckDelayLabel->setText(QApplication::translate("Widget", "1", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H

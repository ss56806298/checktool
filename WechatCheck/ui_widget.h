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

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(400, 300);
        logBrowser = new QTextBrowser(Widget);
        logBrowser->setObjectName(QStringLiteral("logBrowser"));
        logBrowser->setGeometry(QRect(10, 60, 381, 231));
        startButton = new QPushButton(Widget);
        startButton->setObjectName(QStringLiteral("startButton"));
        startButton->setGeometry(QRect(30, 20, 75, 23));
        checkTimerEdit = new QLineEdit(Widget);
        checkTimerEdit->setObjectName(QStringLiteral("checkTimerEdit"));
        checkTimerEdit->setGeometry(QRect(340, 20, 31, 20));
        checkTimerLabel = new QLabel(Widget);
        checkTimerLabel->setObjectName(QStringLiteral("checkTimerLabel"));
        checkTimerLabel->setGeometry(QRect(200, 20, 131, 20));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        startButton->setText(QApplication::translate("Widget", "\346\243\200\346\265\213\345\220\257\345\212\250", Q_NULLPTR));
        checkTimerEdit->setText(QApplication::translate("Widget", "20", Q_NULLPTR));
        checkTimerLabel->setText(QApplication::translate("Widget", "\350\267\235\347\246\273\344\270\213\346\254\241\346\243\200\346\237\245\347\232\204\346\227\266\351\227\264(\347\247\222)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H

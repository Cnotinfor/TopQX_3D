/********************************************************************************
** Form generated from reading UI file 'AnimationSelection.ui'
**
** Created: Fri 26. Aug 14:55:53 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANIMATIONSELECTION_H
#define UI_ANIMATIONSELECTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AnimationSelection
{
public:
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox;
    QLineEdit *lineEdit;
    QCheckBox *checkBox;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *AnimationSelection)
    {
        if (AnimationSelection->objectName().isEmpty())
            AnimationSelection->setObjectName(QString::fromUtf8("AnimationSelection"));
        AnimationSelection->resize(320, 240);
        verticalLayout = new QVBoxLayout(AnimationSelection);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        comboBox = new QComboBox(AnimationSelection);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout->addWidget(comboBox);

        lineEdit = new QLineEdit(AnimationSelection);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        checkBox = new QCheckBox(AnimationSelection);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout->addWidget(checkBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButton = new QPushButton(AnimationSelection);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(AnimationSelection);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);


        retranslateUi(AnimationSelection);

        QMetaObject::connectSlotsByName(AnimationSelection);
    } // setupUi

    void retranslateUi(QWidget *AnimationSelection)
    {
        AnimationSelection->setWindowTitle(QApplication::translate("AnimationSelection", "Form", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("AnimationSelection", "loop", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("AnimationSelection", "Play animation", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("AnimationSelection", "Play next animation", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AnimationSelection: public Ui_AnimationSelection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANIMATIONSELECTION_H

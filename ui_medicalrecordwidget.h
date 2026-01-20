/********************************************************************************
** Form generated from reading UI file 'medicalrecordwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDICALRECORDWIDGET_H
#define UI_MEDICALRECORDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MedicalRecordWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *pageTitle;
    QHBoxLayout *toolbarLayout;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QSpacerItem *horizontalSpacer;
    QLineEdit *searchEdit;
    QTableView *tableView;
    QLabel *messageLabel;

    void setupUi(QWidget *MedicalRecordWidget)
    {
        if (MedicalRecordWidget->objectName().isEmpty())
            MedicalRecordWidget->setObjectName(QString::fromUtf8("MedicalRecordWidget"));
        MedicalRecordWidget->resize(800, 600);
        verticalLayout = new QVBoxLayout(MedicalRecordWidget);
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(12, 12, 12, 12);
        pageTitle = new QLabel(MedicalRecordWidget);
        pageTitle->setObjectName(QString::fromUtf8("pageTitle"));
        pageTitle->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout->addWidget(pageTitle);

        toolbarLayout = new QHBoxLayout();
        toolbarLayout->setSpacing(8);
        toolbarLayout->setObjectName(QString::fromUtf8("toolbarLayout"));
        addButton = new QPushButton(MedicalRecordWidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setMinimumSize(QSize(100, 32));

        toolbarLayout->addWidget(addButton);

        editButton = new QPushButton(MedicalRecordWidget);
        editButton->setObjectName(QString::fromUtf8("editButton"));
        editButton->setEnabled(false);
        editButton->setMinimumSize(QSize(80, 32));

        toolbarLayout->addWidget(editButton);

        deleteButton = new QPushButton(MedicalRecordWidget);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setEnabled(false);
        deleteButton->setMinimumSize(QSize(80, 32));

        toolbarLayout->addWidget(deleteButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolbarLayout->addItem(horizontalSpacer);

        searchEdit = new QLineEdit(MedicalRecordWidget);
        searchEdit->setObjectName(QString::fromUtf8("searchEdit"));
        searchEdit->setMinimumSize(QSize(250, 32));

        toolbarLayout->addWidget(searchEdit);


        verticalLayout->addLayout(toolbarLayout);

        tableView = new QTableView(MedicalRecordWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableView->setSortingEnabled(true);
        tableView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(tableView);

        messageLabel = new QLabel(MedicalRecordWidget);
        messageLabel->setObjectName(QString::fromUtf8("messageLabel"));
        messageLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        messageLabel->setVisible(false);

        verticalLayout->addWidget(messageLabel);


        retranslateUi(MedicalRecordWidget);

        QMetaObject::connectSlotsByName(MedicalRecordWidget);
    } // setupUi

    void retranslateUi(QWidget *MedicalRecordWidget)
    {
        MedicalRecordWidget->setWindowTitle(QCoreApplication::translate("MedicalRecordWidget", "\347\227\205\345\216\206\347\256\241\347\220\206", nullptr));
        pageTitle->setText(QCoreApplication::translate("MedicalRecordWidget", "\347\227\205\345\216\206\344\277\241\346\201\257\347\256\241\347\220\206", nullptr));
        addButton->setText(QCoreApplication::translate("MedicalRecordWidget", "\346\267\273\345\212\240\347\227\205\345\216\206", nullptr));
        editButton->setText(QCoreApplication::translate("MedicalRecordWidget", "\347\274\226\350\276\221", nullptr));
        deleteButton->setText(QCoreApplication::translate("MedicalRecordWidget", "\345\210\240\351\231\244", nullptr));
        searchEdit->setPlaceholderText(QCoreApplication::translate("MedicalRecordWidget", "\346\214\211\346\202\243\350\200\205\345\247\223\345\220\215\343\200\201\350\257\212\346\226\255\346\210\226\345\214\273\347\224\237\346\220\234\347\264\242...", nullptr));
        messageLabel->setText(QString());
        messageLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordWidget", "color: #8B4513; font-size: 12pt;", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MedicalRecordWidget: public Ui_MedicalRecordWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDICALRECORDWIDGET_H

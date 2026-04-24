/********************************************************************************
** Form generated from reading UI file 'medicalrecordwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDICALRECORDWIDGET_H
#define UI_MEDICALRECORDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
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
    QLabel *subtitleLabel;
    QFrame *toolbarCard;
    QHBoxLayout *toolbarLayout;
    QLineEdit *searchEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QFrame *tableCard;
    QVBoxLayout *tableCardLayout;
    QTableView *tableView;
    QLabel *messageLabel;

    void setupUi(QWidget *MedicalRecordWidget)
    {
        if (MedicalRecordWidget->objectName().isEmpty())
            MedicalRecordWidget->setObjectName("MedicalRecordWidget");
        MedicalRecordWidget->resize(800, 600);
        MedicalRecordWidget->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: #F8FAFC;\n"
"}"));
        verticalLayout = new QVBoxLayout(MedicalRecordWidget);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(24, 24, 24, 24);
        pageTitle = new QLabel(MedicalRecordWidget);
        pageTitle->setObjectName("pageTitle");
        pageTitle->setStyleSheet(QString::fromUtf8("font-size: 28px; font-weight: 700; color: #1E293B;"));
        pageTitle->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout->addWidget(pageTitle);

        subtitleLabel = new QLabel(MedicalRecordWidget);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #64748B; margin-bottom: 16px;"));

        verticalLayout->addWidget(subtitleLabel);

        toolbarCard = new QFrame(MedicalRecordWidget);
        toolbarCard->setObjectName("toolbarCard");
        toolbarCard->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: #FFFFFF;\n"
"    border-radius: 12px;\n"
"    border: 1px solid #E2E8F0;\n"
"}"));
        toolbarLayout = new QHBoxLayout(toolbarCard);
        toolbarLayout->setSpacing(12);
        toolbarLayout->setObjectName("toolbarLayout");
        toolbarLayout->setContentsMargins(16, 16, 16, 16);
        searchEdit = new QLineEdit(toolbarCard);
        searchEdit->setObjectName("searchEdit");
        searchEdit->setMinimumSize(QSize(320, 40));
        searchEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #F8FAFC;\n"
"    border: 2px solid #E2E8F0;\n"
"    border-radius: 8px;\n"
"    padding: 10px 14px 10px 36px;\n"
"    color: #1E293B;\n"
"    font-size: 14px;\n"
"}\n"
"QLineEdit:hover {\n"
"    border-color: #CBD5E1;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #2563EB;\n"
"    background-color: #FFFFFF;\n"
"}\n"
"QLineEdit::placeholder {\n"
"    color: #94A3B8;\n"
"}"));

        toolbarLayout->addWidget(searchEdit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        toolbarLayout->addItem(horizontalSpacer);

        addButton = new QPushButton(toolbarCard);
        addButton->setObjectName("addButton");
        addButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #2563EB;\n"
"    color: #FFFFFF;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 10px 20px;\n"
"    font-weight: 600;\n"
"    font-size: 14px;\n"
"    min-width: 100px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #3B82F6;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #1D4ED8;\n"
"}"));
        addButton->setMinimumSize(QSize(120, 40));

        toolbarLayout->addWidget(addButton);

        editButton = new QPushButton(toolbarCard);
        editButton->setObjectName("editButton");
        editButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #FFFFFF;\n"
"    color: #374151;\n"
"    border: 1px solid #D1D5DB;\n"
"    border-radius: 8px;\n"
"    padding: 10px 20px;\n"
"    font-weight: 600;\n"
"    font-size: 14px;\n"
"    min-width: 80px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #F9FAFB;\n"
"    border-color: #9CA3AF;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #F3F4F6;\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: #F3F4F6;\n"
"    color: #9CA3AF;\n"
"    border-color: #E5E7EB;\n"
"}"));
        editButton->setEnabled(false);
        editButton->setMinimumSize(QSize(80, 40));

        toolbarLayout->addWidget(editButton);

        deleteButton = new QPushButton(toolbarCard);
        deleteButton->setObjectName("deleteButton");
        deleteButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #EF4444;\n"
"    color: #FFFFFF;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 10px 20px;\n"
"    font-weight: 600;\n"
"    font-size: 14px;\n"
"    min-width: 80px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #F87171;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DC2626;\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: #FECACA;\n"
"    color: #FCA5A5;\n"
"}"));
        deleteButton->setEnabled(false);
        deleteButton->setMinimumSize(QSize(80, 40));

        toolbarLayout->addWidget(deleteButton);


        verticalLayout->addWidget(toolbarCard);

        tableCard = new QFrame(MedicalRecordWidget);
        tableCard->setObjectName("tableCard");
        tableCard->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: #FFFFFF;\n"
"    border-radius: 12px;\n"
"    border: 1px solid #E2E8F0;\n"
"}"));
        tableCardLayout = new QVBoxLayout(tableCard);
        tableCardLayout->setSpacing(0);
        tableCardLayout->setObjectName("tableCardLayout");
        tableCardLayout->setContentsMargins(0, 0, 0, 0);
        tableView = new QTableView(tableCard);
        tableView->setObjectName("tableView");
        tableView->setStyleSheet(QString::fromUtf8("QTableView {\n"
"    background-color: #FFFFFF;\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    color: #1E293B;\n"
"    selection-background-color: #DBEAFE;\n"
"    selection-color: #1E293B;\n"
"    alternate-background-color: #F8FAFC;\n"
"}\n"
"QTableView::item {\n"
"    padding: 14px 16px;\n"
"    border-bottom: 1px solid #F1F5F9;\n"
"}\n"
"QTableView::item:selected {\n"
"    background-color: #DBEAFE;\n"
"}\n"
"QTableView::item:hover {\n"
"    background-color: #F1F5F9;\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #F8FAFC;\n"
"    color: #475569;\n"
"    padding: 14px 16px;\n"
"    border: none;\n"
"    border-bottom: 2px solid #E2E8F0;\n"
"    font-weight: 700;\n"
"    font-size: 13px;\n"
"    text-transform: uppercase;\n"
"}\n"
"QHeaderView::section:hover {\n"
"    background-color: #F1F5F9;\n"
"}\n"
"QScrollBar:vertical {\n"
"    background-color: #F1F5F9;\n"
"    width: 12px;\n"
"    border-radius: 6px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background-color: #CBD5E1;\n"
""
                        "    min-height: 30px;\n"
"    border-radius: 6px;\n"
"}\n"
"QScrollBar::handle:vertical:hover {\n"
"    background-color: #94A3B8;\n"
"}\n"
"QScrollBar::handle:vertical:pressed {\n"
"    background-color: #64748B;\n"
"}\n"
"QScrollBar::add-line:vertical,\n"
"QScrollBar::sub-line:vertical {\n"
"    height: 0px;\n"
"}\n"
"QScrollBar:horizontal {\n"
"    background-color: #F1F5F9;\n"
"    height: 12px;\n"
"    border-radius: 6px;\n"
"}\n"
"QScrollBar::handle:horizontal {\n"
"    background-color: #CBD5E1;\n"
"    min-width: 30px;\n"
"    border-radius: 6px;\n"
"}\n"
"QScrollBar::handle:horizontal:hover {\n"
"    background-color: #94A3B8;\n"
"}\n"
"QScrollBar::handle:horizontal:pressed {\n"
"    background-color: #64748B;\n"
"}\n"
"QScrollBar::add-line:horizontal,\n"
"QScrollBar::sub-line:horizontal {\n"
"    width: 0px;\n"
"}"));
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableView->setSortingEnabled(true);
        tableView->setShowGrid(false);
        tableView->horizontalHeader()->setStretchLastSection(true);

        tableCardLayout->addWidget(tableView);


        verticalLayout->addWidget(tableCard);

        messageLabel = new QLabel(MedicalRecordWidget);
        messageLabel->setObjectName("messageLabel");
        messageLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        messageLabel->setVisible(false);
        messageLabel->setStyleSheet(QString::fromUtf8("color: #64748B; font-size: 14px;"));

        verticalLayout->addWidget(messageLabel);


        retranslateUi(MedicalRecordWidget);

        QMetaObject::connectSlotsByName(MedicalRecordWidget);
    } // setupUi

    void retranslateUi(QWidget *MedicalRecordWidget)
    {
        MedicalRecordWidget->setWindowTitle(QCoreApplication::translate("MedicalRecordWidget", "\347\227\205\345\216\206\347\256\241\347\220\206", nullptr));
        pageTitle->setText(QCoreApplication::translate("MedicalRecordWidget", "\347\227\205\345\216\206\344\277\241\346\201\257\347\256\241\347\220\206", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("MedicalRecordWidget", "\347\256\241\347\220\206\346\202\243\350\200\205\347\232\204\347\227\205\345\216\206\350\256\260\345\275\225\343\200\201\350\257\212\346\226\255\344\277\241\346\201\257\345\222\214\346\262\273\347\226\227\346\226\271\346\241\210", nullptr));
        searchEdit->setPlaceholderText(QCoreApplication::translate("MedicalRecordWidget", "\360\237\224\215 \346\214\211\346\202\243\350\200\205\345\247\223\345\220\215\343\200\201\350\257\212\346\226\255\346\210\226\345\214\273\347\224\237\346\220\234\347\264\242...", nullptr));
        addButton->setText(QCoreApplication::translate("MedicalRecordWidget", "+ \346\267\273\345\212\240\347\227\205\345\216\206", nullptr));
        editButton->setText(QCoreApplication::translate("MedicalRecordWidget", "\347\274\226\350\276\221", nullptr));
        deleteButton->setText(QCoreApplication::translate("MedicalRecordWidget", "\345\210\240\351\231\244", nullptr));
        messageLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MedicalRecordWidget: public Ui_MedicalRecordWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDICALRECORDWIDGET_H

/********************************************************************************
** Form generated from reading UI file 'medicalrecorddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDICALRECORDDIALOG_H
#define UI_MEDICALRECORDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MedicalRecordDialog
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QGroupBox *patientInfoGroup;
    QFormLayout *patientFormLayout;
    QLabel *patientLabel;
    QComboBox *patientComboBox;
    QFrame *patientDetailsFrame;
    QGridLayout *patientDetailsLayout;
    QLabel *patientNameLabel;
    QLabel *patientNameValue;
    QLabel *patientAgeLabel;
    QLabel *patientAgeValue;
    QLabel *patientGenderLabel;
    QLabel *patientGenderValue;
    QLabel *patientPhoneLabel;
    QLabel *patientPhoneValue;
    QGroupBox *medicalRecordGroup;
    QFormLayout *medicalFormLayout;
    QLabel *diagnosisLabel;
    QLineEdit *diagnosisEdit;
    QLabel *symptomsLabel;
    QTextEdit *symptomsEdit;
    QLabel *treatmentLabel;
    QTextEdit *treatmentEdit;
    QLabel *doctorNameLabel;
    QLineEdit *doctorNameEdit;
    QLabel *notesLabel;
    QTextEdit *notesEdit;
    QLabel *errorLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *buttonLayout;
    QSpacerItem *buttonSpacer;
    QPushButton *cancelButton;
    QPushButton *saveButton;

    void setupUi(QDialog *MedicalRecordDialog)
    {
        if (MedicalRecordDialog->objectName().isEmpty())
            MedicalRecordDialog->setObjectName("MedicalRecordDialog");
        MedicalRecordDialog->resize(500, 650);
        MedicalRecordDialog->setModal(true);
        mainLayout = new QVBoxLayout(MedicalRecordDialog);
        mainLayout->setSpacing(12);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        titleLabel = new QLabel(MedicalRecordDialog);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        mainLayout->addWidget(titleLabel);

        patientInfoGroup = new QGroupBox(MedicalRecordDialog);
        patientInfoGroup->setObjectName("patientInfoGroup");
        patientFormLayout = new QFormLayout(patientInfoGroup);
        patientFormLayout->setObjectName("patientFormLayout");
        patientFormLayout->setHorizontalSpacing(10);
        patientFormLayout->setVerticalSpacing(8);
        patientLabel = new QLabel(patientInfoGroup);
        patientLabel->setObjectName("patientLabel");

        patientFormLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, patientLabel);

        patientComboBox = new QComboBox(patientInfoGroup);
        patientComboBox->setObjectName("patientComboBox");
        patientComboBox->setMinimumSize(QSize(200, 32));
        patientComboBox->setEditable(false);

        patientFormLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, patientComboBox);

        patientDetailsFrame = new QFrame(patientInfoGroup);
        patientDetailsFrame->setObjectName("patientDetailsFrame");
        patientDetailsFrame->setFrameShape(QFrame::Shape::StyledPanel);
        patientDetailsFrame->setFrameShadow(QFrame::Shadow::Sunken);
        patientDetailsLayout = new QGridLayout(patientDetailsFrame);
        patientDetailsLayout->setSpacing(6);
        patientDetailsLayout->setObjectName("patientDetailsLayout");
        patientNameLabel = new QLabel(patientDetailsFrame);
        patientNameLabel->setObjectName("patientNameLabel");

        patientDetailsLayout->addWidget(patientNameLabel, 0, 0, 1, 1);

        patientNameValue = new QLabel(patientDetailsFrame);
        patientNameValue->setObjectName("patientNameValue");

        patientDetailsLayout->addWidget(patientNameValue, 0, 1, 1, 1);

        patientAgeLabel = new QLabel(patientDetailsFrame);
        patientAgeLabel->setObjectName("patientAgeLabel");

        patientDetailsLayout->addWidget(patientAgeLabel, 0, 2, 1, 1);

        patientAgeValue = new QLabel(patientDetailsFrame);
        patientAgeValue->setObjectName("patientAgeValue");

        patientDetailsLayout->addWidget(patientAgeValue, 0, 3, 1, 1);

        patientGenderLabel = new QLabel(patientDetailsFrame);
        patientGenderLabel->setObjectName("patientGenderLabel");

        patientDetailsLayout->addWidget(patientGenderLabel, 1, 0, 1, 1);

        patientGenderValue = new QLabel(patientDetailsFrame);
        patientGenderValue->setObjectName("patientGenderValue");

        patientDetailsLayout->addWidget(patientGenderValue, 1, 1, 1, 1);

        patientPhoneLabel = new QLabel(patientDetailsFrame);
        patientPhoneLabel->setObjectName("patientPhoneLabel");

        patientDetailsLayout->addWidget(patientPhoneLabel, 1, 2, 1, 1);

        patientPhoneValue = new QLabel(patientDetailsFrame);
        patientPhoneValue->setObjectName("patientPhoneValue");

        patientDetailsLayout->addWidget(patientPhoneValue, 1, 3, 1, 1);


        patientFormLayout->setWidget(1, QFormLayout::ItemRole::SpanningRole, patientDetailsFrame);


        mainLayout->addWidget(patientInfoGroup);

        medicalRecordGroup = new QGroupBox(MedicalRecordDialog);
        medicalRecordGroup->setObjectName("medicalRecordGroup");
        medicalFormLayout = new QFormLayout(medicalRecordGroup);
        medicalFormLayout->setObjectName("medicalFormLayout");
        medicalFormLayout->setHorizontalSpacing(10);
        medicalFormLayout->setVerticalSpacing(8);
        diagnosisLabel = new QLabel(medicalRecordGroup);
        diagnosisLabel->setObjectName("diagnosisLabel");

        medicalFormLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, diagnosisLabel);

        diagnosisEdit = new QLineEdit(medicalRecordGroup);
        diagnosisEdit->setObjectName("diagnosisEdit");
        diagnosisEdit->setMinimumSize(QSize(300, 32));
        diagnosisEdit->setMaxLength(500);

        medicalFormLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, diagnosisEdit);

        symptomsLabel = new QLabel(medicalRecordGroup);
        symptomsLabel->setObjectName("symptomsLabel");
        symptomsLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);

        medicalFormLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, symptomsLabel);

        symptomsEdit = new QTextEdit(medicalRecordGroup);
        symptomsEdit->setObjectName("symptomsEdit");
        symptomsEdit->setMinimumSize(QSize(300, 80));
        symptomsEdit->setMaximumSize(QSize(16777215, 80));

        medicalFormLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, symptomsEdit);

        treatmentLabel = new QLabel(medicalRecordGroup);
        treatmentLabel->setObjectName("treatmentLabel");
        treatmentLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);

        medicalFormLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, treatmentLabel);

        treatmentEdit = new QTextEdit(medicalRecordGroup);
        treatmentEdit->setObjectName("treatmentEdit");
        treatmentEdit->setMinimumSize(QSize(300, 80));
        treatmentEdit->setMaximumSize(QSize(16777215, 80));

        medicalFormLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, treatmentEdit);

        doctorNameLabel = new QLabel(medicalRecordGroup);
        doctorNameLabel->setObjectName("doctorNameLabel");

        medicalFormLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, doctorNameLabel);

        doctorNameEdit = new QLineEdit(medicalRecordGroup);
        doctorNameEdit->setObjectName("doctorNameEdit");
        doctorNameEdit->setMinimumSize(QSize(300, 32));
        doctorNameEdit->setMaxLength(100);

        medicalFormLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, doctorNameEdit);

        notesLabel = new QLabel(medicalRecordGroup);
        notesLabel->setObjectName("notesLabel");
        notesLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);

        medicalFormLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, notesLabel);

        notesEdit = new QTextEdit(medicalRecordGroup);
        notesEdit->setObjectName("notesEdit");
        notesEdit->setMinimumSize(QSize(300, 60));
        notesEdit->setMaximumSize(QSize(16777215, 60));

        medicalFormLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, notesEdit);


        mainLayout->addWidget(medicalRecordGroup);

        errorLabel = new QLabel(MedicalRecordDialog);
        errorLabel->setObjectName("errorLabel");
        errorLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        errorLabel->setVisible(false);
        errorLabel->setWordWrap(true);

        mainLayout->addWidget(errorLabel);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName("buttonLayout");
        buttonSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(buttonSpacer);

        cancelButton = new QPushButton(MedicalRecordDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(80, 35));

        buttonLayout->addWidget(cancelButton);

        saveButton = new QPushButton(MedicalRecordDialog);
        saveButton->setObjectName("saveButton");
        saveButton->setMinimumSize(QSize(80, 35));

        buttonLayout->addWidget(saveButton);


        mainLayout->addLayout(buttonLayout);


        retranslateUi(MedicalRecordDialog);

        saveButton->setDefault(true);


        QMetaObject::connectSlotsByName(MedicalRecordDialog);
    } // setupUi

    void retranslateUi(QDialog *MedicalRecordDialog)
    {
        MedicalRecordDialog->setWindowTitle(QCoreApplication::translate("MedicalRecordDialog", "\347\227\205\345\216\206\344\277\241\346\201\257", nullptr));
        titleLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\347\227\205\345\216\206\344\277\241\346\201\257", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "font-size: 16pt; font-weight: bold; color: #8B4513; margin-bottom: 10px;", nullptr));
        patientInfoGroup->setTitle(QCoreApplication::translate("MedicalRecordDialog", "\346\202\243\350\200\205\344\277\241\346\201\257", nullptr));
        patientInfoGroup->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "QGroupBox { font-weight: bold; color: #8B4513; }", nullptr));
        patientLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\346\202\243\350\200\205:", nullptr));
        patientDetailsFrame->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "QFrame { background-color: #FFF8DC; border: 1px solid #D2B48C; border-radius: 6px; padding: 8px; }", nullptr));
        patientNameLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\345\247\223\345\220\215:", nullptr));
        patientNameLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "font-weight: bold;", nullptr));
        patientNameValue->setText(QCoreApplication::translate("MedicalRecordDialog", "-", nullptr));
        patientAgeLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\345\271\264\351\276\204:", nullptr));
        patientAgeLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "font-weight: bold;", nullptr));
        patientAgeValue->setText(QCoreApplication::translate("MedicalRecordDialog", "-", nullptr));
        patientGenderLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\346\200\247\345\210\253:", nullptr));
        patientGenderLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "font-weight: bold;", nullptr));
        patientGenderValue->setText(QCoreApplication::translate("MedicalRecordDialog", "-", nullptr));
        patientPhoneLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\347\224\265\350\257\235:", nullptr));
        patientPhoneLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "font-weight: bold;", nullptr));
        patientPhoneValue->setText(QCoreApplication::translate("MedicalRecordDialog", "-", nullptr));
        medicalRecordGroup->setTitle(QCoreApplication::translate("MedicalRecordDialog", "\347\227\205\345\216\206\344\277\241\346\201\257", nullptr));
        medicalRecordGroup->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "QGroupBox { font-weight: bold; color: #8B4513; }", nullptr));
        diagnosisLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\350\257\212\346\226\255: *", nullptr));
        diagnosisLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "color: #8B4513;", nullptr));
        diagnosisEdit->setPlaceholderText(QCoreApplication::translate("MedicalRecordDialog", "\350\257\267\350\276\223\345\205\245\350\257\212\346\226\255\344\277\241\346\201\257\357\274\210\345\277\205\345\241\253\357\274\214\346\234\200\345\244\232500\345\255\227\347\254\246\357\274\211", nullptr));
        symptomsLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\347\227\207\347\212\266:", nullptr));
        symptomsLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "color: #8B4513;", nullptr));
        symptomsEdit->setPlaceholderText(QCoreApplication::translate("MedicalRecordDialog", "\350\257\267\350\276\223\345\205\245\347\227\207\347\212\266\346\217\217\350\277\260\357\274\210\346\234\200\345\244\2321000\345\255\227\347\254\246\357\274\211", nullptr));
        treatmentLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\346\262\273\347\226\227\346\226\271\346\241\210:", nullptr));
        treatmentLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "color: #8B4513;", nullptr));
        treatmentEdit->setPlaceholderText(QCoreApplication::translate("MedicalRecordDialog", "\350\257\267\350\276\223\345\205\245\346\262\273\347\226\227\346\226\271\346\241\210\357\274\210\346\234\200\345\244\2321000\345\255\227\347\254\246\357\274\211", nullptr));
        doctorNameLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\345\214\273\347\224\237\345\247\223\345\220\215: *", nullptr));
        doctorNameLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "color: #8B4513;", nullptr));
        doctorNameEdit->setPlaceholderText(QCoreApplication::translate("MedicalRecordDialog", "\350\257\267\350\276\223\345\205\245\345\214\273\347\224\237\345\247\223\345\220\215\357\274\210\345\277\205\345\241\253\357\274\214\346\234\200\345\244\232100\345\255\227\347\254\246\357\274\211", nullptr));
        notesLabel->setText(QCoreApplication::translate("MedicalRecordDialog", "\345\244\207\346\263\250:", nullptr));
        notesLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "color: #8B4513;", nullptr));
        notesEdit->setPlaceholderText(QCoreApplication::translate("MedicalRecordDialog", "\350\257\267\350\276\223\345\205\245\345\244\207\346\263\250\344\277\241\346\201\257\357\274\210\346\234\200\345\244\2322000\345\255\227\347\254\246\357\274\211", nullptr));
        errorLabel->setText(QString());
        errorLabel->setStyleSheet(QCoreApplication::translate("MedicalRecordDialog", "color: red; font-weight: bold;", nullptr));
        cancelButton->setText(QCoreApplication::translate("MedicalRecordDialog", "\345\217\226\346\266\210", nullptr));
        saveButton->setText(QCoreApplication::translate("MedicalRecordDialog", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MedicalRecordDialog: public Ui_MedicalRecordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDICALRECORDDIALOG_H

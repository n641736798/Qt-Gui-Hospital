#include "patientdialog.h"
#include <QMessageBox>
#include <QDate>

PatientDialog::PatientDialog(QWidget *parent)
    : QDialog(parent), isEditMode(false)
{
    setupUI();
    setWindowTitle("添加新患者");
}

PatientDialog::PatientDialog(const Patient &patient, QWidget *parent)
    : QDialog(parent), currentPatient(patient), isEditMode(true)
{
    setupUI();
    populateFields(patient);
    setWindowTitle("编辑患者信息");
}

void PatientDialog::setupUI()
{
    setFixedSize(400, 500);
    setStyleSheet(R"(
        QDialog {
            background-color: #F5E6D3;
            color: #8B4513;
        }
        QLabel {
            color: #8B4513;
            font-weight: bold;
        }
        QLineEdit, QTextEdit, QComboBox, QDateEdit {
            background-color: #FFFFFF;
            border: 1px solid #D2B48C;
            border-radius: 6px;
            padding: 8px;
            color: #8B4513;
            font-size: 14px;
        }
        QLineEdit:focus, QTextEdit:focus, QComboBox:focus, QDateEdit:focus {
            border-color: #FF8C42;
        }
        QPushButton {
            background-color: #FF8C42;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #FF9F5A;
        }
        QPushButton:pressed {
            background-color: #E6793A;
        }
        QPushButton#cancelButton {
            background-color: #E8D5C4;
            color: #8B4513;
            border: 1px solid #D2B48C;
        }
        QPushButton#cancelButton:hover {
            background-color: #DEB887;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Form layout
    QFormLayout *formLayout = new QFormLayout();
    
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("请输入患者姓名");
    formLayout->addRow("姓名:", nameEdit);
    
    phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("请输入电话号码");
    formLayout->addRow("电话:", phoneEdit);
    
    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("请输入邮箱地址");
    formLayout->addRow("邮箱:", emailEdit);
    
    addressEdit = new QLineEdit();
    addressEdit->setPlaceholderText("请输入地址");
    formLayout->addRow("地址:", addressEdit);
    
    birthDateEdit = new QDateEdit();
    birthDateEdit->setDate(QDate::currentDate().addYears(-30));
    birthDateEdit->setCalendarPopup(true);
    formLayout->addRow("出生日期:", birthDateEdit);
    
    genderCombo = new QComboBox();
    genderCombo->addItems({"男", "女", "其他"});
    formLayout->addRow("性别:", genderCombo);
    
    medicalHistoryEdit = new QTextEdit();
    medicalHistoryEdit->setPlaceholderText("请输入病史信息");
    medicalHistoryEdit->setMaximumHeight(100);
    formLayout->addRow("病史:", medicalHistoryEdit);
    
    mainLayout->addLayout(formLayout);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    saveButton = new QPushButton("保存");
    cancelButton = new QPushButton("取消");
    cancelButton->setObjectName("cancelButton");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(saveButton, &QPushButton::clicked, this, &PatientDialog::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &PatientDialog::onCancelClicked);
}

void PatientDialog::populateFields(const Patient &patient)
{
    nameEdit->setText(patient.name);
    phoneEdit->setText(patient.phone);
    emailEdit->setText(patient.email);
    addressEdit->setText(patient.address);
    
    QDate birthDate = QDate::fromString(patient.birthDate, "yyyy-MM-dd");
    if (birthDate.isValid()) {
        birthDateEdit->setDate(birthDate);
    }
    
    // 映射英文性别到中文
    QString genderText = patient.gender;
    if (genderText == "Male") genderText = "男";
    else if (genderText == "Female") genderText = "女";
    else if (genderText == "Other") genderText = "其他";
    
    int genderIndex = genderCombo->findText(genderText);
    if (genderIndex >= 0) {
        genderCombo->setCurrentIndex(genderIndex);
    }
    
    medicalHistoryEdit->setPlainText(patient.medicalHistory);
}

Patient PatientDialog::getPatient() const
{
    Patient patient = currentPatient;
    patient.name = nameEdit->text().trimmed();
    patient.phone = phoneEdit->text().trimmed();
    patient.email = emailEdit->text().trimmed();
    patient.address = addressEdit->text().trimmed();
    patient.birthDate = birthDateEdit->date().toString("yyyy-MM-dd");
    
    // 映射中文性别到英文存储
    QString genderText = genderCombo->currentText();
    if (genderText == "男") patient.gender = "Male";
    else if (genderText == "女") patient.gender = "Female";
    else if (genderText == "其他") patient.gender = "Other";
    else patient.gender = genderText; // 保持原值
    
    patient.medicalHistory = medicalHistoryEdit->toPlainText().trimmed();
    
    return patient;
}

void PatientDialog::onSaveClicked()
{
    // Validate required fields
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "验证错误", "患者姓名不能为空。");
        nameEdit->setFocus();
        return;
    }
    
    if (phoneEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "验证错误", "电话号码不能为空。");
        phoneEdit->setFocus();
        return;
    }
    
    accept();
}

void PatientDialog::onCancelClicked()
{
    reject();
}
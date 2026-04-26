#include "medicalrecorddialog.h"
#include "ui_medicalrecorddialog.h"
#include <QDate>
#include <QDebug>

MedicalRecordDialog::MedicalRecordDialog(Mode mode, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MedicalRecordDialog)
    , m_mode(mode)
    , m_dbManager(&DatabaseManager::instance())
    , m_dataLoaded(false)
{
    Q_UNUSED(m_dataLoaded); // Mark as intentionally unused
    ui->setupUi(this);
    setupUI();
    setupConnections();
    setupValidation();
    loadPatients();
    
    // Set window title based on mode
    if (m_mode == Create) {
        setWindowTitle("添加病历");
        ui->titleLabel->setText("添加新病历");
    } else {
        setWindowTitle("编辑病历");
        ui->titleLabel->setText("编辑病历信息");
    }
}

MedicalRecordDialog::~MedicalRecordDialog()
{
    delete ui;
}

void MedicalRecordDialog::setupUI()
{
    // Set fixed size and apply styling
    setFixedSize(500, 650);
    setStyleSheet(R"(
        QDialog {
            background-color: #F5E6D3;
            color: #8B4513;
        }
        QLabel {
            color: #8B4513;
        }
        QLineEdit, QTextEdit, QComboBox {
            background-color: #FFFFFF;
            border: 1px solid #D2B48C;
            border-radius: 6px;
            padding: 8px;
            color: #8B4513;
            font-size: 14px;
        }
        QLineEdit:focus, QTextEdit:focus, QComboBox:focus {
            border-color: #FF8C42;
            border-width: 2px;
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
        QGroupBox {
            font-weight: bold;
            color: #8B4513;
            border: 2px solid #D2B48C;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
    )");
    
    // Set object names for styling
    ui->cancelButton->setObjectName("cancelButton");
    
    // Initially hide patient details frame
    ui->patientDetailsFrame->setVisible(false);
    
    // Set text limits for QTextEdit widgets
    // Note: QTextEdit doesn't have maxLength property, we'll handle this in validation
}

void MedicalRecordDialog::setupConnections()
{
    // Connect patient selection change
    connect(ui->patientComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MedicalRecordDialog::onPatientChanged);
    
    // Connect buttons
    connect(ui->saveButton, &QPushButton::clicked, this, &MedicalRecordDialog::onSaveClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &MedicalRecordDialog::onCancelClicked);
    
    // Connect text change events for real-time validation feedback
    connect(ui->diagnosisEdit, &QLineEdit::textChanged, this, &MedicalRecordDialog::onTextChanged);
    connect(ui->doctorNameEdit, &QLineEdit::textChanged, this, &MedicalRecordDialog::onTextChanged);
    connect(ui->symptomsEdit, &QTextEdit::textChanged, this, &MedicalRecordDialog::onTextChanged);
    connect(ui->treatmentEdit, &QTextEdit::textChanged, this, &MedicalRecordDialog::onTextChanged);
    connect(ui->notesEdit, &QTextEdit::textChanged, this, &MedicalRecordDialog::onTextChanged);
}

void MedicalRecordDialog::setupValidation()
{
    // Set input validators and limits
    ui->diagnosisEdit->setMaxLength(500);
    ui->doctorNameEdit->setMaxLength(100);
    
    // For QTextEdit, we'll check length in validation
}

void MedicalRecordDialog::loadPatients()
{
    ui->patientComboBox->clear();
    ui->patientComboBox->addItem("请选择患者...", -1);
    
    QList<Patient> patients = m_dbManager->getAllPatients();
    for (const Patient &patient : patients) {
        ui->patientComboBox->addItem(patient.name, patient.id);
    }
}

void MedicalRecordDialog::loadPatientInfo(int patientId)
{
    if (patientId <= 0) {
        ui->patientDetailsFrame->setVisible(false);
        return;
    }
    
    Patient patient = m_dbManager->getPatientById(patientId);
    if (patient.id <= 0) {
        ui->patientDetailsFrame->setVisible(false);
        return;
    }
    
    // Update patient information display
    ui->patientNameValue->setText(patient.name);
    ui->patientAgeValue->setText(QString::number(calculateAge(patient.birthDate)));
    ui->patientGenderValue->setText(mapGenderToDisplay(patient.gender));
    ui->patientPhoneValue->setText(patient.phone);
    
    ui->patientDetailsFrame->setVisible(true);
}

void MedicalRecordDialog::setPatientId(int patientId)
{
    if (m_mode != Create) {
        qWarning() << "setPatientId should only be called in Create mode";
        return;
    }
    
    // Find and select the patient in combo box
    for (int i = 0; i < ui->patientComboBox->count(); ++i) {
        if (ui->patientComboBox->itemData(i).toInt() == patientId) {
            ui->patientComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void MedicalRecordDialog::setMedicalRecord(const MedicalRecord &record)
{
    if (m_mode != Edit) {
        qWarning() << "setMedicalRecord should only be called in Edit mode";
        return;
    }
    
    m_record = record;
    m_dataLoaded = true;
    
    // Set patient selection
    for (int i = 0; i < ui->patientComboBox->count(); ++i) {
        if (ui->patientComboBox->itemData(i).toInt() == record.patientId) {
            ui->patientComboBox->setCurrentIndex(i);
            break;
        }
    }
    
    // In edit mode, disable patient selection
    ui->patientComboBox->setEnabled(false);
    
    // Fill form fields
    ui->diagnosisEdit->setText(record.diagnosis);
    ui->symptomsEdit->setPlainText(record.symptoms);
    ui->treatmentEdit->setPlainText(record.treatment);
    ui->doctorNameEdit->setText(record.doctorName);
    ui->notesEdit->setPlainText(record.notes);
}

MedicalRecord MedicalRecordDialog::getMedicalRecord() const
{
    MedicalRecord record = m_record; // Copy existing record (for edit mode)
    
    // Update with form data
    record.patientId = ui->patientComboBox->currentData().toInt();
    record.diagnosis = ui->diagnosisEdit->text().trimmed();
    record.symptoms = ui->symptomsEdit->toPlainText().trimmed();
    record.treatment = ui->treatmentEdit->toPlainText().trimmed();
    record.doctorName = ui->doctorNameEdit->text().trimmed();
    record.notes = ui->notesEdit->toPlainText().trimmed();
    
    // For create mode, set creation date
    if (m_mode == Create) {
        record.createdDate = QDateTime::currentDateTime();
    }
    
    return record;
}

void MedicalRecordDialog::onPatientChanged()
{
    int patientId = ui->patientComboBox->currentData().toInt();
    loadPatientInfo(patientId);
    
    // Clear error when patient is selected
    if (patientId > 0) {
        hideError();
        resetFieldStyles();
    }
}

void MedicalRecordDialog::onSaveClicked()
{
    if (validateInput()) {
        accept();
    }
}

void MedicalRecordDialog::onCancelClicked()
{
    reject();
}

void MedicalRecordDialog::onTextChanged()
{
    // Clear error styling when user starts typing
    hideError();
    resetFieldStyles();
}

bool MedicalRecordDialog::validateInput()
{
    QStringList errors;
    resetFieldStyles();
    
    // Validate patient selection
    int patientId = ui->patientComboBox->currentData().toInt();
    if (patientId <= 0) {
        errors << "请选择患者";
        highlightErrorField(ui->patientComboBox);
    }
    
    // Validate required fields
    QString diagnosis = ui->diagnosisEdit->text().trimmed();
    if (diagnosis.isEmpty()) {
        errors << "诊断信息不能为空";
        highlightErrorField(ui->diagnosisEdit);
    } else if (diagnosis.length() > 500) {
        errors << "诊断信息不能超过500个字符";
        highlightErrorField(ui->diagnosisEdit);
    }
    
    QString doctorName = ui->doctorNameEdit->text().trimmed();
    if (doctorName.isEmpty()) {
        errors << "医生姓名不能为空";
        highlightErrorField(ui->doctorNameEdit);
    } else if (doctorName.length() > 100) {
        errors << "医生姓名不能超过100个字符";
        highlightErrorField(ui->doctorNameEdit);
    }
    
    // Validate optional field lengths
    QString symptoms = ui->symptomsEdit->toPlainText().trimmed();
    if (symptoms.length() > 1000) {
        errors << "症状描述不能超过1000个字符";
        highlightErrorField(ui->symptomsEdit);
    }
    
    QString treatment = ui->treatmentEdit->toPlainText().trimmed();
    if (treatment.length() > 1000) {
        errors << "治疗方案不能超过1000个字符";
        highlightErrorField(ui->treatmentEdit);
    }
    
    QString notes = ui->notesEdit->toPlainText().trimmed();
    if (notes.length() > 2000) {
        errors << "备注信息不能超过2000个字符";
        highlightErrorField(ui->notesEdit);
    }
    
    if (!errors.isEmpty()) {
        QString message = "请修正以下错误:\n• " + errors.join("\n• ");
        showError(message);
        return false;
    }
    
    return true;
}

void MedicalRecordDialog::showError(const QString &message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->setVisible(true);
}

void MedicalRecordDialog::hideError()
{
    ui->errorLabel->setVisible(false);
    ui->errorLabel->clear();
}

void MedicalRecordDialog::resetFieldStyles()
{
    // Reset all field styles to normal
    QString normalStyle = R"(
        background-color: #FFFFFF;
        border: 1px solid #D2B48C;
        border-radius: 6px;
        padding: 8px;
        color: #8B4513;
        font-size: 14px;
    )";
    
    ui->patientComboBox->setStyleSheet(normalStyle);
    ui->diagnosisEdit->setStyleSheet(normalStyle);
    ui->doctorNameEdit->setStyleSheet(normalStyle);
    ui->symptomsEdit->setStyleSheet(normalStyle);
    ui->treatmentEdit->setStyleSheet(normalStyle);
    ui->notesEdit->setStyleSheet(normalStyle);
}

void MedicalRecordDialog::highlightErrorField(QWidget *field)
{
    QString errorStyle = R"(
        background-color: #FFFFFF;
        border: 2px solid #FF0000;
        border-radius: 6px;
        padding: 8px;
        color: #8B4513;
        font-size: 14px;
    )";
    
    field->setStyleSheet(errorStyle);
}

int MedicalRecordDialog::calculateAge(const QString &birthDate) const
{
    QDate birth = QDate::fromString(birthDate, "yyyy-MM-dd");
    if (!birth.isValid()) {
        return 0;
    }
    
    QDate today = QDate::currentDate();
    int age = today.year() - birth.year();
    
    // Adjust if birthday hasn't occurred this year
    if (today.month() < birth.month() || 
        (today.month() == birth.month() && today.day() < birth.day())) {
        age--;
    }
    
    return age;
}

QString MedicalRecordDialog::mapGenderToDisplay(const QString &gender) const
{
    if (gender == "Male") return "男";
    if (gender == "Female") return "女";
    if (gender == "Other") return "其他";
    return gender; // Return as-is if already in Chinese or unknown
}
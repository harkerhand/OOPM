#include "CreateMemberDialog.h"
#include "Utils.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QInputDialog>
#include <string>

CreateMemberDialog::CreateMemberDialog(QWidget *parent)
    : QDialog(parent) {
    QFormLayout *formLayout = new QFormLayout(this);

    _idEdit = new QLineEdit(this);
    _nameEdit = new QLineEdit(this);
    _memberTypeComboBox = new QComboBox(this);
    _memorySizeSpinBox = new QSpinBox(this);
    _dataTypeComboBox = new QComboBox(this);
    _accessibilityComboBox = new QComboBox(this);

    // 成员类型选择框
    _memberTypeComboBox->addItem("Data", QVariant::fromValue(MemberType::Data));
    _memberTypeComboBox->addItem("Function", QVariant::fromValue(MemberType::Function));
    // 数据类型选择框
    _dataTypeComboBox->addItem("Int", QVariant::fromValue(DataType::Int));
    _dataTypeComboBox->addItem("Float", QVariant::fromValue(DataType::Float));
    _dataTypeComboBox->addItem("Double", QVariant::fromValue(DataType::Double));
    _dataTypeComboBox->addItem("Char", QVariant::fromValue(DataType::Char));
    _dataTypeComboBox->addItem("String", QVariant::fromValue(DataType::String));
    _dataTypeComboBox->addItem("Custom", QVariant::fromValue(DataType::Custom));
    // 可访问性选择框
    _accessibilityComboBox->addItem("Public", QVariant::fromValue(Accessibility::Public));
    _accessibilityComboBox->addItem("Private", QVariant::fromValue(Accessibility::Private));
    _accessibilityComboBox->addItem("Protected", QVariant::fromValue(Accessibility::Protected));


    // 设置表单字段
    formLayout->addRow("Member ID:", _idEdit);
    formLayout->addRow("Member Name:", _nameEdit);
    formLayout->addRow("Member Type:", _memberTypeComboBox);
    formLayout->addRow("Memory Bytes:", _memorySizeSpinBox);
    formLayout->addRow("Data Type:", _dataTypeComboBox);
    formLayout->addRow("Accessibility:", _accessibilityComboBox);

    // 创建和取消按钮
    QPushButton *createButton = new QPushButton("Create", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    formLayout->addRow(createButton, cancelButton);

    connect(_memberTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CreateMemberDialog::onMemberTypeChanged);
    connect(_dataTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CreateMemberDialog::onDataTypeChanged);
    connect(createButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);

    setLayout(mainLayout);
    setWindowTitle("Create New Class Member");

    onMemberTypeChanged();
    onDataTypeChanged();
}

ClassMember CreateMemberDialog::getClassMember() const {
    int id = _idEdit->text().toInt();
    QString name = _nameEdit->text();
    MemberType memberType = _memberTypeComboBox->currentData().value<MemberType>();
    int memorySize = _memorySizeSpinBox->value();
    DataType dataType = _dataTypeComboBox->currentData().value<DataType>();
    Accessibility accessibility = _accessibilityComboBox->currentData().value<Accessibility>();
    return ClassMember(id, name, memberType, memorySize, dataType, accessibility);
}


void CreateMemberDialog::onMemberTypeChanged() {
    MemberType memberType = _memberTypeComboBox->currentData().value<MemberType>();
    if (memberType == MemberType::Function) {
        _memorySizeSpinBox->setValue(0);
        _memorySizeSpinBox->setEnabled(false);
        _dataTypeComboBox->setCurrentIndex(_dataTypeComboBox->findData(QVariant::fromValue(DataType::Custom)));
        _dataTypeComboBox->setEnabled(false);
    } else {
        _memorySizeSpinBox->setEnabled(true);
        _dataTypeComboBox->setEnabled(true);
    }
}

void CreateMemberDialog::onDataTypeChanged() {
    if (_memberTypeComboBox->currentData().value<MemberType>() == MemberType::Data) {
        DataType dataType = _dataTypeComboBox->currentData().value<DataType>();
        if (dataType == DataType::Custom) {
            bool ok;
            int size = QInputDialog::getInt(this, "Input Size",
                                            "Enter size for selected data type:", 0, 0, 10000, 1, &ok);
            if (ok) {
                _memorySizeSpinBox->setValue(size);
            }
        } else if (dataType == DataType::String) {
            bool ok;
            std::string str = QInputDialog::getText(this, "Input String",
                                            "Enter string for selected data type:", QLineEdit::Normal, "", &ok).toStdString();
            if (ok) {
                _memorySizeSpinBox->setValue(sizeof(str));
            }
        } else {
            int size = getDataTypeSize(dataType);
            _memorySizeSpinBox->setValue(size);
        }
    }
}


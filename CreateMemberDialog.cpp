#include "CreateMemberDialog.h"
#include "Utils.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QInputDialog>
#include <string>

CreateMemberDialog::CreateMemberDialog(QWidget *parent, const QList<ClassMember> &members, const ClassMember &member, bool isModifyMode)
    : QDialog(parent), _members(members), _isModifyMode(isModifyMode) {

    // 移除问号
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QFormLayout *formLayout = new QFormLayout(this);

    _idEdit = new QLineEdit(this);
    _nameEdit = new QLineEdit(this);
    _memberTypeComboBox = new QComboBox(this);
    _memorySizeSpinBox = new QSpinBox(this);
    _dataTypeComboBox = new QComboBox(this);
    _accessibilityComboBox = new QComboBox(this);

    _idWarningLabel = new QLabel(this);
    _idWarningLabel->setStyleSheet("color: red;");
    _idWarningLabel->setVisible(false); // 初始时隐藏

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


    // 设置id框只能输入整数
    QIntValidator *idValidator = new QIntValidator(0, 99999, this);
    _idEdit->setValidator(idValidator);

    // 设置表单字段
    formLayout->addRow("Member ID:", _idEdit);
    formLayout->addRow("", _idWarningLabel);
    formLayout->addRow("Member Name:", _nameEdit);
    formLayout->addRow("Member Type:", _memberTypeComboBox);
    formLayout->addRow("Memory Bytes:", _memorySizeSpinBox);
    formLayout->addRow("Data Type:", _dataTypeComboBox);
    formLayout->addRow("Accessibility:", _accessibilityComboBox);

    if(isModifyMode == true) {
        _idEdit->setText(QString::number(member.memberId()));
        _nameEdit->setText(member.memberName());
        _memberTypeComboBox->setCurrentIndex(_memberTypeComboBox->findData(QVariant::fromValue(member.memberType())));
        _memorySizeSpinBox->setValue(member.memorySize());
        _dataTypeComboBox->setCurrentIndex(_dataTypeComboBox->findData(QVariant::fromValue(member.dataType())));
        _accessibilityComboBox->setCurrentIndex(_accessibilityComboBox->findData(QVariant::fromValue(member.accessibility())));
        _idEdit->setReadOnly(true);
    }


    // 创建和取消按钮
    _createButton = new QPushButton(isModifyMode ? "Modify" : "Create", this);
    formLayout->addRow(_createButton);

    connect(_memberTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CreateMemberDialog::onMemberTypeChanged);
    connect(_dataTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CreateMemberDialog::onDataTypeChanged);
    connect(_createButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(_idEdit, &QLineEdit::textChanged, this, &CreateMemberDialog::onIdChanged);
    if(!isModifyMode) onIdChanged("0");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);

    setLayout(mainLayout);
    setWindowTitle(isModifyMode ? "Modify" : "Create New" +  QString(" Class Member"));

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

void CreateMemberDialog::onIdChanged(const QString &text) {
    if (_isModifyMode) {
        return;  // 修改模式不进行检查
    }

    bool idExists = false;
    int id = text.toInt();
    for (const ClassMember &existingMember : _members) {
        if (existingMember.memberId() == id) {
            idExists = true;
            break;
        }
    }

    if (idExists) {
        _idWarningLabel->setText("ID already exists!");
        _idWarningLabel->setVisible(true);
        _createButton->setEnabled(false);
    } else if (id == 0) {
        _idWarningLabel->setText("ID can not be empty!");
        _idWarningLabel->setVisible(true);
        _createButton->setEnabled(false);
    } else {
        _idWarningLabel->setVisible(false);
        _createButton->setEnabled(true);
    }
}

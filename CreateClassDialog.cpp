#include "CreateClassDialog.h"
#include "CreateMemberDialog.h"
#include "QMessageBox"
#include "MemberDetailsDialog.h"

CreateClassDialog::CreateClassDialog(QWidget *parent, const QList<ClassInfo> &classes, const ClassInfo& classInfo, const bool isModifyMode)
    : QDialog(parent), _isModifyMode(isModifyMode), _classes(classes) {

    // 移除问号
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QFormLayout *formLayout = new QFormLayout(this);

    _idEdit = new QLineEdit(this);
    _nameEdit = new QLineEdit(this);
    _baseClassNameEdit = new QLineEdit(this);
    _functionEdit = new QTextEdit(this);
    _creationDateEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    _authorEdit = new QLineEdit(this);
    _membersListWidget = new QListWidget(this);

    _idWarningLabel = new QLabel(this);
    _idWarningLabel->setStyleSheet("color: red;");
    _idWarningLabel->setVisible(false); // 初始时隐藏
    _nameWarningLabel = new QLabel(this);
    _nameWarningLabel->setStyleSheet("color: red;");
    _nameWarningLabel->setVisible(false); // 初始时隐藏

    // 设置id框只能输入整数
    QIntValidator *idValidator = new QIntValidator(0, 99999, this);
    _idEdit->setValidator(idValidator);

    QFont font = _membersListWidget->font();
    font.setPointSize(14); // 18 太大了
    _membersListWidget->setFont(font);
    // 设置表单字段
    formLayout->addRow(tr("ID:"), _idEdit);
    formLayout->addRow("", _idWarningLabel); // 在ID字段下面添加警告标签
    formLayout->addRow(tr("Class Name:"), _nameEdit);
    formLayout->addRow("", _nameWarningLabel); // 在name字段下面添加警告标签
    formLayout->addRow(tr("Base Class Name:"), _baseClassNameEdit);
    formLayout->addRow(tr("Function:"), _functionEdit);
    formLayout->addRow(tr("Creation Date:"), _creationDateEdit);
    formLayout->addRow(tr("Author:"), _authorEdit);
    formLayout->addRow(tr("Members:"), _membersListWidget);

    if(isModifyMode == true) {
        _idEdit->setText(QString::number(classInfo.id()));
        _nameEdit->setText(classInfo.name());
        _baseClassNameEdit->setText(classInfo.baseClassName());
        _functionEdit->setPlainText(classInfo.function());
        _creationDateEdit->setDateTime(classInfo.creationDate());
        _authorEdit->setText(classInfo.author());
        _membersListWidget->clear();  // 清除现有内容
        for (const ClassMember &member : classInfo.members()) {
            _membersListWidget->addItem(member.memberName());
            _members.append(member);
        }
        _idEdit->setReadOnly(true);
    }

    // 添加成员按钮
    QPushButton *addMemberButton = new QPushButton(tr("Add Member"), this);
    formLayout->addRow(addMemberButton);
    connect(addMemberButton, &QPushButton::clicked, this, &CreateClassDialog::onAddMemberClicked);


    _createButton = new QPushButton(isModifyMode ? tr("Modify") : tr("Create") , this);
    formLayout->addRow(_createButton);

    // 将按钮的信号连接到对话框的槽函数
    connect(_createButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(_membersListWidget, &QListWidget::itemClicked, this, &CreateClassDialog::onMemberClicked);
    connect(_idEdit, &QLineEdit::textChanged, this, &CreateClassDialog::onIdChanged);
    connect(_nameEdit, &QLineEdit::textChanged, this, &CreateClassDialog::onNameChanged);
    if(!isModifyMode) onIdChanged("");
    onNameChanged(_nameEdit->text());

    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);  // 添加表单布局

    setLayout(mainLayout);

    setWindowTitle((isModifyMode ? tr("Modify") : tr("Create New")) + QString(tr(" Class")));
}


ClassInfo CreateClassDialog::getClassInfo() const {
    int id = _idEdit->text().toInt();
    QString name = _nameEdit->text();
    QString baseClassName = _baseClassNameEdit->text();
    QString function = _functionEdit->toPlainText();
    QDateTime creationDate = _creationDateEdit->dateTime();
    QString author = _authorEdit->text();

    return ClassInfo(id, name, baseClassName, function, creationDate, author, _members);
}

void CreateClassDialog::onAddMemberClicked() {

    CreateMemberDialog dialog(this, _members);
    if (dialog.exec() == QDialog::Accepted) {
        ClassMember newMember = dialog.getClassMember();
        _members.append(newMember);
        QMessageBox::information(this, tr("Member Created"), QString(tr("Member %1 created successfully!")).arg(newMember.memberName()));
        _membersListWidget->addItem(newMember.memberName());
    }

}


void CreateClassDialog::onMemberClicked(QListWidgetItem *item) {
    // 找到被点击的成员在列表中的索引
    int row = _membersListWidget->row(item);
    if (row >= 0 && row < _members.size()) {
        ClassMember member = _members.at(row);

        MemberDetailsDialog detailsDialog(member, this);
        connect(&detailsDialog, &MemberDetailsDialog::memberDeleted, [this, row]() {
            // 删除成员
            _members.removeAt(row);
            delete _membersListWidget->takeItem(row);  // 移除列表项
        });

        if (detailsDialog.exec() == QDialog::Accepted) {
            QMessageBox::information(this, tr("Member Deleted"), QString(tr("Member deleted successfully!")));
        }
    }
}


void CreateClassDialog::onIdChanged(const QString &text) {
    if (_isModifyMode) {
        return;  // 修改模式不进行检查
    }

    bool idExists = false;
    int id = text.toInt();
    for (const ClassInfo &existingClass : _classes) {
        if (existingClass.id() == id) {
            idExists = true;
            break;
        }
    }

    if (idExists) {
        _idWarningLabel->setText(tr("ID already exists!"));
        _idWarningLabel->setVisible(true);
        _createButton->setEnabled(false);
    } else if (id == 0) {
        _idWarningLabel->setText(tr("ID can not be empty!"));
        _idWarningLabel->setVisible(true);
        _createButton->setEnabled(false);
    } else {
        _idWarningLabel->setVisible(false);
        _createButton->setEnabled(true);
    }
}

void CreateClassDialog::onNameChanged(const QString &text) {
    if (text == "") {
        _nameWarningLabel->setText(tr("Name can not be empty!"));
        _nameWarningLabel->setVisible(true);
        _createButton->setEnabled(false);
    } else {
        _nameWarningLabel->setVisible(false);
        _createButton->setEnabled(true);
    }
}

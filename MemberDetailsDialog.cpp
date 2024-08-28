#include "MemberDetailsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MemberDetailsDialog::MemberDetailsDialog(const ClassMember &member, QWidget *parent)
    : QDialog(parent), _member(member) {

    _memberDetailsLabel = new QLabel(this);
    _deleteButton = new QPushButton("Delete", this);
    _returnButton = new QPushButton("Return", this);

    // 设定详细信息
    QString details = QString(tr("ID: %1\nName: %2\nType: %3\nMemory: %4 bytes\nDataType: %5\nAccessibility: %6"))
            .arg(member.memberId())
            .arg(member.memberName())
            .arg(member.memberType() == MemberType::Data ? "Data" : "Function")
            .arg(member.memorySize())
            .arg(member.dataType() == DataType::Int ? "Int" :
                 member.dataType() == DataType::Float ? "Float" :
                 member.dataType() == DataType::Double ? "Double" :
                 member.dataType() == DataType::Char ? "Char" :
                 member.dataType() == DataType::String ? "String" : "Custom")
            .arg(member.accessibility() == Accessibility::Public ? "Public" :
                 member.accessibility() == Accessibility::Private ? "Private" : "Protected");

    _memberDetailsLabel->setText(details);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(_memberDetailsLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(_deleteButton);
    buttonLayout->addWidget(_returnButton);

    mainLayout->addLayout(buttonLayout);

    connect(_deleteButton, &QPushButton::clicked, this, &MemberDetailsDialog::onDeleteButtonClicked);
    connect(_returnButton, &QPushButton::clicked, this, &MemberDetailsDialog::onReturnButtonClicked);

    setLayout(mainLayout);
    resize(400, 300);
    setWindowTitle(tr("Member Details"));
}

void MemberDetailsDialog::onDeleteButtonClicked() {
    emit memberDeleted();
    accept();
}

void MemberDetailsDialog::onReturnButtonClicked() {
    reject();
}

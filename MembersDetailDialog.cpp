#include "MembersDetailDialog.h"
#include "CreateMemberDialog.h"
#include <QHeaderView>
#include <QToolBar>
#include <QHeaderView>
#include <QMessageBox>

MembersDetailDialog::MembersDetailDialog(QList<ClassMember> &members, QWidget *parent)
    : QDialog(parent), _members(members) {

    // 移除问号
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QToolBar *toolBar = new QToolBar("new");
    QAction *addAction = new QAction("Add Member", this);
    addAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(addAction, &QAction::triggered, this, &MembersDetailDialog::onAddMember);
    toolBar->addAction(addAction);

    _tableView = new QTableView(this);
    _tableView->horizontalHeader()->setStretchLastSection(true);
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(_tableView, &QTableView::clicked, this, &MembersDetailDialog::onCellClicked);

    showMembers();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolBar);
    layout->addWidget(_tableView);

    setLayout(layout);
    setWindowTitle("Members Details");
    resize(900, 600);
}

void MembersDetailDialog::onAddMember() {
    CreateMemberDialog dialog(this, _members);
    if (dialog.exec() == QDialog::Accepted) {
        ClassMember newMember = dialog.getClassMember();
        _members.append(newMember);
        QMessageBox::information(this, "Member Created", QString("Member %1 created successfully!").arg(newMember.memberName()));
        showMembers();
    }
}

void MembersDetailDialog::showMembers() {
    _tableView->clearSpans();
    _model = new MemberModel(_members, this);
    _tableView->setModel(_model);
}


void MembersDetailDialog::onCellClicked(const QModelIndex &index) {
    if (!index.isValid())
        return;

    int column = index.column();
    int row = index.row();
    if(column == 0){
        QMessageBox msgBox;
        msgBox.setText("你想要进行什么操作？");
        QAbstractButton* modifyButton = msgBox.addButton("修改", QMessageBox::YesRole);
        QAbstractButton* deleteButton = msgBox.addButton("删除", QMessageBox::NoRole);
        msgBox.addButton("取消", QMessageBox::RejectRole);
        msgBox.exec();

        if(msgBox.clickedButton() == deleteButton) {
            _members.removeAt(row);
            showMembers();
        } else if(msgBox.clickedButton() == modifyButton) {
            CreateMemberDialog dialog(this, _members, _members.at(row), true);
            if (dialog.exec() == QDialog::Accepted) {
                ClassMember newMember = dialog.getClassMember();
                _members[row] = newMember;
                QMessageBox::information(this, "Member Modify", QString("Member %1 modified successfully!").arg(newMember.memberName()));
                showMembers();
            }
        }
    }
}

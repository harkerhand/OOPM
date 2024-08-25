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

    showMembers();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolBar);
    layout->addWidget(_tableView);

    setLayout(layout);
    setWindowTitle("Members Details");
    resize(900, 600);
}

void MembersDetailDialog::onAddMember() {
    CreateMemberDialog dialog(this);
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

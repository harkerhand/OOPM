#include "MembersDetailDialog.h"
#include "CreateMemberDialog.h"
#include <QHeaderView>
#include <QToolBar>
#include <QMessageBox>
#include "Utils.h"

MembersDetailDialog::MembersDetailDialog(QList<ClassMember> &members, QWidget *parent)
    : QDialog(parent), _members(members) {

    // 移除问号
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QToolBar *toolBar = new QToolBar("new");
    QAction *addAction = new QAction(tr("Add Member"), this);
    addAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(addAction, &QAction::triggered, this, &MembersDetailDialog::onAddMember);
    toolBar->addAction(addAction);

    _tableView = new QTableView(this);
    _tableView->horizontalHeader()->setStretchLastSection(true);
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(_tableView, &QTableView::clicked, this, &MembersDetailDialog::onCellClicked);


    _totalSizeLabel = new QLabel(tr("Total Memory Size: 0 bytes"), this); // 确保初始值


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolBar);
    layout->addWidget(_tableView);
    layout->addWidget(_totalSizeLabel);

    setLayout(layout);
    setWindowTitle(tr("Members Details"));
    setWindowIcon(QIcon(ICON_PATH));
    updateTotalSize(); // 初始化时更新总内存大小显示
    resize(900, 600);
    showMembers();
}

void MembersDetailDialog::onAddMember() {
    CreateMemberDialog dialog(this, _members);
    if (dialog.exec() == QDialog::Accepted) {
        ClassMember newMember = dialog.getClassMember();
        _members.append(newMember);
        QMessageBox::information(this, tr("Member Created"), QString(tr("Member %1 created successfully!")).arg(newMember.memberName()));
        showMembers();
    }
}

void MembersDetailDialog::showMembers() {
    _tableView->clearSpans();
    _model = new MemberModel(_members, this);
    _tableView->setModel(_model);
    updateTotalSize();
}


void MembersDetailDialog::onCellClicked(const QModelIndex &index) {
    if (!index.isValid())
        return;

    int column = index.column();
    int row = index.row();
    if(column == 0){
        QMessageBox msgBox;
        msgBox.setText(tr("What do you want to do?"));
        QAbstractButton* modifyButton = msgBox.addButton(tr("Modify"), QMessageBox::YesRole);
        QAbstractButton* deleteButton = msgBox.addButton(tr("Delete"), QMessageBox::NoRole);
        msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
        msgBox.exec();

        if(msgBox.clickedButton() == deleteButton) {
            _members.removeAt(row);
            showMembers();
        } else if(msgBox.clickedButton() == modifyButton) {
            CreateMemberDialog dialog(this, _members, _members.at(row), true);
            if (dialog.exec() == QDialog::Accepted) {
                ClassMember newMember = dialog.getClassMember();
                _members[row] = newMember;
                QMessageBox::information(this, tr("Member Modify"), QString(tr("Member %1 modified successfully!")).arg(newMember.memberName()));
                showMembers();
            }
        }
    }
}

void MembersDetailDialog::updateTotalSize() {
    int totalSize = 0;
    for (const auto &member : _members) {
        totalSize += member.memorySize(); // 返回该成员的内存大小
    }
    _totalSizeLabel->setText(tr("Total Memory Size: %1 bytes").arg(totalSize));
}

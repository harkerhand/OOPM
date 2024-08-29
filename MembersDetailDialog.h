#ifndef MEMBERSDETAILDIALOG_H
#define MEMBERSDETAILDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QList>
#include <QLabel>
#include "ClassMember.h"
#include "MemberModel.h"

class MembersDetailDialog : public QDialog {
    Q_OBJECT

public:
    explicit MembersDetailDialog(QList<ClassMember> &members, QWidget *parent = nullptr);
    void showMembers();

    QList<ClassMember> const getMembers() {
        return _members;
    }

private slots:
    void onAddMember();
    void onCellClicked(const QModelIndex &index);
    void updateTotalSize();

private:
    QList<ClassMember> _members;
    QTableView *_tableView;
    MemberModel *_model;
    QLabel *_totalSizeLabel;
};

#endif // MEMBERSDETAILDIALOG_H

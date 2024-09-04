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
    explicit MembersDetailDialog(QList<ClassMember> &members, QWidget *parent = nullptr); // 构造函数，初始化对话框并显示成员列表
    void showMembers(); // 显示成员列表

    QList<ClassMember> const getMembers() { return _members; } // 获取成员列表

private slots:
    void onAddMember(); // 处理添加成员操作
    void onCellClicked(const QModelIndex &index); // 处理表格单元格点击事件
    void updateTotalSize(); // 更新成员总大小标签

private:
    QList<ClassMember> _members; // 成员列表
    QTableView *_tableView; // 显示成员的表格视图
    MemberModel *_model; // 成员数据模型，用于管理表格数据
    QLabel *_totalSizeLabel; // 显示成员总大小的标签
};

#endif // MEMBERSDETAILDIALOG_H

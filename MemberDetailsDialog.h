#ifndef MEMBERDETAILSDIALOG_H
#define MEMBERDETAILSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "ClassMember.h"  // 确保包含您的 ClassMemberInfo 类定义

class MemberDetailsDialog : public QDialog {
    Q_OBJECT

public:
    explicit MemberDetailsDialog(const ClassMember &member, QWidget *parent = nullptr); // 构造函数，初始化对话框并显示成员详情

signals:
    void memberDeleted(); // 成员删除信号，删除操作后发出该信号

private slots:
    void onDeleteButtonClicked(); // 处理删除按钮点击事件
    void onReturnButtonClicked(); // 处理返回按钮点击事件

private:
    QLabel *_memberDetailsLabel; // 显示成员详细信息的标签
    QPushButton *_deleteButton; // 删除按钮
    QPushButton *_returnButton; // 返回按钮
    ClassMember _member; // 当前显示的成员信息
};

#endif // MEMBERDETAILSDIALOG_H

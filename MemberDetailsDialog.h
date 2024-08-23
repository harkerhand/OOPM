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
    explicit MemberDetailsDialog(const ClassMember &member, QWidget *parent = nullptr);

signals:
    void memberDeleted();

private slots:
    void onDeleteButtonClicked();
    void onReturnButtonClicked();

private:
    QLabel *_memberDetailsLabel;
    QPushButton *_deleteButton;
    QPushButton *_returnButton;
    ClassMember _member;
};

#endif // MEMBERDETAILSDIALOG_H

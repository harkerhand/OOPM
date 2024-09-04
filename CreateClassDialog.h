#ifndef CREATECLASSDIALOG_H
#define CREATECLASSDIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QListWidget>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include "ClassInfo.h"
#include "Enums.h"

class CreateClassDialog : public QDialog {
    Q_OBJECT

public:
    // 构造函数，初始化对话框，并设置类信息和模式
    CreateClassDialog(QWidget *parent = nullptr, const QList<ClassInfo> &classes = {}, const ClassInfo &classInfo = ClassInfo(), const bool isModifyMode = false);

    // 获取当前编辑的类信息
    ClassInfo getClassInfo() const;

private slots:
    // 添加成员按钮点击事件处理
    void onAddMemberClicked();
    // 成员列表项点击事件处理
    void onMemberClicked(QListWidgetItem *item);
    // 类ID变化事件处理
    void onIdChanged(const QString &text);
    // 类名称变化事件处理
    void onNameChanged(const QString &text);

private:
    QLineEdit *_idEdit; // 类ID输入框
    QLineEdit *_nameEdit; // 类名称输入框
    QLineEdit *_baseClassNameEdit; // 基类名称输入框
    QTextEdit *_functionEdit; // 类功能描述输入框
    QDateTimeEdit *_creationDateEdit; // 创建日期选择框
    QLineEdit *_authorEdit; // 作者输入框
    QListWidget *_membersListWidget; // 成员列表控件
    QLabel *_idWarningLabel; // 类ID警告标签
    QLabel *_nameWarningLabel; // 类名称警告标签
    QList<ClassMember> _members; // 类成员列表
    QPushButton *_createButton; // 创建按钮
    bool _isModifyMode; // 标记是否为修改模式
    const QList<ClassInfo> &_classes; // 所有类的信息列表
    bool isIdExists(); // 检查ID是否存在
};

#endif // CREATECLASSDIALOG_H

#ifndef CREATEMEMBERDIALOG_H
#define CREATEMEMBERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include "ClassMember.h"
#include "Enums.h"  // 包含您定义的枚举

class CreateMemberDialog : public QDialog {
    Q_OBJECT

public:
    explicit CreateMemberDialog(QWidget *parent = nullptr, const QList<ClassMember> &members = {}, const ClassMember &member = ClassMember(), bool isModifyMode = false); // 构造函数，初始化对话框，设置成员信息和模式
    ClassMember getClassMember() const; // 获取当前编辑的成员信息
private slots:
    void onMemberTypeChanged(); // 成员类型变化事件处理
    void onDataTypeChanged(); // 数据类型变化事件处理
    void onIdChanged(const QString &text); // 成员ID变化事件处理
    void onNameChanged(const QString &text); // 成员名称变化事件处理
private:
    QLineEdit *_idEdit; // 成员ID输入框
    QLineEdit *_nameEdit; // 成员名称输入框
    QComboBox *_memberTypeComboBox; // 成员类型下拉框
    QSpinBox *_memorySizeSpinBox; // 内存大小输入框
    QComboBox *_dataTypeComboBox; // 数据类型下拉框
    QComboBox *_accessibilityComboBox; // 可访问性下拉框
    const QList<ClassMember> &_members; // 所有成员的信息列表
    bool _isModifyMode; // 标记是否为修改模式
    QPushButton *_createButton; // 创建按钮
    QLabel *_idWarningLabel; // 成员ID警告标签
    QLabel *_nameWarningLabel; // 成员名称警告标签
    bool isIdExists(); // 检查ID是否存在
};

#endif // CREATEMEMBERDIALOG_H

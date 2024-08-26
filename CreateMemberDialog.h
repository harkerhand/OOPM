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
    explicit CreateMemberDialog(QWidget *parent = nullptr, const QList<ClassMember> &members = {}, const ClassMember &member = ClassMember(), bool isModifyMode = false);

    ClassMember getClassMember() const;

private slots:
    void onMemberTypeChanged();
    void onDataTypeChanged();
    void onIdChanged(const QString &text);
    void onNameChanged(const QString &text);
private:
    QLineEdit *_idEdit;
    QLineEdit *_nameEdit;
    QComboBox *_memberTypeComboBox;
    QSpinBox *_memorySizeSpinBox;
    QComboBox *_dataTypeComboBox;
    QComboBox *_accessibilityComboBox;
    const QList<ClassMember> &_members;
    bool _isModifyMode;
    QPushButton *_createButton;
    QLabel *_idWarningLabel;
    QLabel *_nameWarningLabel;
};

#endif // CREATEMEMBERDIALOG_H

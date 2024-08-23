#ifndef CREATEMEMBERDIALOG_H
#define CREATEMEMBERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include "ClassMember.h"
#include "Enums.h"  // 包含您定义的枚举

class CreateMemberDialog : public QDialog {
    Q_OBJECT

public:
    explicit CreateMemberDialog(QWidget *parent = nullptr);

    ClassMember getClassMember() const;

private slots:
    void onMemberTypeChanged(int index);
    void onDataTypeChanged(int index);
private:
    QLineEdit *_idEdit;
    QLineEdit *_nameEdit;
    QComboBox *_memberTypeComboBox;
    QSpinBox *_memorySizeSpinBox;
    QComboBox *_dataTypeComboBox;
    QComboBox *_accessibilityComboBox;
};

#endif // CREATEMEMBERDIALOG_H

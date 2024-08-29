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
    CreateClassDialog(QWidget *parent = nullptr, const QList<ClassInfo> &classes = {}, const ClassInfo &classInfo = ClassInfo(), const bool isModifyMode = false);

    ClassInfo getClassInfo() const;

private slots:
    void onAddMemberClicked();
    void onMemberClicked(QListWidgetItem *item);
    void onIdChanged(const QString &text);
    void onNameChanged(const QString &text);
private:
    QLineEdit *_idEdit;
    QLineEdit *_nameEdit;
    QLineEdit *_baseClassNameEdit;
    QTextEdit *_functionEdit;
    QDateTimeEdit *_creationDateEdit;
    QLineEdit *_authorEdit;
    QListWidget *_membersListWidget;
    QLabel *_idWarningLabel;
    QLabel *_nameWarningLabel;
    QList<ClassMember> _members;
    QPushButton *_createButton;
    bool _isModifyMode;
    const QList<ClassInfo> &_classes;
    bool isIdExists();
};


#endif // CREATECLASSDIALOG_H

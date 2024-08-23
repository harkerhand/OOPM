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
#include "ClassInfo.h"
#include "Enums.h"

class CreateClassDialog : public QDialog {
    Q_OBJECT

public:
    CreateClassDialog(QWidget *parent = nullptr);

    ClassInfo getClassInfo() const;

private slots:
    void onAddMemberClicked();
    void onMemberClicked(QListWidgetItem *item);

private:
    QLineEdit *_idEdit;
    QLineEdit *_nameEdit;
    QLineEdit *_baseClassNameEdit;
    QTextEdit *_functionEdit;
    QDateTimeEdit *_creationDateEdit;
    QLineEdit *_authorEdit;
    QListWidget *_membersListWidget;
    QList<ClassMember> _members;
};


#endif // CREATECLASSDIALOG_H

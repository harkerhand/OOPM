#ifndef USERMANAGEMENTWINDOW_H
#define USERMANAGEMENTWINDOW_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QStandardItemModel>
#include "UserModel.h"

class UserManagementWindow : public QWidget {
    Q_OBJECT

public:
    UserManagementWindow(QWidget *parent = nullptr);

private slots:
    void onAddUser();
    void onDeleteUser();
    void saveUserData();
private:
    void setupUI();
    void loadUserData();
    void showUsers();

    QTableView *_tableView;
    QPushButton *_addButton;
    QPushButton *_deleteButton;
    QPushButton *_saveButton;
    QLineEdit *_usernameEdit;
    QLineEdit *_passwordEdit;
    QList<QPair<QString, QString>> _users;
};

#endif // USERMANAGEMENTWINDOW_H

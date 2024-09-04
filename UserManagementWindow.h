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
    void onAddUser(); // 添加用户槽函数
    void onDeleteUser(); // 删除用户槽函数
    void saveUserData(); // 保存用户数据槽函数
    void onBackToLogin(); // 返回登录界面槽函数

private:
    void setupUI(); // 设置用户界面
    void loadUserData(); // 加载用户数据
    void showUsers(); // 显示用户列表

    QTableView *_tableView; // 用户表格视图
    QPushButton *_addButton; // 添加用户按钮
    QPushButton *_deleteButton; // 删除用户按钮
    QPushButton *_saveButton; // 保存用户数据按钮
    QLineEdit *_usernameEdit; // 用户名输入框
    QLineEdit *_passwordEdit; // 密码输入框
    QPushButton *_backToLogin; // 返回登录界面按钮
    QList<QPair<QString, QString>> _users; // 存储用户信息的列表（用户名和密码的键值对）

};

#endif // USERMANAGEMENTWINDOW_H

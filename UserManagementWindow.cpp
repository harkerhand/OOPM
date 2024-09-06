#include "UserManagementWindow.h"
#include "Utils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include "UserModel.h"
#include <QHeaderView>
#include "LoginWindow.h"

UserManagementWindow::UserManagementWindow(QWidget *parent)
    : QWidget(parent) {
    setupUI();
    loadUserData(); // Load existing user data if any
}

void UserManagementWindow::setupUI() {
    _addButton = new QPushButton(tr("Add User"), this);
    _deleteButton = new QPushButton(tr("Delete User"), this);
    _saveButton = new QPushButton(tr("Save Data"), this);
    _usernameEdit = new QLineEdit(this);
    _passwordEdit = new QLineEdit(this);
    _backToLogin = new QPushButton(tr("Back Login"), this);

    QRegularExpression regex("[A-Za-z0-9!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]+");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
    _usernameEdit->setValidator(validator);
    _passwordEdit->setValidator(validator);

    _tableView = new QTableView(this);
    _tableView->horizontalHeader()->setStretchLastSection(true);
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    usernameLayout->addWidget(new QLabel(tr("Username:")));
    usernameLayout->addWidget(_usernameEdit);
    passwordLayout->addWidget(new QLabel(tr("Password:")));
    passwordLayout->addWidget(_passwordEdit);

    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addWidget(_tableView);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(_addButton);
    buttonLayout->addWidget(_deleteButton);
    buttonLayout->addWidget(_saveButton);
    buttonLayout->addWidget(_backToLogin);
    mainLayout->addLayout(buttonLayout);

    connect(_addButton, &QPushButton::clicked, this, &UserManagementWindow::onAddUser);
    connect(_deleteButton, &QPushButton::clicked, this, &UserManagementWindow::onDeleteUser);
    connect(_saveButton, &QPushButton::clicked, this, &UserManagementWindow::saveUserData);
    connect(_backToLogin, &QPushButton::clicked, this, &UserManagementWindow::onBackToLogin);
    setWindowTitle(tr("User Manager"));
    setWindowIcon(QIcon(ICON_PATH));
}

void UserManagementWindow::showUsers() {
    _tableView->clearSpans();
    UserModel *model = new UserModel(_users, this);
    _tableView->setModel(model);
}


void UserManagementWindow::loadUserData() {
    QFile file(USER_FILE_PATH);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        while (!stream.atEnd()) {
            QString username;
            QString hashedPassword;
            stream >> username >> hashedPassword;
            _users.append(qMakePair(username, hashedPassword));
        }
        file.close();
        showUsers();
    } else {
        qDebug() << "Unable to open file for reading.";
    }
}

void UserManagementWindow::saveUserData() {
    QFile file(USER_FILE_PATH);
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
    }
    for (const auto &user : _users) {
        QString username = user.first;
        QString password = user.second;
        saveUserAccount(username, password);
     }
    QMessageBox::information(this, tr("Success"), tr("Save successfully!"));
}

void UserManagementWindow::onAddUser() {
    QString username = _usernameEdit->text();
    QString password = _passwordEdit->text();
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, tr("Add User"), tr("Username and password cannot be empty."));
        return;
    }
    if(_passwordEdit->text().length() < 6) {
        QMessageBox::warning(this, tr("Add User"), tr("Password cannot be less than six digits."));
        return;
    }
    _users.append(qMakePair(username, password));
    _usernameEdit->setText("");
    _passwordEdit->setText("");
    showUsers();
}

void UserManagementWindow::onDeleteUser() {
    QModelIndexList selectedRows = _tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, tr("Delete User"), tr("No user selected."));
        return;
    }
    for (int i = selectedRows.size() - 1; i >= 0; --i) {
        QModelIndex index = selectedRows.at(i);
        _users.removeAt(index.row());
    }
    showUsers();
}

void UserManagementWindow::onBackToLogin() {
    saveUserData();
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}

#include "LoginWindow.h"
#include "MainWindow.h"
#include "Utils.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include <QPainter>
#include <QPixmap>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include "UserManagementWindow.h"
#include <QRegularExpressionValidator>
#include <QRegularExpression>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent), _translator(new QTranslator(this)) {
    setupUI();
}

void LoginWindow::setupUI() {
    _titleLabel = new QLabel(tr("Object-Oriented Project Management"));
    _titleLabel->setAlignment(Qt::AlignCenter);
    _titleLabel->setStyleSheet("font-size: 60px; font-weight: bold; background-color: transparent;"); // Larger, bold font

    _usernameLabel = new QLabel(tr("Username:"));
    _passwordLabel = new QLabel(tr("Password:"));


    _usernameLineEdit = new QLineEdit();
    _passwordLineEdit = new QLineEdit();
    _passwordLineEdit->setEchoMode(QLineEdit::Password);
    QRegularExpression regex("[A-Za-z0-9!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]+");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
    _usernameLineEdit->setValidator(validator);
    _passwordLineEdit->setValidator(validator);

    _loginButton = new QPushButton(tr("Login"));
    _loginButton->setStyleSheet(
        "QPushButton {"
        "    border: 2px solid rgba(219, 125, 100);"
        "    border-radius: 5px;"
        "    padding: 5px 10px;"
        "}"
        "QPushButton:hover {"
        "    border: 2px solid blue;"  // 悬停时边框变为蓝色
        "}"
        "QPushButton:pressed {"
        "    border: 2px solid green;" // 按下时边框变为绿色
        "}"
    );
    connect(_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);

    _languageComboBox = new QComboBox();
    _languageComboBox->addItem(tr("English"));
    _languageComboBox->addItem(tr("中文"));
    connect(_languageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoginWindow::onLanguageChanged);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("background-color: transparent;");

    // Create the login section layout
    QVBoxLayout *loginLayout = new QVBoxLayout();
    loginLayout->addWidget(_usernameLabel);
    loginLayout->addWidget(_usernameLineEdit);
    loginLayout->addWidget(_passwordLabel);
    loginLayout->addWidget(_passwordLineEdit);
    loginLayout->addWidget(_loginButton);
    loginLayout->setAlignment(Qt::AlignCenter);

    // Create language buttons
    _englishButton = new QPushButton(tr("English"));
    _chineseButton = new QPushButton(tr("中文"));
    _lightThemeButton = new QPushButton(tr("Light"));
    _darkThemeButton = new QPushButton(tr("Dark"));

    // Connect language buttons to the slot
    connect(_englishButton, &QPushButton::clicked, this, [this](){ onLanguageChanged(0); });
    connect(_chineseButton, &QPushButton::clicked, this, [this](){ onLanguageChanged(1); });
    connect(_lightThemeButton, &QPushButton::clicked, this, [this](){ onThemeChanged(0); });
    connect(_darkThemeButton, &QPushButton::clicked, this, [this](){ onThemeChanged(1); });


    QWidget *loginContainer = new QWidget();
    loginContainer->setLayout(loginLayout);

    // Create the main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Add spacer on the left
    mainLayout->addStretch();

    // Add the login container in the middle
    mainLayout->addWidget(loginContainer);

    // Add spacer on the right
    mainLayout->addStretch();

    // Create a vertical layout for the bottom section
    // Create a horizontal layout for the language buttons
    QHBoxLayout *languageLayout = new QHBoxLayout();
    languageLayout->addWidget(_englishButton);
    languageLayout->addWidget(_chineseButton);
    languageLayout->addStretch(); // Add stretchable space to push buttons to the right

    QWidget *langWidget = new QWidget();
    langWidget->setLayout(languageLayout);
    langWidget->setStyleSheet("background-color: transparent;");

    QHBoxLayout *themeLayout = new QHBoxLayout();
    themeLayout->addWidget(_lightThemeButton);
    themeLayout->addWidget(_darkThemeButton);
    themeLayout->addStretch();

    QWidget *themeWidget = new QWidget();
    themeWidget->setLayout(themeLayout);
    themeWidget->setStyleSheet("background-color: transparent;");

    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(themeWidget, 0, Qt::AlignBottom | Qt::AlignRight);
    bottomLayout->addWidget(langWidget, 0, Qt::AlignBottom | Qt::AlignRight);

    QWidget *bottomWidget = new QWidget();
    bottomWidget->setLayout(bottomLayout);
    bottomWidget->setStyleSheet("background-color: transparent;");

    // Create the main layout for the window
    QVBoxLayout *windowLayout = new QVBoxLayout();
    windowLayout->addWidget(_titleLabel);
    windowLayout->addWidget(centralWidget);
    windowLayout->addWidget(bottomWidget);

    setLayout(windowLayout);

    setWindowTitle(tr("Login"));
    setFixedSize(1280, 720);

    // Center the window on the screen
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screenRect = desktop->screenGeometry();
    move((screenRect.width() - width()) / 2, (screenRect.height() - height()) / 2);
    onLanguageChanged(0);
    onThemeChanged(1);
}



void LoginWindow::onLoginButtonClicked() {
    QString username = _usernameLineEdit->text();
    QString password = _passwordLineEdit->text();


    UserType userType = verifyAccount(username, password);
    switch (userType) {
    case UserType::Admin: {
        // 管理员登录成功
        qDebug() << "管理员登录成功";
        QMessageBox::information(this, tr("Success"), tr("Welcome Administor!"));
        UserManagementWindow *userManagementWindow = new UserManagementWindow();
        userManagementWindow->show();

        this->close();
        break;
    }
    case UserType::User: {
        QMessageBox::information(this, tr("Success"), tr("Welcome User!"));
        MainWindow *mainwindow = new MainWindow();
        mainwindow->show();
        this->close();
        break;
    }
    case UserType::Invalid:
        // 登录失败
        QMessageBox::information(this, tr("Error"), tr("Username or password incorrect!"));
        break;
    }
    _usernameLineEdit->setText("");
    _passwordLineEdit->setText("");


}

void LoginWindow::onLanguageChanged(int index) {
    if (index == 1) { // 中文选择

        if(_translator.load(":/langs/OOPM_zh_CN.qm"))
        {
            qApp->installTranslator(&_translator);
        }
        _chineseButton->setStyleSheet("color: blue;"); // Highlight English button
        _englishButton->setStyleSheet("color: white;");
    } else { // 英文选择
        // Clear existing translator
        if(_translator.load(":/langs/OOPM_en_US.qm"))
        {
            qApp->installTranslator(&_translator);
        }
        _englishButton->setStyleSheet("color: blue;"); // Highlight English button
        _chineseButton->setStyleSheet("color: white;");
    }

    updateUI();
}

void LoginWindow::updateUI() {
    // Update the text on the UI elements to reflect the selected language
    _titleLabel->setText(tr("Object-Oriented Project Management"));
    _usernameLabel->setText(tr("Username:"));
    _passwordLabel->setText(tr("Password:"));
    _loginButton->setText(tr("Login"));
    _lightThemeButton->setText(tr("Light"));
    _darkThemeButton->setText(tr("Dark"));
}

void LoginWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if(_nowTheme)
        painter.drawPixmap(0, 0, width(), height(),QPixmap(":/imgs/darkBg.png"));
    else painter.drawPixmap(0, 0, width(), height(),QPixmap(":/imgs/lightBg.png"));
    QWidget::paintEvent(event); // Call base class implementation if needed
}


void LoginWindow::onThemeChanged(int index) {
    if (index == 0) {
        QFile file(":/styles/lightstyle.qss");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // 打印错误信息到标准错误输出
            qWarning() << "Failed to open file:" << file.errorString();
        }

        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        qApp->setStyleSheet(styleSheet);
        _lightThemeButton->setStyleSheet("color: blue;");
        _darkThemeButton->setStyleSheet("color: white;");
    } else {
        QFile file(":/styles/darkstyle.qss");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // 打印错误信息到标准错误输出
            qWarning() << "Failed to open file:" << file.errorString();
        }
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        qApp->setStyleSheet(styleSheet);
        _darkThemeButton->setStyleSheet("color: blue;");
        _lightThemeButton->setStyleSheet("color: white;");
    }
    _nowTheme = index;
}

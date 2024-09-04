#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QTranslator>

class QLineEdit;
class QPushButton;
class QLabel;
class QComboBox;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);

private slots:
    void onLoginButtonClicked(); // 处理登录按钮点击事件
    void onLanguageChanged(int index); // 处理语言选择变化事件
    void onThemeChanged(int index);  // 处理主题切换事件

private:
    void setupUI(); // 设置和初始化用户界面
    void updateUI(); // 更新用户界面，包括语言和主题的变化
    void paintEvent(QPaintEvent *event) override; // 重写的绘制事件，用于自定义背景渲染

    QLineEdit *_usernameLineEdit; // 用户名输入框
    QLineEdit *_passwordLineEdit; // 密码输入框
    QPushButton *_loginButton; // 登录按钮
    QLabel *_titleLabel; // 窗口标题标签
    QLabel *_usernameLabel; // 用户名标签
    QLabel *_passwordLabel; // 密码标签
    QPushButton *_chineseButton; // 中文按钮
    QPushButton *_englishButton; // 英文按钮
    QTranslator _translator; // 翻译器，用于处理多语言支持
    QPushButton *_lightThemeButton; // 轻主题按钮
    QPushButton *_darkThemeButton; // 暗主题按钮
    bool _nowTheme; // 当前主题状态（`true` 表示暗主题，`false` 表示轻主题）

};

#endif // LOGINWINDOW_H

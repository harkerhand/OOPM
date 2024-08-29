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
    void onLoginButtonClicked();
    void onLanguageChanged(int index);
    void onThemeChanged(int index);

private:
    void setupUI();
    void loadTranslations();
    void updateUI();
    void paintEvent(QPaintEvent *event) override;

    QLineEdit *_usernameLineEdit;
    QLineEdit *_passwordLineEdit;
    QPushButton *_loginButton;
    QLabel *_titleLabel;
    QLabel *_usernameLabel;
    QLabel *_passwordLabel;
    QComboBox *_languageComboBox;
    QPushButton *_chineseButton;
    QPushButton *_englishButton;
    QTranslator _translator;
    QPushButton *_lightThemeButton;
    QPushButton *_darkThemeButton;
    bool _nowTheme;
};

#endif // LOGINWINDOW_H

#include "Utils.h"
#include <QCryptographicHash>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>

QString DOCUMENT_PATH = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
QString USER_FILE_PATH = DOCUMENT_PATH + "/OOPM/passports/users.dat";
QString ICON_PATH = ":/imgs/icon.png";

int getDataTypeSize(DataType dataType) {
    switch (dataType) {
        case DataType::Int:
            return sizeof(int);
        case DataType::Float:
            return sizeof(float);
        case DataType::Double:
            return sizeof(double);
        case DataType::Char:
            return sizeof(char);
        case DataType::String:
            return 0; // 字符串的字节数依赖于实际内容
        case DataType::Custom:
            return 0; // 自定义类型的字节数需要用户定义
        default:
            return 0; // 默认返回 0
    }
}

QString hashPassword(const QString &password) {
    if(password.length() == 64) return password;
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    return hash.toHex(); // 将哈希值转换为十六进制字符串
}

void saveAdminAccount(const QString &username, const QString &password) {
    QString filePath;
    QString hashedPassword = hashPassword(password);

    QFile file(filePath);
    QFileInfo fileInfo(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        printf("001");
        return;
    }
    QDataStream stream(&file);
    stream << username << hashedPassword;
    file.close();
}

void saveUserAccount(const QString &username, const QString &password) {

    QString hashedPassword = hashPassword(password);
    QFile file(USER_FILE_PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        return;
    }
    QDataStream stream(&file);
    stream << username << hashedPassword;
    file.close();
}


UserType verifyAccount(const QString &username, const QString &password) {
    qDebug() << USER_FILE_PATH;
    const QString adminFilePath = ":/passports/admin.dat";
    QString hashedPassword = hashPassword(password);

    // 验证管理员账号
    QFile adminFile(adminFilePath);
    if (!adminFile.open(QIODevice::ReadOnly)) {
        qDebug() << adminFilePath;
        return UserType::Invalid;
    } else {
        QDataStream stream(&adminFile);
        QString fileUsername, fileHash;
        while (!stream.atEnd()) {
            stream >> fileUsername >> fileHash;
            if (fileUsername == username && fileHash == hashedPassword) {
                adminFile.close();
                return UserType::Admin; // 管理员账号和密码匹配
            }
        }
    }
    adminFile.close();

    // 验证用户账号
    QFile userFile(USER_FILE_PATH);
    if (!userFile.open(QIODevice::ReadOnly)) {
        printf("004");
        return UserType::Invalid;
    } else {
        QDataStream stream(&userFile);
        QString fileUsername, fileHash;
        while (!stream.atEnd()) {
            stream >> fileUsername >> fileHash;
            if (fileUsername == username && fileHash == hashedPassword) {
                userFile.close();
                return UserType::User; // 用户账号和密码匹配
            }
        }
        userFile.close();
    }

    return UserType::Invalid; // 账号或密码错误
}

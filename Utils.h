#ifndef UTILS_H
#define UTILS_H
#include <QTranslator>
#include "Enums.h"
extern QString DOCUMENT_PATH;
extern QString USER_FILE_PATH;
extern QString ICON_PATH;
extern QTranslator translator;

int getDataTypeSize(DataType dataType);
QString hashPassword(const QString &password);
void saveAdminAccount(const QString &username, const QString &password);
UserType verifyAccount(const QString &username, const QString &password);
void saveUserAccount(const QString &username, const QString &password);
#endif // UTILS_H

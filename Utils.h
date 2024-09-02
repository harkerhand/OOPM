#ifndef UTILS_H
#define UTILS_H

#include "Enums.h"
extern QString USER_FILE_PATH;

int getDataTypeSize(DataType dataType);
QString hashPassword(const QString &password);
void saveAdminAccount(const QString &username, const QString &password);
UserType verifyAccount(const QString &username, const QString &password);
void saveUserAccount(const QString &username, const QString &password);
#endif // UTILS_H

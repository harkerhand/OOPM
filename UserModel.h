#ifndef USERMODEL_H
#define USERMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>

class UserModel : public QAbstractTableModel {
    Q_OBJECT

public:
    UserModel(const QList<QPair<QString, QString>> &users, QObject *parent = nullptr)
        : QAbstractTableModel(parent), _users(users) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return _users.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return 2;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

        const auto &item = _users[index.row()];
        if (index.column() == 0)
            return item.first; // Username
        else if (index.column() == 1)
            return item.second; // Hashed password

        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0: return tr("Username");
                case 1: return tr("Password");
                default: return QVariant();
            }
        }
        return QVariant();
    }


private:
    QList<QPair<QString, QString>> _users;
};

#endif

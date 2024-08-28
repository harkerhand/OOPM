#ifndef MEMBERMODEL_H
#define MEMBERMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "ClassMember.h"
#include "Enums.h"

class MemberModel : public QAbstractTableModel {
    Q_OBJECT

public:
    MemberModel(const QList<ClassMember> &members, QObject *parent = nullptr)
        : QAbstractTableModel(parent), _members(members) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return _members.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return 6;  // Number of columns
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

        const ClassMember &classMember = _members.at(index.row());
        switch (index.column()) {
            case 0: return classMember.memberId();
            case 1: return classMember.memberName();
            case 2: return memberTypeToString(classMember.memberType());
            case 3: return classMember.memorySize();
            case 4: return dataTypeToString(classMember.dataType());
            case 5: return accessibilityToString(classMember.accessibility());
            default: return QVariant();
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0: return tr("ID");
                case 1: return tr("Member Name");
                case 2: return tr("Member Type");
                case 3: return tr("Memory Size");
                case 4: return tr("Data Type");
                case 5: return tr("Accessibility");
                default: return QVariant();
            }
        }
        return QVariant();
    }

private:
    QList<ClassMember> _members;
};

#endif // MEMBERMODEL_H

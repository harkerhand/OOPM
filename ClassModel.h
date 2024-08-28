#ifndef CLASSMODEL_H
#define CLASSMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "ClassInfo.h"

class ClassModel : public QAbstractTableModel {
    Q_OBJECT

public:
    ClassModel(const QList<ClassInfo> &classes, QObject *parent = nullptr)
        : QAbstractTableModel(parent), _classes(classes) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return _classes.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return 7;  // Number of columns
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

        const ClassInfo &classInfo = _classes.at(index.row());
        switch (index.column()) {
        case 0: return classInfo.id();
        case 1: return classInfo.name();
        case 2: return classInfo.baseClassName();
        case 3: return classInfo.function();
        case 4: return classInfo.creationDate().toString("yyyy-MM-dd");
        case 5: return classInfo.author();
        case 6: return QString::number(classInfo.members().size()) + "个";
        default: return QVariant();
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0: return tr("ID");
                case 1: return tr("Class Name");
                case 2: return tr("Base Class");
                case 3: return tr("Function");
                case 4: return tr("Creation Date");
                case 5: return tr("Author");
                case 6: return tr("Members");
                default: return QVariant();
            }
        }
        return QVariant();
    }

private:
    QList<ClassInfo> _classes;
};


#endif // CLASSMODEL_H

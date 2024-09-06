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

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override {
        // 使用 std::sort 对 _classes 进行排序
        std::sort(_classes.begin(), _classes.end(), [column, order](const ClassInfo &a, const ClassInfo &b) {
            switch (column) {
                case 0:  // 根据 ID 排序
                    return order == Qt::AscendingOrder ? a.id() < b.id() : a.id() > b.id();
                case 1:  // 根据 Class Name 排序
                    return order == Qt::AscendingOrder ? a.name() < b.name() : a.name() > b.name();
                case 2:  // 根据 Base Class Name 排序
                    return order == Qt::AscendingOrder ? a.baseClassName() < b.baseClassName() : a.baseClassName() > b.baseClassName();
                case 3:  // 根据 Function 排序
                    return order == Qt::AscendingOrder ? a.function() < b.function() : a.function() > b.function();
                case 4:  // 根据 Creation Date 排序
                    return order == Qt::AscendingOrder ? a.creationDate() < b.creationDate() : a.creationDate() > b.creationDate();
                case 5:  // 根据 Author 排序
                    return order == Qt::AscendingOrder ? a.author() < b.author() : a.author() > b.author();
                case 6:  // 根据成员数量排序
                    return order == Qt::AscendingOrder ? a.members().size() < b.members().size() : a.members().size() > b.members().size();
                default:
                    return false;
            }
        });

        // 通知视图数据已更改
        emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    }




private:
    QList<ClassInfo> _classes;
};


#endif // CLASSMODEL_H

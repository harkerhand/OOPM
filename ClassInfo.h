#ifndef CLASSINFO_H
#define CLASSINFO_H

#include<QString>
#include<QDateTime>
#include<QList>
#include "ClassMember.h"
class ClassInfo{
private:
    int _id; // 编号
    QString _name; // 类名
    QString _baseClassName; // 基类名
    QString _function; // 功能
    QDateTime _creationDate; //创建时间
    QString _author;
    QList<ClassMember> _members;

public:
    ClassInfo();
    ClassInfo(int id, const QString& name, const QString& baseClassName, const QString& function,
              const QDateTime& creationDate, const QString& author, const QList<ClassMember>& members = QList<ClassMember>());

    void addMember(const ClassMember& member);
    void removeMember(int memberId);

    int calculateMemorySize() const;

    // 序列化与反序列化方法
    void saveToStream(QDataStream& out) const;
    void loadFromStream(QDataStream& in);

    // Getter 方法
    int id() const { return _id; }
    QString name() const { return _name; }
    QString baseClassName() const { return _baseClassName; }
    QString function() const { return _function; }
    QDateTime creationDate() const { return _creationDate; }
    QString author() const { return _author; }
    QList<ClassMember> members() const { return _members; }
};




#endif // CLASSINFO_H

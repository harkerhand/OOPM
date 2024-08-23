#include "ClassInfo.h"

ClassInfo::ClassInfo()
    : _id(0), _name(""), _baseClassName(""), _function(""),
      _creationDate(QDateTime()), _author("") {}

ClassInfo::ClassInfo(int id, const QString& name, const QString& baseClassName, const QString& function,
                     const QDateTime& creationDate, const QString& author, const QList<ClassMember>& members)
    : _id(id), _name(name), _baseClassName(baseClassName), _function(function),
      _creationDate(creationDate), _author(author), _members(members) {}

void ClassInfo::addMember(const ClassMember& member) {
    _members.append(member);
}

void ClassInfo::removeMember(int memberId) {
    for (int i = 0; i < _members.size(); ++i) {
        if (_members[i].memberId() == memberId) {  // 使用 getter 方法
            _members.removeAt(i);
            return;
        }
    }
}

int ClassInfo::calculateMemorySize() const {
    int totalSize = 0;
    for (const ClassMember& member : _members) {
        if (member.memberType() == MemberType::Data) {  // 使用 getter 方法
            totalSize += member.memorySize();  // 使用 getter 方法
        }
    }
    return totalSize;
}

void ClassInfo::saveToStream(QDataStream& out) const {
    out << _id << _name << _baseClassName << _function << _creationDate << _author;
    out << _members.size();
    for (const ClassMember& member : _members) {
        member.saveToStream(out);
    }
}

void ClassInfo::loadFromStream(QDataStream& in) {
    in >> _id >> _name >> _baseClassName >> _function >> _creationDate >> _author;
    int membersCount;
    in >> membersCount;
    _members.clear();
    for (int i = 0; i < membersCount; ++i) {
        ClassMember member;
        member.loadFromStream(in);
        _members.append(member);
    }
}

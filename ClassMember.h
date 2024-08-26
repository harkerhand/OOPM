#ifndef CLASSMEMBER_H
#define CLASSMEMBER_H

#include <QString>
#include "Enums.h"
#include <QDataStream>
// 数据成员
class ClassMember {
public:
    ClassMember();
    ClassMember(int memberId, const QString& memberName, MemberType memberType,
                    int memorySize, DataType dataType, Accessibility accessibility);

    // 序列化与反序列化方法
    void saveToStream(QDataStream& out) const;
    void loadFromStream(QDataStream& in);

    int memberId() const { return _memberId; }
    QString memberName() const { return _memberName; }
    MemberType memberType() const { return _memberType; }
    int memorySize() const { return _memorySize; }
    DataType dataType() const { return _dataType; }
    Accessibility accessibility() const { return _accessibility; }
    void setMemberId(int id) { _memberId = id; }
    void setMemberName(const QString &name) { _memberName = name; }
    void setMemberType(const MemberType &memberType) { _memberType = memberType; }
    void setMemorySize(const int &size) { _memorySize = size; }
    void setDataType(const DataType &dataType) { _dataType = dataType; }
    void setAccessibility(const Accessibility &access) {_accessibility = access; }

private:
    int _memberId; // 成员编号
    QString _memberName; // 成员名称
    MemberType _memberType; // 成员类型
    int _memorySize; // 内存字节数 仅在成员类型为 Data 时有效
    DataType _dataType; // 数据类型 仅在成员类型为 Data 时有效
    Accessibility _accessibility; // 可访问性
};



#endif // CLASSMEMBER_H

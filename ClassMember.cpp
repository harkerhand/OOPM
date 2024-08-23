#include "ClassMember.h"

ClassMember::ClassMember()
    : _memberId(0), _memberName(""), _memberType(MemberType::Data),
      _memorySize(0), _dataType(DataType::Int), _accessibility(Accessibility::Public) {}

ClassMember::ClassMember(int memberId, const QString& memberName, MemberType memberType,
                         int memorySize, DataType dataType, Accessibility accessibility)
    : _memberId(memberId), _memberName(memberName), _memberType(memberType),
      _memorySize(memorySize), _dataType(dataType), _accessibility(accessibility) {
    if(memberType == MemberType::Function) {
        _memorySize = 0;
        _dataType = DataType::Custom;
    }
}

void ClassMember::saveToStream(QDataStream& out) const {
    out << _memberId << _memberName << static_cast<int>(_memberType)
        << _memorySize << static_cast<int>(_dataType) << static_cast<int>(_accessibility);
}

void ClassMember::loadFromStream(QDataStream& in) {
    int memberType, dataType, accessibility;
    in >> _memberId >> _memberName >> memberType >> _memorySize >> dataType >> accessibility;
    _memberType = static_cast<MemberType>(memberType);
    _dataType = static_cast<DataType>(dataType);
    _accessibility = static_cast<Accessibility>(accessibility);
}

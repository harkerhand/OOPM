#ifndef ENUMS_H
#define ENUMS_H

#include <QMetaType>

enum class MemberType {
    Data,
    Function
};


enum class Accessibility {
    Public,
    Private,
    Protected
};

enum class DataType {
    Int,
    Float,
    Double,
    Char,
    String,
    Custom // 用于表示自定义的数据类型
};


Q_DECLARE_METATYPE(MemberType)
Q_DECLARE_METATYPE(Accessibility)
Q_DECLARE_METATYPE(DataType)


#endif // ENUMS_H

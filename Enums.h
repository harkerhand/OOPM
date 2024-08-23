#ifndef ENUMS_H
#define ENUMS_H

#include <QMetaType>
#include <QString>

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

// 返回MemberType的字符串表示
inline QString memberTypeToString(MemberType type) {
    switch (type) {
        case MemberType::Data: return "Data";
        case MemberType::Function: return "Function";
        default: return "Unknown";
    }
}

// 返回Accessibility的字符串表示
inline QString accessibilityToString(Accessibility access) {
    switch (access) {
        case Accessibility::Public: return "Public";
        case Accessibility::Private: return "Private";
        case Accessibility::Protected: return "Protected";
        default: return "Unknown";
    }
}

// 返回DataType的字符串表示
inline QString dataTypeToString(DataType type) {
    switch (type) {
        case DataType::Int: return "Int";
        case DataType::Float: return "Float";
        case DataType::Double: return "Double";
        case DataType::Char: return "Char";
        case DataType::String: return "String";
        case DataType::Custom: return "Custom";
        default: return "Unknown";
    }
}

#endif // ENUMS_H

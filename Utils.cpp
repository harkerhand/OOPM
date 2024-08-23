#include "Utils.h"


int getDataTypeSize(DataType dataType) {
    switch (dataType) {
        case DataType::Int:
            return sizeof(int);
        case DataType::Float:
            return sizeof(float);
        case DataType::Double:
            return sizeof(double);
        case DataType::Char:
            return sizeof(char);
        case DataType::String:
            return 0; // 字符串的字节数依赖于实际内容
        case DataType::Custom:
            return 0; // 自定义类型的字节数需要用户定义
        default:
            return 0; // 默认返回 0
    }
}

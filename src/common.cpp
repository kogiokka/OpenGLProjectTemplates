#include "common.hpp"

namespace gl
{
    unsigned int sizeOf(DataType type)
    {
        switch (type)
        {
            case gl::DataType::UnsignedShort: return sizeof(unsigned short);
            case gl::DataType::UnsignedInt: return sizeof(unsigned int);
            case gl::DataType::Short: return sizeof(short);
            case gl::DataType::Int: return sizeof(int);
            case gl::DataType::Float: return sizeof(float);
            case gl::DataType::Double: return sizeof(double);
        }
    }
}

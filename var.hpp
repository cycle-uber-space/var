
#ifndef _VAR_HPP_
#define _VAR_HPP_

#include <stdint.h>

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef float F32;
typedef double F64;

class var
{
public:
    var();
    var(F32 val);
    var(F64 val);

    bool is_nil() const;
    bool is_f32() const;
    bool is_f64() const;

    int _type;
    union
    {
        F32 f32;
        F64 f64;
    } _data;
};

#endif

#ifdef VAR_IMPLEMENTATION

#ifndef _VAR_CPP_
#define _VAR_CPP_

enum
{
    VAR_NIL = 0,
    VAR_F32,
    VAR_F64,
};

var::var() : _type(VAR_NIL)
{
}

var::var(F32 val) : _type(VAR_F32)
{
    _data.f32 = val;
}

var::var(F64 val) : _type(VAR_F64)
{
    _data.f64 = val;
}

bool var::is_nil() const
{
    return _type == VAR_NIL;
}

bool var::is_f32() const
{
    return _type == VAR_F32;
}

bool var::is_f64() const
{
    return _type == VAR_F64;
}

#endif

#endif

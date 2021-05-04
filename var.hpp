
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

    bool is_nil() const;

    int _type;
};

#endif

#ifdef VAR_IMPLEMENTATION

#ifndef _VAR_CPP_
#define _VAR_CPP_

enum
{
    VAR_NIL = 0,
};

var::var() : _type(VAR_NIL)
{
}

bool var::is_nil() const
{
    return _type == VAR_NIL;
}

#endif

#endif

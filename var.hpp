
#ifndef _VAR_HPP_
#define _VAR_HPP_

#ifndef VAR_WANT_STRING
#define VAR_WANT_STRING 1
#endif

#ifndef VAR_WANT_MATH
#define VAR_WANT_MATH 1
#endif

#ifndef VAR_FAIL
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#define VAR_FAIL(...) do { fprintf(stderr, "FAIL: "); fprintf(stderr, __VA_ARGS__); raise(SIGINT); exit(1); } while (0)
#endif

#if VAR_WANT_STRING
#include <stdlib.h>
#include <string.h>
#endif

#include <stdint.h>

#if VAR_WANT_MATH
#include <math.h>
#endif

#ifdef VAR_NAMESPACE
namespace VAR_NAMESPACE {
#endif

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

#if VAR_WANT_STRING

class str
{
public:
    str() : _size(0),  _data(nullptr)
    {
    }

    str(char const * str)
    {
        _size = strlen(str) + 1;
        _data = (char *) malloc(_size);
        memcpy(_data, str, _size);
    }

    str(str const & other)
    {
        _size = other._size;
        _data = (char *) malloc(_size);
        memcpy(_data, other._data, _size);
    }

    ~str()
    {
        if (_data)
        {
            _size = 0;
            free(_data);
            _data = nullptr;
        }
    }

    str & operator=(str const & other)
    {
        if (_data)
        {
            free(_data);
        }
        _size = other._size;
        _data = (char *) malloc(_size);
        memcpy(_data, other._data, _size);
        return *this;
    }

    size_t _size;
    char * _data;
};

#endif /* _VAR_HPP_ */

class var
{
public:
    var();
    var(U8 val);
    var(U16 val);
    var(U32 val);
    var(U64 val);
    var(I8 val);
    var(I16 val);
    var(I32 val);
    var(I64 val);
    var(F32 val);
    var(F64 val);
#if VAR_WANT_STRING
    var(char const * str);
#endif

    bool is_nil() const;
    bool is_u8() const;
    bool is_u16() const;
    bool is_u32() const;
    bool is_u64() const;
    bool is_i8() const;
    bool is_i16() const;
    bool is_i32() const;
    bool is_i64() const;
    bool is_f32() const;
    bool is_f64() const;
#if VAR_WANT_STRING
    bool is_str() const;
#endif

    I32 to_i32() const;
    F32 to_f32() const;
    F32 to_f64() const;
    char const * to_str() const;

    int _type;
    union
    {
        U8 u8;
        U16 u16;
        U32 u32;
        U64 u64;
        I8 i8;
        I16 i16;
        I32 i32;
        I64 i64;
        F32 f32;
        F64 f64;
    } _data;
#if VAR_WANT_STRING
    // NOTE we cannot put types with non-trivial constructors into unions
    str _data_str;
#endif
};

var operator+(var a, var b);

#if VAR_WANT_MATH
var sin(var x);
#endif

#ifdef VAR_NAMESPACE
}
#endif

#endif

#ifdef VAR_IMPLEMENTATION

#ifndef _VAR_CPP_
#define _VAR_CPP_

#ifdef VAR_NAMESPACE
namespace VAR_NAMESPACE {
#endif

enum
{
    VAR_NIL = 0,

    VAR_U8,
    VAR_U16,
    VAR_U32,
    VAR_U64,

    VAR_I8,
    VAR_I16,
    VAR_I32,
    VAR_I64,

    VAR_F32,
    VAR_F64,

#if VAR_WANT_STRING
    VAR_STR,
#endif
};

static char const * type_name(int type)
{
    switch (type)
    {
    case VAR_NIL:
        return "nil";

    case VAR_U8:
        return "u8";
    case VAR_U16:
        return "u16";
    case VAR_U32:
        return "u32";
    case VAR_U64:
        return "u64";

    case VAR_I8:
        return "i8";
    case VAR_I16:
        return "i16";
    case VAR_I32:
        return "i32";
    case VAR_I64:
        return "i64";

    case VAR_F32:
        return "f32";
    case VAR_F64:
        return "f64";

#if VAR_WANT_STRING
    case VAR_STR:
        return "str";
#endif

    default:
        return "<unknown>";
    }
}

str operator+(str const & a, str const & b)
{
    str c;
    c._size = a._size + b._size - 1;
    c._data = (char *) malloc(c._size);
    memcpy(c._data, a._data, a._size);
    memcpy(c._data + a._size - 1, b._data, b._size);
    return c;
}

var::var() : _type(VAR_NIL)
{
}

var::var(U8 val) : _type(VAR_U8)
{
    _data.u8 = val;
}

var::var(U16 val) : _type(VAR_U16)
{
    _data.u16 = val;
}

var::var(U32 val) : _type(VAR_U32)
{
    _data.u32 = val;
}

var::var(U64 val) : _type(VAR_U64)
{
    _data.u64 = val;
}

var::var(I8 val) : _type(VAR_I8)
{
    _data.i8 = val;
}

var::var(I16 val) : _type(VAR_I16)
{
    _data.i16 = val;
}

var::var(I32 val) : _type(VAR_I32)
{
    _data.i32 = val;
}

var::var(I64 val) : _type(VAR_I64)
{
    _data.i64 = val;
}

var::var(F32 val) : _type(VAR_F32)
{
    _data.f32 = val;
}

var::var(F64 val) : _type(VAR_F64)
{
    _data.f64 = val;
}

#if VAR_WANT_STRING

var::var(char const * str) : _type(VAR_STR)
{
    _data_str = str;
}

#endif

bool var::is_nil() const
{
    return _type == VAR_NIL;
}

bool var::is_u8() const
{
    return _type == VAR_U8;
}

bool var::is_u16() const
{
    return _type == VAR_U16;
}

bool var::is_u32() const
{
    return _type == VAR_U32;
}

bool var::is_u64() const
{
    return _type == VAR_U64;
}

bool var::is_i8() const
{
    return _type == VAR_I8;
}

bool var::is_i16() const
{
    return _type == VAR_I16;
}

bool var::is_i32() const
{
    return _type == VAR_I32;
}

bool var::is_i64() const
{
    return _type == VAR_I64;
}

bool var::is_f32() const
{
    return _type == VAR_F32;
}

bool var::is_f64() const
{
    return _type == VAR_F64;
}

#if VAR_WANT_STRING

bool var::is_str() const
{
    return _type == VAR_STR;
}

#endif

I32 var::to_i32() const
{
    switch (_type)
    {
    case VAR_I32:
        return _data.i32;
    default:
        VAR_FAIL("no conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return I32();
    }
}

F32 var::to_f32() const
{
    switch (_type)
    {
    case VAR_F32:
        return _data.f32;
    case VAR_F64:
        return (F32) _data.f64;
    default:
        VAR_FAIL("no conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return F32();
    }
}

F32 var::to_f64() const
{
    switch (_type)
    {
    case VAR_F32:
        return (F64) _data.f32;
    case VAR_F64:
        return _data.f64;
    default:
        VAR_FAIL("no conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return F64();
    }
}

#if VAR_WANT_STRING

char const * var::to_str() const
{
    switch (_type)
    {
    case VAR_STR:
        return _data_str._data;
    default:
        VAR_FAIL("no conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return NULL;
    }
}

#endif

#define _VAR2(a, b) (((a) << 16) | (b))

var operator+(var a, var b)
{
    switch (_VAR2(a._type, b._type))
    {
    case _VAR2(VAR_I32, VAR_I32):
        return var(a.to_i32() + b.to_i32());
    case _VAR2(VAR_STR, VAR_STR):
        {
            var c;
            c._type = VAR_STR;
            c._data_str = a._data_str + b._data_str;
            return c;
        }
    default:
        VAR_FAIL("no implementation of %s() for type %s (%d) and %s (%d)\n", __FUNCTION__, type_name(a._type), a._type, type_name(b._type), b._type);
        return var();
    }
}

#if VAR_WANT_MATH
var sin(var x)
{
    switch (x._type)
    {
    case VAR_F32:
        return ::sinf(x._data.f32);
    case VAR_F64:
        return ::sin(x._data.f64);
    default:
        VAR_FAIL("no implementation of %s() for type %s (%d)\n", __FUNCTION__, type_name(x._type), x._type);
        return var();
    }
}
#endif

#ifdef VAR_NAMESPACE
}
#endif

#endif /* _VAR_CPP_ */

#endif  /* VAR_IMPLEMENTATION */

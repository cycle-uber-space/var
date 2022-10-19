
#ifndef _VAR_HPP_
#define _VAR_HPP_

#ifndef VAR_WANT_STRING
#define VAR_WANT_STRING 1
#endif

#ifndef VAR_WANT_MATH
#define VAR_WANT_MATH 1
#endif

#ifndef VAR_WANT_IOSTREAM
#define VAR_WANT_IOSTREAM 0
#endif

#ifndef VAR_FAIL
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#define VAR_FAIL(...) do { fprintf(stderr, "%s:%d: FAIL: ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__); raise(SIGINT); exit(1); } while (0)
#endif

#if VAR_WANT_STRING
#include <stdlib.h>
#include <string.h>
#endif

#include <stdint.h>

#if VAR_WANT_MATH
#include <math.h>
#endif

#if VAR_WANT_IOSTREAM
#include <iostream>
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

    U8 to_u8() const;
    U16 to_u16() const;
    U32 to_u32() const;
    U64 to_u64() const;
    I8 to_i8() const;
    I16 to_i16() const;
    I32 to_i32() const;
    I64 to_i64() const;
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

bool id(var a, var b);
bool eq(var a, var b);

bool operator==(var a, var b);
bool operator!=(var a, var b);

var operator+(var a, var b);

#if VAR_WANT_MATH
var sin(var x);
var cos(var x);
var sqrt(var x);
#endif

#if VAR_WANT_IOSTREAM
std::ostream & operator<<(std::ostream & out, var v);
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

U8 var::to_u8() const
{
    switch (_type)
    {
    case VAR_U8:
        return _data.u8;
    default:
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return U16();
    }
}

U16 var::to_u16() const
{
    switch (_type)
    {
    case VAR_U8:
        return (U16) _data.u8;
    case VAR_U16:
        return _data.u16;
    default:
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return U16();
    }
}

U32 var::to_u32() const
{
    switch (_type)
    {
    case VAR_U8:
        return (U32) _data.u8;
    case VAR_U16:
        return (U32) _data.u16;
    case VAR_U32:
        return _data.u32;
    default:
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return U16();
    }
}

I8 var::to_i8() const
{
    switch (_type)
    {
    case VAR_I8:
        return _data.i8;
    default:
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return I16();
    }
}

I16 var::to_i16() const
{
    switch (_type)
    {
    case VAR_U8:
        return (I16) _data.u8;
    case VAR_I8:
        return (I16) _data.i8;
    case VAR_I16:
        return _data.i16;
    default:
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return I16();
    }
}

I32 var::to_i32() const
{
    switch (_type)
    {
    case VAR_U8:
        return (I32) _data.u8;
    case VAR_U16:
        return (I32) _data.u16;
    case VAR_I8:
        return (I32) _data.i8;
    case VAR_I16:
        return (I32) _data.i16;
    case VAR_I32:
        return _data.i32;
    default:
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return I32();
    }
}

I64 var::to_i64() const
{
    switch (_type)
    {
    case VAR_U8:
        return (I64) _data.u8;
    case VAR_U16:
        return (I64) _data.u16;
    case VAR_U32:
        return (I64) _data.u32;
    case VAR_I8:
        return (I64) _data.i8;
    case VAR_I16:
        return (I64) _data.i16;
    case VAR_I32:
        return (I64) _data.i32;
    case VAR_I64:
        return _data.i64;
    default:
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return I64();
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
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
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
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
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
        VAR_FAIL("no safe conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return NULL;
    }
}

#endif

#define _VAR2(a, b) (((a) << 16) | (b))

static bool id(str a, str b)
{
    return a._size == b._size && a._data == b._data;
}

bool id(var a, var b)
{
    if (a._type != b._type)
    {
        return false;
    }

    switch (a._type)
    {
    case VAR_NIL:
        return true;
    case VAR_U8:
        return a._data.u8 == b._data.u8;
    case VAR_U16:
        return a._data.u16 == b._data.u16;
    case VAR_U32:
        return a._data.u32 == b._data.u32;
    case VAR_U64:
        return a._data.u64 == b._data.u64;
    case VAR_I8:
        return a._data.i8 == b._data.i8;
    case VAR_I16:
        return a._data.i16 == b._data.i16;
    case VAR_I32:
        return a._data.i32 == b._data.i32;
    case VAR_I64:
        return a._data.i64 == b._data.i64;
    case VAR_F32:
        return a._data.f32 == b._data.f32;
    case VAR_F64:
        return a._data.f64 == b._data.f64;
    case VAR_STR:
        return id(a._data_str, b._data_str);
    default:
        VAR_FAIL("cannot compare identity of type %s (%d)\n", type_name(a._type), a._type);
        return false;
    }
}

bool eq(var a, var b)
{
    switch (_VAR2(a._type, b._type))
    {
    case _VAR2(VAR_U8, VAR_I32):
        return a.to_i32() == b.to_i32();
#if VAR_WANT_STRING
    case _VAR2(VAR_STR, VAR_STR):
        return strcmp(a.to_str(), b.to_str()) == 0;
#endif
    default:
        VAR_FAIL("eq(%s, %s)?\n", type_name(a._type), type_name(b._type));
        return id(a, b);
    }
}

bool operator==(var a, var b)
{
    return eq(a, b);
}

bool operator!=(var a, var b)
{
    return !eq(a, b);
}

var operator+(var a, var b)
{
    switch (_VAR2(a._type, b._type))
    {
    case _VAR2(VAR_U8, VAR_U8):
        return var((U8) (a.to_u8() + b.to_u8()));
    case _VAR2(VAR_U16, VAR_U16):
        return var((U8) (a.to_u16() + b.to_u16()));
    case _VAR2(VAR_U8, VAR_U16):
        return var((U16) (a.to_u16() + b.to_u16()));
    case _VAR2(VAR_I32, VAR_I32):
        return var(a.to_i32() + b.to_i32());
    case _VAR2(VAR_STR, VAR_STR):
        {
            var c;
            c._type = VAR_STR;
            c._data.u64 = 0;
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

var cos(var x)
{
    switch (x._type)
    {
    case VAR_F32:
        return ::cosf(x._data.f32);
    case VAR_F64:
        return ::cos(x._data.f64);
    default:
        VAR_FAIL("no implementation of %s() for type %s (%d)\n", __FUNCTION__, type_name(x._type), x._type);
        return var();
    }
}

var sqrt(var x)
{
    switch (x._type)
    {
    case VAR_F32:
        return ::sqrtf(x._data.f32);
    case VAR_F64:
        return ::sqrt(x._data.f64);
    default:
        VAR_FAIL("no implementation of %s() for type %s (%d)\n", __FUNCTION__, type_name(x._type), x._type);
        return var();
    }
}
#endif

#if VAR_WANT_IOSTREAM

std::ostream & operator<<(std::ostream & out, var x)
{
    switch (x._type)
    {
    case VAR_I8:
        return out << x._data.i8;
    case VAR_I16:
        return out << x._data.i16;
    case VAR_I32:
        return out << x._data.i32;
    case VAR_I64:
        return out << x._data.i64;
    case VAR_U8:
        return out << x._data.u8;
    case VAR_U16:
        return out << x._data.u16;
    case VAR_U32:
        return out << x._data.u32;
    case VAR_U64:
        return out << x._data.u64;
    case VAR_F32:
        return out << x._data.f32;
    case VAR_F64:
        return out << x._data.f64;
    case VAR_STR:
        return out << x._data_str._data;
    default:
        VAR_FAIL("no implementation of %s() for type %s (%d)\n", __FUNCTION__, type_name(x._type), x._type);
        return out;
    }
}

#endif

#ifdef VAR_NAMESPACE
}
#endif

#endif /* _VAR_CPP_ */

#endif  /* VAR_IMPLEMENTATION */

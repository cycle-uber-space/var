
#ifndef _VAR_HPP_
#define _VAR_HPP_

#ifndef VAR_WANT_STRING
#define VAR_WANT_STRING 1
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

#endif

class var
{
public:
    var();
    var(F32 val);
    var(F64 val);
#if VAR_WANT_STRING
    var(char const * str);
#endif

    bool is_nil() const;
    bool is_f32() const;
    bool is_f64() const;
#if VAR_WANT_STRING
    bool is_str() const;
#endif

    F32 to_f32() const;
    char const * to_str() const;

    int _type;
    union
    {
        F32 f32;
        F64 f64;
    } _data;
#if VAR_WANT_STRING
    // NOTE we cannot put types with non-trivial constructors into unions
    str _data_str;
#endif
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
    };
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
};

#if VAR_WANT_STRING

char const * var::to_str() const
{
    switch (_type)
    {
    default:
        VAR_FAIL("no conversion %s() for type %s (%d)\n", __FUNCTION__, type_name(_type), _type);
        return NULL;
    }
}

#endif

#endif

#endif

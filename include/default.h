#pragma once
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#ifndef DEFAULT_ERROR_STRUCT
#    define DEFAULT_ERROR_STRUCT

typedef struct Error {
    char* msg;
} Error;
#endif

#ifndef DEFAULT_RESULT_STRUCT
#    define DEFAULT_RESULT_STRUCT

typedef struct Result {
    bool ok;
    u64 code;
    Error err;
} Result;
#endif

#define ERROR_INIT(m) \
    (Error) {         \
        .msg = m      \
    }
#define RESULT_INIT(o, c, e)         \
    (Result) {                       \
        .ok = o, .code = c, .err = e \
    }

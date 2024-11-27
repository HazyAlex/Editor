#ifndef TYPES_INCLUDE_GUARD
#define TYPES_INCLUDE_GUARD

#include <stdint.h>

#ifdef _WIN32
    #define OS_NEWLINE "\r\n"
    #define OS_NEWLINE_SIZE 2
#else
    #define OS_NEWLINE "\n"
    #define OS_NEWLINE_SIZE 1
#endif


typedef _Bool bool;
#define TRUE  1
#define FALSE 0

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;


typedef union {
    struct {
        i32 x;
        i32 y;
    };
    i32 data[2];
} ivec2;

typedef union {
    struct {
        i32 x;
        i32 y;
        i32 z;
    };
    i32 data[3];
} ivec3;

typedef union {
    struct {
        u32 x;
        u32 y;
    };
    u32 data[2];
} uvec2;

typedef union {
    struct {
        u32 x;
        u32 y;
        u32 z;
    };
    u32 data[3];
} uvec3;

typedef union {
    struct {
        u8 r;
        u8 g;
        u8 b;
    };
    u8 data[3];
} color;

ivec2 vec2_from_i32(i32 x, i32 y);
ivec3 vec3_from_i32(i32 x, i32 y, i32 z);
uvec2 vec2_from_u32(u32 x, u32 y);
uvec3 vec3_from_u32(u32 x, u32 y, u32 z);
color color_from(u8 r, u8 g, u8 b);


#endif

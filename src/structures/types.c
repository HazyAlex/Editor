#include "types.h"


ivec2 vec2_from_i32(i32 x, i32 y)
{
    ivec2 result = {.x = x, .y = y};
    return result;
}

ivec3 vec3_from_i32(i32 x, i32 y, i32 z)
{
    ivec3 result = {.x = x, .y = y, .z = z};
    return result;
}

uvec2 vec2_from_u32(u32 x, u32 y)
{
    uvec2 result = {.x = x, .y = y};
    return result;
}

uvec3 vec3_from_u32(u32 x, u32 y, u32 z)
{
    uvec3 result = {.x = x, .y = y, .z = z};
    return result;
}

color color_from(u8 r, u8 g, u8 b)
{
    color result = {.r = r, .g = g, .b = b};
    return result;
}

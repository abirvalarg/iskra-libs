#pragma once

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;

typedef uint8_t Byte;
typedef uint16_t HalfWord;
typedef uint32_t Word;

#ifndef __cplusplus
typedef Byte bool;
#endif

#define CHECK(type, size) _Static_assert(sizeof(type) == size, "Wrong size of " #type)

CHECK(int8_t, 1);
CHECK(uint8_t, 1);
CHECK(int16_t, 2);
CHECK(uint16_t, 2);
CHECK(int32_t, 4);
CHECK(uint32_t, 4);
CHECK(Byte, 1);
CHECK(HalfWord, 2);
CHECK(Word, 4);

#undef CHECK

#define false 0
#define true 1
#define NULL 0
#define BIT(n) (1 << n)

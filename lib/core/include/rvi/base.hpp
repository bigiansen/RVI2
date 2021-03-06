#pragma once

#include <cstdint>
#include <random>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <functional>
#include <stdexcept>

#include <rvi/str_utils.hpp>
#include <rvi/assert.hpp>

namespace rvi
{
    #define RVI_CONCAT(x,y) x##y
    #define FWD_DECL_CLASS(X) class X

    // Generate a unique prefixed name
    #define RVI_UNIQUENAME(prefix) RVI_CONCAT(prefix,__COUNTER__)

    // Discard return value explicitly
    #define DISCARD_RESULT auto RVI_UNIQUENAME(_trash_) =

    // Compiler conditional macros
    #if defined(_MSC_VER) && !defined(__INTEL_COMPILER) /* MSVC++ */
    #define RVI_COMPILER_MSVC 1

    #elif defined(__GNUC__) || defined(__GNUG__) && !defined(__clang__) /* GNU GCC/G++ */
    #define RVI_COMPILER_GNU 1

    #elif defined(__clang__) /* CLANG */
    #define RVI_COMPILER_CLANG 1

    #elif defined(__INTEL_COMPILER) || defined(__ICC) /* INTEL C/C++ COMPILER */
    #define RVI_COMPILER_INTEL 1

    #endif 

    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // Integer data typedefs
    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    using u8  = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using i8  = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;
   
    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // Endianess constant expressions
    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    static constexpr bool RVI_BIG_ENDIAN
        = static_cast<uint8_t>(0x00FF) == 0x00;

    static constexpr bool RVI_LITTLE_ENDIAN
        = static_cast<uint8_t>(0x00FF) == 0xFF;
}
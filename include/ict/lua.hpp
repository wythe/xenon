// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

#define _CRT_SECURE_NO_WARNINGS // Added by MAB for winblows

namespace ict {
    namespace lua {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#define luaall_c
#include <../src/lua-5.1.4/src/lapi.c>
    }
}

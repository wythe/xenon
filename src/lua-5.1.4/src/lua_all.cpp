//-- Copyright 2016 Intrig
//-- See https://github.com/intrig/xenon for license.
/*
* all.c -- Lua core, libraries and interpreter in a single file
*/

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4334 )
#endif

#define luaall_c

#include <cmath>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>


namespace xenon {
    namespace lua {
// all those deprecated but awesome C functions
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include "lapi.c"
#include "lcode.c"
#include "ldebug.c"
#include "ldo.c"
#include "ldump.c"
#include "lfunc.c"
#include "lgc.c"
#include "llex.c"
#include "lmem.c"
#include "lobject.c"
#include "lopcodes.c"
#include "lparser.c"
#include "lstate.c"
#include "lstring.c"
#include "ltable.c"
#include "ltm.c"
#include "lundump.c"
#include "lvm.c"
#include "lzio.c"

#include "lauxlib.c"
#include "lbaselib.c"
#include "ldblib.c"
#include "liolib.c"
#include "linit.c"
#include "lmathlib.c"
#include "loadlib.c"
#include "loslib.c"
#include "lstrlib.c"
#include "ltablib.c"
    } // lua
} // ict
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#pragma once
#include <ict/xddl.h>
#include <ict/lua.hpp>
namespace ict {
template <typename State>
inline message::cursor get_cursor(State * L) {
    ict::lua::lua_getglobal(L, "node");
    return *static_cast<message::cursor *>(lua_touserdata(L, -1));
}

// get a timestamp from a .Net encoded date (don't ask)
inline static int script_datetime(ict::lua::lua_State *L) {
    auto n = get_cursor(L);
    auto value = ict::to_integer<int64_t>(n->bits);
    auto dt = DateTime(value);
    std::ostringstream os;
    os << dt;
    ict::lua::lua_pushstring(L, os.str().c_str());
    return 1;
}

// get the description of a previous node
inline static int script_Description(ict::lua::lua_State *L) {
    const char * s = ict::lua::luaL_checklstring(L, 1, NULL);
    auto n = get_cursor(L);
    auto c = rfind(n, s);
    if (c.is_root()) ict::lua::lua_pushstring(L, "");
    else ict::lua::lua_pushstring(L, description(c).c_str());
    return 1;
}

// get the enum string for this node
inline static int script_EnumValue(ict::lua::lua_State *L) {
    auto n = get_cursor(L);
    ict::lua::lua_pushstring(L, n->elem->v->venum_string(n->elem, n).c_str());
    return 1;
}

// get the value of a previous node
inline static int script_Value(ict::lua::lua_State *L) {
    const char * s = ict::lua::luaL_checklstring(L, 1, NULL);
    auto n = get_cursor(L);
    auto c = rfind(n, s);
    if (c.is_root()) ict::lua::lua_pushnumber(L, 0);
    else ict::lua::lua_pushnumber(L, c->value());
    return 1;
}

inline static int script_Slice(ict::lua::lua_State *L) {
    int index = static_cast<int>(ict::lua::luaL_checknumber(L, 1));
    int length = static_cast<int>(ict::lua::luaL_checknumber(L, 2));
    auto n = get_cursor(L);
    auto subs = n->bits.substr(index, length);
    auto num = ict::to_integer<int64_t>(subs);
    ict::lua::lua_pushnumber(L, num);
    return 1;
}


inline static int script_TwosComplement(ict::lua::lua_State * L) {
    auto n = get_cursor(L);
    auto value = ict::to_integer<int>(n->bits);
    value <<= (32 - n->bits.bit_size());
    value >>= (32 - n->bits.bit_size());
    ict::lua::lua_pushnumber(L, value);
    return 1;
}

inline static int script_Ascii(ict::lua::lua_State * L) {
    auto & bits = get_cursor(L)->bits;
    auto s = std::string(bits.begin(), bits.bit_size() / 8);
    for (auto & c : s) if (!isprint(c)) c = '.';
    ict::lua::lua_pushstring(L, s.c_str());
    return 1;
}

inline static int script_Imsi_S(ict::lua::lua_State * L) {
    auto & bits = get_cursor(L)->bits;
    ict::lua::lua_pushstring(L, decode_imsi(bits).c_str());
    return 1;
}

inline static int script_Gsm7(ict::lua::lua_State * L) {
    int fill = 0;
    const char * s = ict::lua::luaL_checklstring(L, 1, NULL);
    if (s) {
        auto c = ict::rfind(get_cursor(L), s);
        if (!c.is_root()) fill = to_integer<int>(c->bits); 
    }
    std::string sms = ict::gsm7(get_cursor(L)->bits, fill);
    ict::lua::lua_pushstring(L, sms.c_str());
    return 1;
}

inline static int script_find(ict::lua::lua_State *L) {
    auto n = ict::get_root(get_cursor(L));
    const char * s = luaL_checkstring(L, 1);

    auto c = ict::find(n, s);
    if (c == n.end()) {
        lua_pushstring(L, "");
        return 1;
    }
    lua_pushstring(L, description(c).c_str());
    return 1;
}

inline static int script_Search(ict::lua::lua_State *) {
    IT_PANIC("search() lua function is no longer supported, use find().");
}
}

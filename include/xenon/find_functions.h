#pragma once
//-- Copyright 2016 Intrig
//-- See https://github.com/intrig/xenon for license.
#include <ict/ict.h>
#include <vector>
#include <string>
#include <xenon/find_first.h>
#include <xenon/for_each_path.h>

namespace xenon {

template <typename T>  
inline bool leaf_test(const T &) { return 1; }

template <typename T> 
inline std::string name_of(const T & a) { return a.name; }

template <>
inline std::string name_of(const std::string & value) { return value; }

template <>
inline std::string name_of(const int & value) { return ict::to_string(value); }

template <typename S, typename C> 
inline void path_string(S & ss, C c) {
    if (!c.is_root()) {
        path_string(ss, c.parent());
        ss << '/';
        ss << name_of(*c);
    }
}

// return the path of a cursor
template <typename T, typename C = typename T::is_cursor> 
inline std::string path_string(T c) {
    ict::osstream ss;
    path_string(ss, c);
    return ss.take();
}
}

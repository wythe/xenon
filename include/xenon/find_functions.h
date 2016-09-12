#pragma once
//-- Copyright 2016 Intrig
//-- See https://github.com/intrig/xenon for license.
#include <ict/ict.h>
#include <vector>
#include <string>
#include <xenon/find_first.h>
#include <xenon/for_each_path.h>

namespace xenon {
// find and rfind algorithms
struct path {
    typedef std::vector<std::string>::iterator iterator;
    typedef std::vector<std::string>::const_iterator const_iterator;

    path(const std::vector<std::string> & path, bool abs = true) : p(path), abs(abs) { }

    path(const std::string & path_string) {
        auto ps = path_string;
        if (ps.size() == 0) std::runtime_error("invalid empty path");
        abs = true;
        if (ps[0] == '/') { 
            abs = true;
            ps.erase(0, 1);
            if (ps.size() == 0) std::runtime_error("a lone '/' is an invalid path");
            if (ps[0] == '/') { 
                abs = false;
                ps.erase(0, 1);
                if (ps.size() == 0) std::runtime_error("a lone '//' is an invalid path");
            }
        }
        p = ict::escape_split(ps, '/');
    }

    path(const char * path_string) : path(std::string(path_string)) {}

    iterator begin() { return p.begin(); }
    const_iterator begin() const { return p.begin(); }
    const_iterator cbegin() const { return p.begin(); }
    iterator end() { return p.end(); }
    const_iterator end() const { return p.end(); }
    const_iterator cend() const { return p.end(); }
    bool absolute() const { return abs; }
    void absolute(bool abs) { this->abs = abs; }
    bool empty() const { return p.empty(); }
    size_t size() const { return p.size(); }
    private:
    std::vector<std::string> p;
    bool abs = false;
};

inline ict::osstream & operator<<(ict::osstream & os, const path & p) {
    if (p.absolute()) os << "/";
    ict::join(os, p.cbegin(), p.cend(), "/");
    return os;
}

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

#pragma once
//-- Copyright 2016 Intrig
//-- See https://github.com/intrig/xenon for license.
#include <ict/ict.h>
#include <vector>
#include <string>

namespace xenon {
// find_first
struct path {
    typedef std::vector<std::string>::iterator iterator;
    typedef std::vector<std::string>::const_iterator const_iterator;

    path(const std::vector<std::string> & path, bool abs = false) : p(path), abs(abs) { }

    path(const std::string & path_string) {
        auto ps = path_string;
        if (ps.size() == 0) std::runtime_error("invalid empty path");
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

inline std::ostream & operator<<(std::ostream & os, const path & p) {
    if (p.absolute()) os << "/";
    ict::join(os, p.cbegin(), p.cend(), "/");
    return os;
}

// root a/b a
// similar to for_each_path, but we return after the first time found
template <typename Cursor, typename ForwardIterator>
inline Cursor find_first(Cursor parent, ForwardIterator first, ForwardIterator last, ForwardIterator curr) {
    for (auto child = parent.begin(); child != parent.end(); ++child) {
        if (*curr == child->name()) {
            if ((curr + 1) == last) { 
                return child;
            } else {
                auto c = find_first(child, first, last, curr + 1);
                if (c != child.end()) return c;
            }
        } else {
            auto c = find_first(child, first, last, first);
            if (c != child.end()) return c;
        }
    }
    return parent.end();
}

template <typename Cursor, typename ForwardIterator>
inline Cursor find_first_abs(Cursor parent, ForwardIterator first, ForwardIterator last, ForwardIterator curr) {
    for (auto child = parent.begin(); child != parent.end(); ++child) {
        if (*curr == child->name()) {
            if ((curr + 1) == last) { 
                return child;
            } else {
                auto c = find_first_abs(child, first, last, curr + 1);
                if (c != child.end()) return c;
            }
        }
    }
    return parent.end();
}

template <typename Cursor>
inline Cursor find_first(Cursor parent, const path & path) {
    return path.absolute() ? 
        find_first_abs(parent, path.begin(), path.end(), path.begin()) :
        find_first(parent, path.begin(), path.end(), path.begin());
}

template <typename Cursor>
inline Cursor rfind_first(Cursor first, const path & path) {
    typedef typename Cursor::ascending_cursor_type ascending_cursor;
    auto rfirst = ascending_cursor(first);
    while (!rfirst.is_root()) {
        if (rfirst->name() == *path.begin()) {
            auto parent = Cursor(rfirst);
            if (path.size() == 1) return parent;
            auto x = find_first_abs(parent, path.begin(), path.end(), path.begin() + 1);
            if (x != parent.end()) return x;
        }
        ++rfirst;
    }
    return rfirst;
}


}

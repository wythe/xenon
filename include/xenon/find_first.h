#pragma once
//-- Copyright 2016 Intrig
//-- See https://github.com/intrig/xenon for license.
#include <ict/ict.h>
#include <vector>
#include <string>

namespace xenon {
// find_first
struct xpath {
    typedef std::vector<std::string>::iterator iterator;
    typedef std::vector<std::string>::const_iterator const_iterator;

    xpath(const std::vector<std::string> & xpath, bool abs = false) : p(xpath), abs(abs) { }

    xpath(const std::string & path_string) {
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

    xpath(const char * path_string) : xpath(std::string(path_string)) {}

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

inline ict::osstream & operator<<(ict::osstream & os, const xpath & p) {
    if (p.absolute()) os << "/";
    ict::join(os, p.cbegin(), p.cend(), "/");
    return os;
}

inline std::ostream & operator<<(std::ostream & os, const xpath & p) {
    if (p.absolute()) os << "/";
    ict::join(os, p.cbegin(), p.cend(), "/");
    return os;
}

// similar to for_each_path, but we return after the first time found
template <typename Cursor, typename Path, typename ForwardIterator>
inline Cursor find_first(Cursor parent, const Path & path, ForwardIterator curr) {
    for (auto child = parent.begin(); child != parent.end(); ++child) {
        if (*curr == child->name()) {
            if ((curr + 1) == path.end()) { 
                return child;
            } else {
                auto c = find_first(child, path, curr + 1);
                if (c != child.end()) return child;
            }
        } else {
            auto c = find_first(child, path, path.begin());
            if (c != child.end()) return child;
        }
    }
    return parent.end();
}

template <typename Cursor>
inline Cursor find_first(Cursor parent, const xpath & path) {
    return find_first(parent, path, path.begin());
}
}

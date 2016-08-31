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

namespace util {
    template <typename Cursor, typename PathIter, typename Op, typename Test>
    inline Cursor find_first_x(Cursor parent, PathIter first, PathIter last, Op op, Test test) {
        if (parent.empty()) return parent.end();
        for (auto i = parent.begin(); i!=parent.end(); ++i) {
            if (op(*i) == *first) {
                if (first + 1 == last) {
                    if (test(*i)) return i;
                } else {
                    auto n = find_first_x(i, first + 1, last, op, test);
                    if (n != i.end()) return n;
                }
            }
        }
        return parent.end();
    }
}

// find_first given a path
template <typename Cursor, typename Op, typename Test>
inline Cursor find_first(Cursor parent, const xpath & path, Op op, Test test) {
    typedef typename Cursor::linear_type iterator;
    if (path.absolute()) return util::find_first_x(parent, path.begin(), path.end(), op, test);
    else {
        for (iterator i = parent.begin(); i!= parent.end(); ++i) {
            if (op(*i) == *path.begin()) {
                if (path.begin() + 1 == path.end()) {
                    if (test(*i)) return i;
                } else {
                    auto c = Cursor(i);
                    auto x = util::find_first_x(c, path.begin() + 1, path.end(), op, test);
                    if (x != c.end()) return x;
                }
            }
        }
        return parent.end();
    }
}

template <typename Cursor, typename Op>
inline Cursor find_first(Cursor parent, const xpath & path, Op op) {
    typedef typename Cursor::value_type value_type;
    return find_first(parent, path, op, [](const value_type &){ return true; });
}

template <typename Cursor>
inline Cursor find_first(Cursor parent, const xpath & path) {
    return find_first(parent, path, name_of<typename Cursor::value_type>);
}

template <typename Cursor, typename ForIter, typename Action>
void for_each_path(Cursor parent, const xpath & path, ForIter curr, Action action) {
    auto last = --path.end();
    IT_WARN("last is " << *last);
    for (auto i = parent.begin(); i != parent.end(); ++i) {
        IT_WARN(path << " " << *curr << ": " << i->name());
        if (*curr == i->name()) {
            if (curr == last) { 
                action(i);
            } 
            for_each_path(i, path, curr + 1, action);
        } else {
            for_each_path(i, path, path.begin(), action);
        }
    }
}

template <typename Cursor, typename Action>
void for_each_path(Cursor parent, const xpath & path, Action action) {
    for_each_path(parent, path, path.begin(), action);
}

#if 0
void match_path(Cursor parent, path & curr_path, const path & path) {
    for (iterator i = parent.begin(); i!= parent.end(); ++i) {
        curr_path += name_of(i);
        if (curr_path.has_tail(path)) f(
    }
}

template <typename Cursor, const path & path, typename Function>
inline Function for_each_path(cursor parent, const path & path, Op f) {
    typedef typename Cursor::linear_type iterator;
    path curr;
    for (iterator i = parent.begin(); i!= parent.end(); ++i) {
        curr = name_of(i);
    }

    if (path.absolute()) return util::find_first_x(parent, path.begin(), path.end(), op, test);
    else {
        for (iterator i = parent.begin(); i!= parent.end(); ++i) {
            if (op(*i) == *path.begin()) {
                if (path.begin() + 1 == path.end()) {
                    if (test(*i)) return i;
                } else {
                    auto c = Cursor(i);
                    auto x = util::find_first_x(c, path.begin() + 1, path.end(), op, test);
                    if (x != c.end()) return x;
                }
            }
        }
        return parent.end();
    }
}
#endif
}

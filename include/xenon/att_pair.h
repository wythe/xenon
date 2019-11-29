#pragma once
#include "ict/exception.h"
#include <vector>
#include <string>

namespace xenon {
struct att_pair {
    operator std::string() const { return name; }
    att_pair(std::string name, std::string value)
        : name(name), value(value) {}
    std::string name;
    std::string value;
};

typedef std::vector<att_pair> att_list;

template <typename T>
inline auto find_att_by_name(T first, T last, const std::string & name) -> T {
    return std::find_if(first, last, [&](auto a) { return a.name == name; });
}

inline bool att_exists(const att_list &atts, std::string name) {
#if 1
    auto i = find_att_by_name(atts.begin(), atts.end(), name);
#else
    auto i = std::find(atts.begin(), atts.end(), name);
#endif
    if (i != atts.end())
        return true;
    return false;
}

inline std::string find_att(const att_list &atts, std::string name,
                            const std::string &def = "") {
#if 1
    auto i = find_att_by_name(atts.begin(), atts.end(), name);
#else
    auto i = std::find(atts.begin(), atts.end(), name);
#endif
    if (i == atts.end())
        return def;
    return i->value;
}

inline std::string find_required_att(const att_list &atts,
                                     const std::string &name) {
#if 1
    auto i = find_att_by_name(atts.begin(), atts.end(), name);
#else
    auto i = std::find(atts.begin(), atts.end(), name);
#endif
    if (i == atts.end())
        IT_PANIC("missing required attribute: " << name);
    return i->value;
}

template <typename T>
std::pair<T, T> find_exclusive_att(const att_list &atts, T x, T y) {
    auto p = std::make_pair(find_att(atts, x), find_att(atts, y));
    if (p.first.empty() != p.second.empty())
        return p; // exclusive or
    IT_PANIC(x << " and " << y << " are required exclusive attributes");
}

template <typename T> bool to_boolean(T value) {
    if (value == "true")
        return true;
    if (value == "false")
        return false;
    IT_PANIC(value << " must be true or false");
};

inline void create_att_list(att_list &atts, const char **att_array) {
    atts.clear();
    // TODO: build this data structure in xml_base, but use uint64_t for the
    // names
    for (const char **ai = att_array; (*ai)[0] != '\0'; ai += 2) {
        atts.emplace_back(*ai, *(ai + 1));
        if (find_att_by_name(atts.begin(), atts.end(), std::string(*ai)) !=
            (atts.end() - 1)) {
            IT_PANIC("duplicate attribute: \"" << *ai << "\"");
        }
    }
}

// make sure incorrect att doesn't appear nor duplicates
inline void validate_att_list(const att_list &atts,
                              std::vector<std::string> valid) {
    auto names = std::vector<std::string>();
    valid.reserve(atts.size());
    for (auto &i : atts)
        names.push_back(i.name);
    std::sort(valid.begin(), valid.end());

    std::sort(names.begin(), names.end());
    auto i = std::adjacent_find(names.begin(), names.end());
    if (i != names.end())
        IT_PANIC("duplicate attribute name: " << *i);

    // go through names and make sure each is in valid
    for (auto &i : names) {
        if (std::find(valid.begin(), valid.end(), i) == valid.end())
            IT_PANIC(i << " is an invalid attribute name");
    }
}
} // namespace xenon

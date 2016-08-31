#pragma once
//-- Copyright 2016 Intrig
//-- See https://github.com/intrig/xenon for license.
#include <ict/ict.h>
#include <vector>
#include <string>
#include <xenon/find_first.h>
namespace xenon {

template <typename Cursor, typename ForIter, typename Action>
// curr - current iterator into path
void for_each_path(Cursor parent, const xpath & path, ForIter curr, Action action) {
    auto last = --path.end();
    IT_WARN("last is " << *last);
    for (auto i = parent.begin(); i != parent.end(); ++i) {
        IT_WARN(path << " " << *curr << ": " << i->name());
        if (*curr == i->name()) {
            if (curr == last) { 
                IT_WARN("Action!");
                action(i);
                IT_WARN("starting over and going deeper");
                for_each_path(i, path, path.begin(), action);
            } else {
                IT_WARN("continuing and going deeper");
                for_each_path(i, path, curr + 1, action);
            }
        } else {
            IT_WARN("no match, going deeper");
            for_each_path(i, path, path.begin(), action);
        }
    }
}

template <typename Cursor, typename Action>
void for_each_path(Cursor parent, const xpath & path, Action action) {
    for_each_path(parent, path, path.begin(), action);
}

}

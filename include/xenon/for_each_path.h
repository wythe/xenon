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
    for (auto c = parent.begin(); c != parent.end(); ++c) {
        if (*curr == c->name()) {
            if ((curr + 1) == path.end()) { 
                action(c);
                for_each_path(c, path, path.begin(), action);
            } else for_each_path(c, path, curr + 1, action);
        } else for_each_path(c, path, path.begin(), action);
    }
}

template <typename Cursor, typename Action>
void for_each_path(Cursor parent, const xpath & path, Action action) {
    for_each_path(parent, path, path.begin(), action);
}

}

#pragma once
//-- Copyright 2016 Intrig
//-- See https://github.com/intrig/xenon for license.
#include <ict/ict.h>
#include <vector>
#include <string>
#include <xenon/find_first.h>
namespace xenon {

template <typename Cursor, typename Path, typename ForwardIterator, typename Action>
// Cursor - SequenceContainer and ForwardIterator
// Path - Container with ForwardIterator
// Action - Lambda Expression that takes a Cursor parameter
//
// parent - parent node in tree
// path - a vector of strings made up from something like "a/b/c"
// curr - current iterator into path
// action - lambda function to be called upon match
inline void for_each_path(Cursor parent, const Path & path, ForwardIterator curr, Action action) {
    for (auto child = parent.begin(); child != parent.end(); ++child) {
        if (*curr == child->name()) {
            if ((curr + 1) == path.end()) { 
                action(child);
                for_each_path(child, path, path.begin(), action);
            } else for_each_path(child, path, curr + 1, action);
        } else for_each_path(child, path, path.begin(), action);
    }
}

template <typename Cursor, typename Action>
void for_each_path(Cursor parent, const xpath & path, Action action) {
    for_each_path(parent, path, path.begin(), action);
}

}

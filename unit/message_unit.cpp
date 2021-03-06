#include "message_unit.h"
#include <string>
#include <vector>
#include <xenon/xenon.h>

namespace xn = xenon;

// Call for_each_path and return the number of matches.
static int count_test(xn::message const &m, std::string const &path) {
    int found = 0;
    xn::for_each_path(m, path, [&](xn::message::const_cursor) { found++; });
    return found;
}

struct first_type {
    std::string bits;
    xenon::path path;
    int result; // -1 means not found
};

static std::ostream &operator<<(std::ostream &os, const first_type &x) {
    os << x.bits << ' ' << x.path << ' ' << x.result;
    return os;
}

static auto fv = std::vector<std::pair<std::string, std::vector<first_type>>> {
    {"field01/start",
     {
         {"@1", "foo", 1},
         {"@0", "foo", 0},
         {"@1", "boo", -1},
         {"@1", "boo/foo", -1},
     }},
        {"find/A",
         {
             {"@1", "a/b", 1},
             {"@0", "a/b", 0},
             {"@1", "b", 1},
             {"@0", "b", 0},
             {"@1", "boo", -1},
             {"@1", "boo/foo", -1},
         }},
    {
        "find/B", {
            {"@10", "c", 1}, {"@01", "c", 0}, {"@01", "a/b/c", 0},
                {"@10", "a/b/c", 1}, {"@01", "b/c", 0}, {"@10", "b/c", 1},
        }
    }
};

void unit::find_first_test() {
    xn::spec_server s("xddlunit");
    for (auto &x : fv) {
        auto rec = xn::get_record(s, x.first);
        for (auto &y : x.second) {
            auto m = xn::parse(rec, y.bits);
            auto c = xn::find_first(m, y.path);
            if (c != m.end()) {
                auto last = --y.path.end();
                IT_ASSERT_MSG(c->name()
                                  << " != " << *last << " for path " << y.path,
                              c->name() == *last);
            } else {
                IT_ASSERT_MSG(x.first << ' ' << y, y.result == -1);
            }
        }
    }
}

void unit::find_first_again() {
    xenon::spec_server d("../xddl/icd.xddl");
    auto decoded_msg =
        xenon::parse(d, "0101011107952901a3de0a800982511effffffff");
    auto c = xenon::find_first(decoded_msg, "DATA/MSG_TYPE");
    IT_ASSERT(c != decoded_msg.end());
}

void unit::for_each_path_test() {
    xn::spec_server s("xddlunit");
    {
        auto rec = xn::get_record(s, "field01/start");
        auto m = xn::parse(rec, "@1");

        IT_ASSERT(count_test(m, "foo") == 1);
        IT_ASSERT(count_test(m, "foo/boo") == 0);
        IT_ASSERT(count_test(m, "goo") == 0);
        IT_ASSERT(count_test(m, "goo/foo") == 0);
    }
    {
        auto rec = xn::get_record(s, "find/A");
        auto m = xn::parse(rec, "@1");

        IT_ASSERT(count_test(m, "c") == 1);
        auto n = count_test(m, "b/c");
        IT_ASSERT_MSG(n << " is wrong", n == 1);
        IT_ASSERT(count_test(m, "a/b/c") == 1);
        IT_ASSERT(count_test(m, "a/b/c/d") == 0);
        IT_ASSERT(count_test(m, "b/c/d") == 0);
    }
    {
        auto m = xn::parse(s, "find/B", "@10");
        IT_ASSERT(count_test(m, "a") == 1);
        IT_ASSERT(count_test(m, "b") == 1);
        IT_ASSERT(count_test(m, "c") == 2);
        IT_ASSERT(count_test(m, "d") == 0);
        IT_ASSERT(count_test(m, "a/c") == 0);
        IT_ASSERT(count_test(m, "a/b") == 1);
        IT_ASSERT(count_test(m, "b/c") == 2);
        IT_ASSERT(count_test(m, "c/b") == 0);
        IT_ASSERT(count_test(m, "a/b/c") == 2);
        IT_ASSERT(count_test(m, "a/b/c/d") == 0);
        IT_ASSERT(count_test(m, "b/c/d") == 0);
    }
    {
        auto m = xn::parse(s, "find/C", "@1");
        IT_ASSERT(count_test(m, "a") == 2);
        IT_ASSERT(count_test(m, "b") == 2);
        IT_ASSERT(count_test(m, "c") == 2);
        IT_ASSERT(count_test(m, "a/b") == 2);
        IT_ASSERT(count_test(m, "b/c") == 2);
        IT_ASSERT(count_test(m, "a/b/c") == 2);
        IT_ASSERT(count_test(m, "a/b/c/a") == 1);
        IT_ASSERT(count_test(m, "b/c/a/b/c") == 1);
    }
    {
        // 2 patterns in different sections
        auto m = xn::parse(s, "find/D", "@10001");
        IT_ASSERT(count_test(m, "c") == 2);
        IT_ASSERT(count_test(m, "b/c") == 2);
        IT_ASSERT(count_test(m, "a/b/c") == 2);
        IT_ASSERT(count_test(m, "C/a/b/c") == 1);
    }
}

int main(int, char **) {
    unit test;
    return ict::unit_test<unit>(&test).run();
}

#include <vector>
#include <string>
#include <xenon/xenon.h>
#include "message_unit.h"

namespace xn = xenon;

int count_test(xn::message const & m, std::string const & path) {
    int found = 0;
    xn::for_each_path(m.root(), path, [&](xn::message::const_cursor c) {
        found++;
    });
    return found;

}


void unit::for_each_path_test() {
    xn::spec_server s("xddlunit");
    auto rec = xn::get_record(s, "field01/start");
    auto m = xn::parse(rec, "@1");
    
    IT_ASSERT(count_test(m, "foo") == 1);
    IT_ASSERT(count_test(m, "foo/boo") == 0);
    IT_ASSERT(count_test(m, "goo") == 0);
    IT_ASSERT(count_test(m, "goo/foo") == 0);

    { 
        xn::spec_server s("xddlunit");
        auto rec = xn::get_record(s, "find/A");
        auto m = xn::parse(rec, "@1");

        IT_ASSERT(count_test(m, "c") == 1);
        auto n = count_test(m, "b/c");
        IT_ASSERT_MSG(n << " is wrong", n == 1);
        IT_ASSERT(count_test(m, "a/b/c") == 1);
        IT_ASSERT(count_test(m, "a/b/c/d") == 0);
        IT_ASSERT(count_test(m, "b/c/d") == 0);
    }
}


int main (int, char **) {
    unit test;
    return ict::unit_test<unit>(&test).run();
}

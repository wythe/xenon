#pragma once
#include "xspx_parser.h"

namespace xspx {

template <typename Elem>
std::string attributes(const Elem & elem) {
    std::ostringstream os;
    if (elem.attributes.empty()) {
        return "none";
    }
    os << "name  | format | description\n";
    os << "------|--------|------------\n";
    for (auto & a : elem.attributes) {
        os << a.name << " | " << a.type_name << "|\n";
    }

    return os.str();
}

template <typename Elem>
std::string children(const Elem & elem) {
    auto v = elem.children;
    if (v.empty()) return "none";
    for (auto & s : v) {
        s.insert(0, "<");
        s.append(">");
    }
    return ict::join(v, ", ");
}


template <typename OS, typename Elem>
void disp_element(OS & os, Elem const & elem) {
    os << "# <" << elem.tag << "> element {\n\n";
    
    os << "attributes: " << attributes(elem) << "\n\n";

    os << "children: " << children(elem) << "\n\n";


    os << "}\n\n";
}

void to_writedown(std::ostream & os, const xsp_parser & xspx) {
    os << ":title XDDL Element Reference\n\n";
    os << ":toc(\"auto\")\n\n";
    for (auto & i : xspx.elems) {
        for (auto & j : i) {
            disp_element(os, j);
        }
        os << "// done\n";
    }    

#if 0
    os << "choices:\n";
    for (auto & choice : xspx.choices) {
        for (auto & i : choice.elems) os << i.tag << ": " << i.name << ", ";
        os << '\n';
    }
#endif

}

}

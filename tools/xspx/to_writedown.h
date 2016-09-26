#pragma once
#include "xspx_parser.h"

namespace xspx {

template <typename Elem>
std::string attributes(const Elem & elem) {
    std::ostringstream os;
    if (elem.attributes.empty()) {
        return "attributes: none\n";
    }
    os << "\n\nattributes | name  | type | required | description\n";
    os <<     "-----------|-------|--------|----------|------------\n";
    for (auto & a : elem.attributes) {
        os << " | " << a.name << " | " << a.type_name << "|";
        if (a.required) os << " yes ";
        os << " | ";
        os << "\n";
    }

    return os.str();
}

std::string attribute_types(const custom_type_list & types) {
    std::ostringstream os;
    os << "\ntype | default | description\n";
    os <<   "-----|---------|------------\n";
    for (auto & t : types) {
        os << t.name << " | " << t.def << " |\n";
    }
    return os.str();
}

template <typename Elem>
std::string children(const Elem & elem) {
    auto v = elem.children;
    if (v.empty()) return "none";
    for (auto & s : v) {
        s.insert(0, "^");
    }
    return ict::join(v, ", ");
}


template <typename OS, typename Elem>
void disp_element(OS & os, Elem const & elem) {
    os << "# " << elem.tag << " {\n\n";
    
    os << attributes(elem) << "\n\n";

    os << "children: " << children(elem) << "\n\n";


    os << "}\n\n";
}

template <typename OS>
void disp_choices(OS & os, const choice_type & choice) {
    os << "# " <<  choice.name << "{\n";
    for (auto & elem : choice.elems) disp_element(os, elem);
    os << "}\n\n";
}

void to_writedown(std::ostream & os, const xsp_parser & xspx) {
    os << ":title XDDL Element Reference\n\n";
    os << ":toc(\"auto\")\n\n";

    os << "# Elements {\n";
    for (auto & i : xspx.elems) {
        for (auto & j : i) {
            if (!j.is_base) disp_element(os, j);
        }
    }    
    os << "} // Elements\n";

    os << "# Polymorphic Elements {\n";
    for (auto & choice : xspx.choices)disp_choices(os, choice); 
    os << "} // Polymorphic Elements\n\n";
    os << "# Attribute Types {\n";
    os << attribute_types(xspx.custom_types);
    os << "} // Attributes\n";

#if 0
#endif

}

}

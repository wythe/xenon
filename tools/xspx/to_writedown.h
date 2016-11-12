#pragma once
#include "xspx_parser.h"

namespace xspx {

template <typename Elem>
std::string attributes(const Elem & elem) {
    std::ostringstream os;
    if (elem.attributes.empty()) {
        return "\nattributes: none\n\n";
    }
    os << "\nattributes | name  | [type](#AttributeTypes) | required\n";
    os <<   "-----------|-------|-------------------------|---------\n";
    for (auto & a : elem.attributes) {
        if (a.fixed.empty()) {
            os << " | " << a.name << " | " << a.type_name << "|";
            if (a.required) os << " &#10004; ";
            os << " | ";
            os << "\n";
        }
    }
    return os.str();
}

std::string attribute_types(const custom_type_list & types) {
    std::ostringstream os;
    os << "\nType | Default | Description\n";
    os <<   "-----|---------|------------\n";
    for (auto & t : types) {
        os << t.name << " | " << t.def << " | " << t.desc << "\n";
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

inline std::string anchor(const std::string & x) {
    std::string y = x;
    std::replace(y.begin(), y.end(), ' ', '-');
    return y;
}

template <typename OS, typename Cursor>
void disp_element(OS & os, Cursor c) {
    std::string n = (c->display.empty()) ? c->tag.c_str() : c->display; 
    os << "# " << n << " {\n\n";
    
    os << ":include ex.wd#" << anchor(n) << "/summary?\n";

    if (!c.empty()) {
        for (auto choice = c.begin(); choice!=c.end(); ++choice) {
            disp_element(os, choice);
        }
    } else {
        os << attributes(*c) << "\n\n";

        os << "children: " << children(*c) << "\n\n";
    }

    os << ":include ex.wd#" << anchor(n) << "/detail?\n";

    os << "}\n\n";
}

namespace ict {
template <typename Cursor, typename Pred>
void for_each_cursor(Cursor & parent, Pred op) {
    for (auto c = parent.begin(); c!=parent.end(); ++c) {
        op(c);
    }

}
}

template <typename OS>
void disp_choices(OS & os, const choice_type & choice) {
    os << "# " <<  choice.name << "{\n";
    ict::for_each_cursor(choice.elems, [&](auto c){ disp_element(os, c); });

    // for (auto c = choice.elems.begin(); : choice.elems) disp_element(os, elem);
    os << "}\n\n";
}

void to_writedown(std::ostream & os, const xsp_parser & xspx) {
    os << ":title XDDL Element Reference\n\n";
    os << ":toc(\"auto\")\n\n";

    auto tree = xspx::elem_tree(xspx);
    os << "# Elements {\n";
    for (auto c = tree.begin(); c != tree.end(); ++c) {
        disp_element(os, c);
    }    
    os << "} // Elements\n";
#if 0
    os << "# Polymorphic Elements {\n";
    for (auto & choice : xspx.choices)disp_choices(os, choice); 
    os << "} // Polymorphic Elements\n\n";
#endif
    os << "# Attribute Types {\n";
    os << attribute_types(xspx.custom_types);
    os << "} // Attributes\n";

#if 0
#endif

}

}

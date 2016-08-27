#include <xenon/xenon.h>
int main () {
    try {
        xenon::spec_server s("/Users/mark/wythe/xenon/xddl");
        auto rec = xenon::get_record(s, "3GPP/TS-36.331/DL-DCCH-Message");
        auto m = xenon::parse(rec,
            "0C01513C9FB9C248283B11084808F0080824810A1FA800A8202C090A1FA800010C0098090808C82E4194DFE830");
        
        // TODO make this "SM" and not "//SM" (change defaults)
        // TODO make this find_first()
        auto c = xenon::find(m.root(), "//SM"); // now find the SM field
        if (c == m.end()) throw std::runtime_error("Cannot find message!\n");

        std::cout << "Message is " << description(c) << '\n';
    } catch (std::exception & e) {
        std::cerr << e.what() << '\n';
    }
}

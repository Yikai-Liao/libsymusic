//
// Created by lyk on 23-12-13.
//

#include <string>
#include <sstream>
#include "nanobench.h"
#include "symusic.h"
// #include "symusic/container.h"
#include "fmt/core.h"

using namespace symusic;

int main(){
    fmt::print("{}, {}\n", "Hello", "world");
    Score<tag::Tick> s(0);
    fmt::print("Note num {}\n", s.note_num());
    // auto t = s.dumps<tag::Midi>();

    // bench sstream and fmt::format
    ankerl::nanobench::Bench().run("sstream", []{
        for(int i = 0; i < 1000; i++) {
            std::stringstream ss;
            ss << "Hello, word " << rand() << " !\n";
            ankerl::nanobench::doNotOptimizeAway(ss.str());
        }
    });

    // string +
    ankerl::nanobench::Bench().run("string operator +", []{
        for (int i = 0; i < 1000; i++) {
            std::string s = "Hello, world " + std::to_string(rand()) + " !\n";
            ankerl::nanobench::doNotOptimizeAway(s);
        }
    });

    // c style sprintf
    ankerl::nanobench::Bench().run("sprintf", []{
        for (int i = 0; i < 1000; i++) {
            char buf[100];
            sprintf(buf, "Hello, world %d !\n", rand());
            ankerl::nanobench::doNotOptimizeAway(buf);
        }
    });

    // fmt::format
    ankerl::nanobench::Bench().run("fmt::format", []{
        for (int i = 0; i < 1000; i++) {
            std::string s = fmt::format("Hello, world {} !\n", rand());
            ankerl::nanobench::doNotOptimizeAway(s);
        }
    });
    return 0;
}

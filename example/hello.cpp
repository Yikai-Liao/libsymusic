//
// Created by lyk on 23-12-13.
//

#include <string>
#include <sstream>
#include <iostream>
#include "nanobench.h"
#include "fmt/core.h"

#include "symusic.h"

using namespace symusic;

extern template std::string TextMeta<Quarter>::to_string() const;


int main(){
    fmt::print("{}, {}\n", "Hello", "world");
    fmt::print("{0:d}, {0:s}\n", TextMeta<Quarter>(0.142, "Hello, world"));
    std::cout << Note<Tick>(1,2,3,4) << std::endl;
    std::cout << TextMeta<Quarter>(0.312, "Hello, world").to_string() << std::endl;

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

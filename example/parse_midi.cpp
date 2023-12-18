//
// Created by lyk on 23-12-17.
//
#include "symusic.h"
#include "fmt/core.h"
// get a filename from command line
int main(const int argc, char ** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    const std::string filename = argv[1];
    // convert to filesystem::path
    const std::filesystem::path path = filename;
    const auto score = symusic::io::parse<symusic::tag::Tick, symusic::tag::MIDI>(path);
    fmt::print("{}\n", score);
    const auto & t = score.tracks[0];
    fmt::print("{}\n", t);
    fmt::print("{}\n", std::span(t.notes.begin(), t.notes.begin() + 5));
    symusic::io::dump<symusic::tag::Tick, symusic::tag::MIDI>(score, "tmp.mid");
    return 0;
}
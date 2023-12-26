#include "symusic.h"
#include "fmt/core.h"
// get a filename from command line
using namespace symusic;

int main(const int argc, char ** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    const std::string filename = argv[1];
    // convert to filesystem::path
    const std::filesystem::path path = filename;
    const auto score = Score<Tick>::parse<DataFormat::MIDI>(read_file(filename));
    fmt::print("note_num: {}\n", score.note_num());
    const auto data = score.dumps<DataFormat::MIDI>();
    fmt::print("data size: {} KB\n", data.size() / 1024);
    return 0;
}
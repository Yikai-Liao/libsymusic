cmake_minimum_required(VERSION 3.20)
project(libsymusic)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_TEST "Build the test target" ON)

add_subdirectory(3rdparty/fmt EXCLUDE_FROM_ALL)

# create library target: libsymusic
add_library(libsymusic SHARED
        src/container.cpp
        src/format/common.cpp
        src/format/midi.cpp
        src/format/zpp.cpp
        src/utils.cpp
        src/conversion.cpp
)
target_include_directories(libsymusic PUBLIC ./include)
# expose headers
# link to 3rdparty
target_link_libraries(libsymusic PUBLIC fmt::fmt)
target_include_directories(libsymusic PUBLIC ./3rdparty/pdqsort)
target_include_directories(libsymusic PUBLIC ./3rdparty/minimidi/include)
target_include_directories(libsymusic PUBLIC ./3rdparty/zpp_bits)

if(BUILD_TEST)
    add_subdirectory(3rdparty/nanobench EXCLUDE_FROM_ALL)

    add_executable(hello example/hello.cpp)
    target_link_libraries(hello libsymusic)
    target_link_libraries(hello nanobench)

    add_executable(parse_midi example/parse_midi.cpp)
    target_link_libraries(parse_midi libsymusic)
endif()
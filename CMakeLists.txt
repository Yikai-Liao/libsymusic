cmake_minimum_required(VERSION 3.20)
project(libsymusic)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_TEST "Build the test target" ON)
option(ENABLE_LTO "Enables link-time optimization, requires compiler support." OFF)

# show compiler name and version
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

add_subdirectory(3rdparty/fmt EXCLUDE_FROM_ALL)
include_directories(include)

# create library target: libsymusic
file(GLOB_RECURSE libsymusic_src src/*.cpp src/io/*.cpp)

foreach(src_file ${libsymusic_src})
    message(STATUS "libsymusic_src: ${src_file}")
endforeach()

add_library(libsymusic STATIC ${libsymusic_src})
target_include_directories(libsymusic PUBLIC ./include)
# expose headers
# link to 3rdparty
target_link_libraries(libsymusic PUBLIC fmt::fmt-header-only)
target_include_directories(libsymusic PUBLIC  ./3rdparty/pdqsort)
target_include_directories(libsymusic PUBLIC  ./3rdparty/minimidi/include)
target_include_directories(libsymusic PUBLIC  ./3rdparty/zpp_bits)

if (ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT has_lto OUTPUT lto_check_output)
    if(has_lto)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            message(STATUS "Clang detected, use -fuse-ld=gold")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
        endif ()
        message(STATUS "Link-time optimization (LTO) enabled")
        set_property(TARGET libsymusic PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "Link-time optimization (LTO) is not supported: \n${lto_check_output}")
    endif()
endif()


if(BUILD_TEST)
    message(STATUS "Building test target")

    add_subdirectory(3rdparty/nanobench EXCLUDE_FROM_ALL)

    add_executable(hello example/hello.cpp)
    target_link_libraries(hello libsymusic)
    target_link_libraries(hello nanobench)

    add_executable(parse_midi example/parse_midi.cpp)
    target_link_libraries(parse_midi libsymusic)
endif()

# enable parallel compilation for MSVC
if(MSVC)
    message(STATUS "MSVC parallel compilation enabled")
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/Gm->)
    cmake_host_system_information(RESULT CPU_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP${CPU_NUMBER_OF_LOGICAL_CORES}>)
endif()
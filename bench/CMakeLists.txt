cmake_minimum_required(VERSION 3.20)
project(json-pointer-loc-bench)
set (CMAKE_CXX_STANDARD 11)

add_executable(
    json-pointer-loc-bench
    bench_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../json-pointer-loc.c
)
if(MSVC)
    target_compile_options(json-pointer-loc-bench "/W4" "$<$<CONFIG:RELEASE>:/O2>")
else()
    add_compile_options(json-pointer-loc-bnech "-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
endif()

target_include_directories(json-pointer-loc-bench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/nanobench/src/include)
target_include_directories(json-pointer-loc-bench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/sf-city-lots-json/citylots.json
    ${CMAKE_CURRENT_BINARY_DIR}/citylots.json
    COPYONLY
)

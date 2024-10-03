#define ANKERL_NANOBENCH_IMPLEMENT
#include <fstream>
#include <sstream>
#include <iostream>
#include "nanobench.h"
#include "json-pointer-loc.h"

int main() {
    std::ifstream f;
    f.exceptions ( std::ifstream::badbit );
    f.open("citylots.json", std::ios::binary);
    std::ostringstream iss;
    iss << f.rdbuf();
    std::string city_lots = iss.str();
    const char* city_lots_c = city_lots.c_str();
    size_t city_lots_sz = city_lots.size();
    // pointer to the very last scalar value in file
    const char* pointer = "/features/206559/geometry/coordinates/0/10/2";
    
    ankerl::nanobench::Bench().run("scan a large json file", [&] {
        json_pointer_loc_t loc;
        json_pointer_loc(city_lots_c, city_lots_sz, pointer, &loc);
        ankerl::nanobench::doNotOptimizeAway(loc);
    });
}
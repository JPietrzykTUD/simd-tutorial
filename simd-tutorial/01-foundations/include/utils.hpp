#pragma once

#include <iostream>
#define verify(expr) if (!(expr)) { std::cerr << "Verification failed: " << #expr << std::endl; std::exit(1); } else { std::cout << #expr << std::endl; }

#include <cstdint>
#include <cstddef>
#include <random>
#include <algorithm>

std::mt19937 rnd_generator {1337};

inline void fill(uint32_t * data, size_t element_count, uint32_t lower, uint32_t upper) {
    std::uniform_int_distribution<uint32_t> dist{lower, upper};
    std::generate(data, data + element_count, [&](){ return dist(rnd_generator); });
}


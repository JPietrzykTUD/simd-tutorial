#pragma once

#include <iostream>
#define verify(expr) if (!(expr)) { std::cerr << "Verification failed: " << #expr << std::endl; std::exit(1); }

#include <cstdint>
#include <cstddef>
#include <random>
#include <algorithm>

std::mt19937 rnd_generator {1337};

template<typename T>
inline void fill(T * data, size_t element_count, T lower, T upper) {
    std::uniform_int_distribution<T> dist{lower, upper};
    std::generate(data, data + element_count, [&](){ return dist(rnd_generator); });
}


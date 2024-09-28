// src/utils/Benchmark.hpp
#pragma once
#include <chrono>
#include <functional>

class Benchmark {
public:
    template<typename F>
    static long long measureTime(F func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
};
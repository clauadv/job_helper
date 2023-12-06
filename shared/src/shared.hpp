#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <filesystem>

using namespace std;

// src/macros
#define LOG(str, ...) \
    { \
        const auto filename = filesystem::path(__FILE__).filename().string(); \
        printf("[%s] " str "\n", filename.c_str(), __VA_ARGS__); \
    }

#define LOG_ERROR(str, ...) \
    { \
        const auto filename = filesystem::path(__FILE__).filename().string(); \
        printf("[%s:%d] " str "\n", filename.c_str(), __LINE__, __VA_ARGS__); \
    }

// src/includes
#include "color/color.hpp"
#include "math/vector.hpp"
#include "input/input.hpp"
#include "pixel/pixel.hpp"
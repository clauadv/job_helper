#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>

// src/macros
#define LOG_INFO(text, ...) printf("[info] " text "\n", __VA_ARGS__)
#define LOG_ERROR(text, ...) printf("[error] " text "\n", __VA_ARGS__)

// src/includes
#include "color/color.hpp"
#include "math/vector.hpp"
#include "input/input.hpp"
#include "pixel/pixel.hpp"
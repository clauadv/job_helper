// dllmain.cpp : Defines the entry point for the DLL application.
#include <src/shared.hpp>
#include "captcha/captcha.hpp"

bool main() {
    spdlog::info("job_helper::fish @ github.com/clauadv/job_helper");

    const auto device_context = GetDC(nullptr);
    if (!device_context) {
        spdlog::error("failed to get device_context");

        std::this_thread::sleep_for(std::chrono::seconds(5));

        return true;
    }

    const std::pair<int, int> screen_resolution{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    if (screen_resolution.first != 1920 || screen_resolution.second != 1080) {
        spdlog::error("screen resolution unsupported, feel free to change the positions tho");

        std::this_thread::sleep_for(std::chrono::seconds(5));

        return true;
    }

    spdlog::info("waiting for job interface");

    for (;;) {
        static bool resolved{ false };
        if (!resolved) {
            resolved = captcha::solve(device_context, screen_resolution);
        }

        if (resolved) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            const auto failed = captcha::failed(device_context, screen_resolution);
            if (failed) {
                spdlog::info("equipping fishing rod");
                shared::input::simulate_key(0x31);
            }

            spdlog::info("waiting 1 seconds before reeling the fishing rod again");
            std::this_thread::sleep_for(std::chrono::seconds(1));

            spdlog::info("simulating key space");
            shared::input::simulate_key(VK_SPACE);

            resolved = false;
        }

        static std::pair<int, int> marker{ 0, 0 };
        if (!marker.first) {
            marker = shared::pixel::find_position_in_rectangle(device_context, { 1920, 870 });
        }

        if (marker.first) {
            spdlog::info("simulating key space");
            shared::input::simulate_key(VK_SPACE);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            spdlog::info("reset marker");
            marker = { 0, 0 };
        }

        static std::pair<int, int> position{ 0, 0 };
        if (!position.first) {
            position = shared::pixel::find_position(device_context, { 812, 1027 }, { 300, 1 });
        }

        if (position.first) {
            const auto pixel = GetPixel(device_context, position.first, position.second);

            shared::color::c_color<int> color{ (pixel >> 16) & 0xff, (pixel >> 8) & 0xff, pixel & 0xff };
            if (color.r_between(221, 255) && color.g_between(150, 255) && color.b_between(200, 255)) {
                spdlog::info("simulating key space");
                shared::input::simulate_key(VK_SPACE);

                spdlog::info("reset position");
                position = { 0, 0 };

                spdlog::info("waiting 8 seconds before reeling the fishing rod again");
                std::this_thread::sleep_for(std::chrono::seconds(8));

                spdlog::info("simulating key space \n");
                shared::input::simulate_key(VK_SPACE);
            }
        }
    }

    ReleaseDC(nullptr, device_context);

    return false;
}
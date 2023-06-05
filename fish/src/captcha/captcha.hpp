#pragma once
#include <baseapi.h>
#include <opencv2/opencv.hpp>

namespace captcha {
    bool initialize() {
        HKEY key_handle{};
        const auto key_name = std::string{ "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment" };

        auto path = std::filesystem::path{ std::filesystem::current_path() / "tessdata" }.string();
        std::replace(path.begin(), path.end(), '\\', '/');

        const auto open_result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, key_name.c_str(), 0, KEY_ALL_ACCESS, &key_handle);
        if (open_result != ERROR_SUCCESS) {
            spdlog::error("failed to open registry key");
            return false;
        }

        const auto set_value_result = RegSetValueExA(key_handle, "TESSDATA_PREFIX", 0, REG_SZ, reinterpret_cast<const BYTE*>(path.c_str()), static_cast<unsigned long>(path.size() + 1));
        if (set_value_result != ERROR_SUCCESS) {
            spdlog::error("failed to set registry value");
            RegCloseKey(key_handle);

            return false;
        }

        const auto close_result = RegCloseKey(key_handle);
        if (close_result != ERROR_SUCCESS) {
            spdlog::error("failed to close registry key");
            return false;
        }

        spdlog::info("set tessdata_prefix to {}", path);

        return true;
    }

	inline bool solve(const HDC device_context, const std::pair<int, int> screen_resolution) {
        const auto pixel = GetPixel(device_context, 950, 410);

        shared::color::c_color<int> color{ (pixel >> 16) & 0xff, (pixel >> 8) & 0xff, pixel & 0xff };
        if (!(color.r_between(200, 250) && color.g_between(200, 250) && color.b_between(200, 234)))
            return false;

        spdlog::info("found captcha interface");

        tesseract::TessBaseAPI tesseract{};
        tesseract.Init(nullptr, "eng", tesseract::OEM_DEFAULT);
        tesseract.SetPageSegMode(tesseract::PSM_SINGLE_LINE);

        const auto device_context_target = CreateCompatibleDC(device_context);
        const auto handle_bitmap_target = CreateCompatibleBitmap(device_context, screen_resolution.first, screen_resolution.second);

        SelectObject(device_context_target, handle_bitmap_target);
        BitBlt(device_context_target, 0, 0, screen_resolution.first, screen_resolution.second, device_context, 0, 0, SRCCOPY);

        const auto frame = cv::Mat(screen_resolution.second, screen_resolution.first, CV_8UC4);
        auto bitmap_info = BITMAPINFO{ sizeof(BITMAPINFOHEADER), screen_resolution.first, -screen_resolution.second, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };
        GetDIBits(device_context_target, handle_bitmap_target, 0, screen_resolution.second, frame.data, &bitmap_info, DIB_RGB_COLORS);

        DeleteObject(handle_bitmap_target);
        DeleteDC(device_context_target);

        const auto screen_frame = frame({ 855, 555, 200, 40 });

        auto processed_image = cv::Mat{};
        cv::cvtColor(screen_frame, processed_image, cv::COLOR_BGR2GRAY);

        tesseract.SetImage(processed_image.data, processed_image.cols, processed_image.rows, 1, processed_image.cols);

        auto result = std::string{ tesseract.GetUTF8Text() };

        if (!result.empty()) {
            std::transform(result.begin(), result.end(), result.begin(), ::toupper);

            spdlog::info("setting cursor position");
            shared::input::simulate_click({ 850, 620 });

            spdlog::info("sending captcha {}", result);
            shared::input::send_text(std::wstring{ result.begin(), result.end() });

            spdlog::info("waiting 3 seconds before sending");
            std::this_thread::sleep_for(std::chrono::seconds(3));

            spdlog::info("pressing submit");
            shared::input::simulate_click({ 905, 695 });

            return true;
        }

        return false;
    }

    inline bool failed(const HDC device_context, const std::pair<int, int> screen_resolution) {
        const auto pixel = GetPixel(device_context, 44, 805);

        shared::color::c_color<int> color{ (pixel >> 16) & 0xff, (pixel >> 8) & 0xff, pixel & 0xff };
        if (!(color.r_between(220, 255) && color.g_between(220, 255) && color.b_between(220, 255)))
            return false;

        tesseract::TessBaseAPI tesseract{};
        tesseract.Init(nullptr, "eng", tesseract::OEM_DEFAULT);
        tesseract.SetPageSegMode(tesseract::PSM_SINGLE_LINE);

        const auto device_context_target = CreateCompatibleDC(device_context);
        const auto handle_bitmap_target = CreateCompatibleBitmap(device_context, screen_resolution.first, screen_resolution.second);

        SelectObject(device_context_target, handle_bitmap_target);
        BitBlt(device_context_target, 0, 0, screen_resolution.first, screen_resolution.second, device_context, 0, 0, SRCCOPY);

        const auto frame = cv::Mat(screen_resolution.second, screen_resolution.first, CV_8UC4);
        auto bitmap_info = BITMAPINFO{ sizeof(BITMAPINFOHEADER), screen_resolution.first, -screen_resolution.second, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };
        GetDIBits(device_context_target, handle_bitmap_target, 0, screen_resolution.second, frame.data, &bitmap_info, DIB_RGB_COLORS);

        DeleteObject(handle_bitmap_target);
        DeleteDC(device_context_target);

        const auto screen_frame = frame({ 218, 795, 65, 30 });

        auto processed_image = cv::Mat{};
        cv::cvtColor(screen_frame, processed_image, cv::COLOR_BGR2GRAY);

        tesseract.SetImage(processed_image.data, processed_image.cols, processed_image.rows, 1, processed_image.cols);

        const auto result = std::string{ tesseract.GetUTF8Text() };
        if (result.contains("captcha.")) {
            spdlog::info("captcha failed");
            return true;
        }
    }
}
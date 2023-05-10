#pragma once

namespace shared::pixel {
    inline std::pair<int, int> find_position(const HDC device_context, const std::pair<int, int> position, const std::pair<int, int> size) {
        const auto stride = size.first * 4;
        std::vector<std::uint8_t> pixels(stride);

        const auto compatible_dc = CreateCompatibleDC(device_context);
        const auto compatible_bitmap = CreateCompatibleBitmap(device_context, size.first, size.second);

        SelectObject(compatible_dc, compatible_bitmap);

        BitBlt(compatible_dc, 0, 0, size.first, size.second, device_context, position.first, position.second, SRCCOPY);
        GetBitmapBits(compatible_bitmap, stride, pixels.data());

        DeleteObject(compatible_bitmap);
        DeleteDC(compatible_dc);

        for (int i = 0; i < size.first; i++) {
            const auto pixel = *reinterpret_cast<std::uint32_t*>(pixels.data() + i * 4);

            shared::color::c_color<int> color{ (pixel >> 16) & 0xff, (pixel >> 8) & 0xff, pixel & 0xff };
            if (color.r_between(20, 50) && color.g_between(140, 220) && color.b_between(80, 150)) {
                spdlog::info("found color ({}, {}, {}) at ({}, {})", color.r, color.g, color.b, position.first + i + 15, position.second);
                return { position.first + i + 15, position.second };
            }
        }

        return { 0, 0 };
    }
}
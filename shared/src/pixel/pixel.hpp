#pragma once

namespace shared::pixel
{
    inline std::pair<int, int> find_position(const HDC device_context, const std::pair<int, int> position, const std::pair<int, int> size)
    {
        const auto stride = size.first * 4;
        std::vector<std::uint8_t> pixels(stride);

        const auto device_context_target = CreateCompatibleDC(device_context);
        const auto handle_bitmap_target = CreateCompatibleBitmap(device_context, size.first, size.second);

        SelectObject(device_context_target, handle_bitmap_target);

        BitBlt(device_context_target, 0, 0, size.first, size.second, device_context, position.first, position.second, SRCCOPY);
        GetBitmapBits(handle_bitmap_target, stride, pixels.data());

        DeleteObject(handle_bitmap_target);
        DeleteDC(device_context_target);

        for (int i = 0; i < size.first; i++)
        {
            const auto pixel = *reinterpret_cast<std::uint32_t*>(pixels.data() + i * 4);

            shared::color::c_color<int> color{ (pixel >> 16) & 0xff, (pixel >> 8) & 0xff, pixel & 0xff };
            if (color.r_between(20, 50) && color.g_between(140, 220) && color.b_between(80, 150))
            {
                spdlog::info("found color ({}, {}, {}) at ({}, {})", color.r, color.g, color.b, position.first + i + 15, position.second);
                return { position.first + i + 15, position.second };
            }
        }

        return { 0, 0 };
    }

    inline std::pair<int, int> find_position_in_rectangle(const HDC device_context, const std::pair<int, int> size)
    {
        const auto stride = size.first * 4;
        std::vector<std::uint8_t> pixels(stride * size.second);

        const auto device_context_target = CreateCompatibleDC(device_context);
        const auto handle_bitmap_target = CreateCompatibleBitmap(device_context, size.first, size.second);

        SelectObject(device_context_target, handle_bitmap_target);

        BitBlt(device_context_target, 0, 0, size.first, size.second, device_context, 0, 0, SRCCOPY);
        GetBitmapBits(handle_bitmap_target, stride * size.second, pixels.data());

        DeleteObject(handle_bitmap_target);
        DeleteDC(device_context_target);

        for (int y = 220; y < size.second; y++)
        {
            for (int x = 0; x < size.first; x++)
            {
                const auto pixel = *reinterpret_cast<std::uint32_t*>(pixels.data() + (y * stride) + (x * 4));

                shared::color::c_color<int> color{ (pixel >> 16) & 0xff, (pixel >> 8) & 0xff, pixel & 0xff };
                if (color.r_between(17, 22) && color.g_between(230, 240) && color.b_between(230, 240))
                {
                    spdlog::info("found color ({}, {}, {}) at ({}, {})", color.r, color.g, color.b, x + 15, y);
                    return { x + 15, y };
                }
            }
        }

        return { 0, 0 };
    }
}
#pragma once

namespace shared
{
    class c_pixel
    {
    public:
        static inline shared::c_vector2<int> find_marker_position(const HDC device_context, const shared::c_vector2<int> size)
        {
            const auto stride = size.x * 4;
            std::vector<std::uint8_t> pixels(stride * size.y);

            const auto device_context_target = CreateCompatibleDC(device_context);
            const auto handle_bitmap_target = CreateCompatibleBitmap(device_context, size.x, size.y);

            SelectObject(device_context_target, handle_bitmap_target);

            BitBlt(device_context_target, 0, 0, size.x, size.y, device_context, 0, 0, SRCCOPY);
            GetBitmapBits(handle_bitmap_target, stride * size.y, pixels.data());

            DeleteObject(handle_bitmap_target);
            DeleteDC(device_context_target);

            for (int y = 220; y < size.y; y++)
            {
                for (int x = 0; x < size.x; x++)
                {
                    const auto pixel = *reinterpret_cast<std::uint32_t*>(pixels.data() + (y * stride) + (x * 4));

                    auto color = shared::c_color<int>::get_pixel_color(pixel);
                    if (color.r_between(15, 30) && color.g_between(200, 215) && color.b_between(20, 35))
                    {
                        LOG_INFO("found color (%d, %d, %d) at (%d, %d)", color.r, color.g, color.b, x, y);
                        return { x, y };
                    }
                }
            }

            return shared::c_vector2<int>{ 0, 0 };
        }

        static shared::c_vector2<int> find_skillcheck_position(const HDC device_context, const shared::c_vector2<int> position, const shared::c_vector2<int> size)
        {
            const auto stride = size.x * 4;
            std::vector<std::uint8_t> pixels(stride);

            const auto device_context_target = CreateCompatibleDC(device_context);
            const auto handle_bitmap_target = CreateCompatibleBitmap(device_context, size.x, size.y);

            SelectObject(device_context_target, handle_bitmap_target);

            BitBlt(device_context_target, 0, 0, size.x, size.y, device_context, position.x, position.y, SRCCOPY);
            GetBitmapBits(handle_bitmap_target, stride, pixels.data());

            DeleteObject(handle_bitmap_target);
            DeleteDC(device_context_target);

            for (int i = 0; i < size.x; i++)
            {
                const auto pixel = *reinterpret_cast<std::uint32_t*>(pixels.data() + i * 4);

                // NOTE: here we are transforming RGB to BGR cuz idk why but im getting the color inversed when using RGB here
                auto color = shared::c_color<int>::get_pixel_color(pixel, true);
                if (color.r_between(20, 50) && color.g_between(140, 220) && color.b_between(80, 150))
                {
                    LOG_INFO("found color (%d, %d, %d) at (%d, %d)", color.r, color.g, color.b, position.x + i, position.y);
                    return { position.x + i + 15, position.y };
                }
            }

            return shared::c_vector2<int>{ 0, 0 };
        }
    };
}
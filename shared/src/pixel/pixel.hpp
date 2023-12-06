#pragma once

namespace shared
{
    class c_pixel
    {
    public:
        static c_vector2<int> find_marker_position(const HDC device_context, const ivector2& size)
        {
            const auto stride = size.m_x * 4;
            vector<uint8_t> pixels(stride * size.m_y);

            const auto device_context_target = CreateCompatibleDC(device_context);
            const auto handle_bitmap_target = CreateCompatibleBitmap(device_context, size.m_x, size.m_y);

            SelectObject(device_context_target, handle_bitmap_target);

            BitBlt(device_context_target, 0, 0, size.m_x, size.m_y, device_context, 0, 0, SRCCOPY);
            GetBitmapBits(handle_bitmap_target, stride * size.m_y, pixels.data());

            DeleteObject(handle_bitmap_target);
            DeleteDC(device_context_target);

            for (auto y{ 220 }; y < size.m_y; y++)
            {
                for (auto x{ 0 }; x < size.m_x; x++)
                {
                    const auto pixel = *reinterpret_cast<uint32_t*>(pixels.data() + (y * stride) + (x * 4));

                    auto color = icolor::get_pixel_color(pixel);
                    if (color.r_between(15, 30) && color.g_between(200, 215) && color.b_between(20, 35))
                    {
                        LOG("found color (%d, %d, %d) at (%d, %d)", color.m_r, color.m_g, color.m_b, x, y);
                        return { x, y };
                    }
                }
            }

            return ivector2{ 0, 0 };
        }

        static ivector2 find_skillcheck_position(const HDC device_context, const ivector2& position, const ivector2& size)
        {
            const auto stride = size.m_x * 4;
            vector<uint8_t> pixels(stride);

            const auto device_context_target = CreateCompatibleDC(device_context);
            const auto handle_bitmap_target = CreateCompatibleBitmap(device_context, size.m_x, size.m_y);

            SelectObject(device_context_target, handle_bitmap_target);

            BitBlt(device_context_target, 0, 0, size.m_x, size.m_y, device_context, position.m_x, position.m_y, SRCCOPY);
            GetBitmapBits(handle_bitmap_target, stride, pixels.data());

            DeleteObject(handle_bitmap_target);
            DeleteDC(device_context_target);

            for (auto idx{ 0 }; idx < size.m_x; idx++)
            {
                const auto pixel = *reinterpret_cast<uint32_t*>(pixels.data() + idx * 4);

                // NOTE: here we are transforming RGB to BGR cuz idk why but im getting the color inversed when using RGB here
                auto color = icolor::get_pixel_color(pixel, true);
                if (color.r_between(20, 50) && color.g_between(140, 220) && color.b_between(80, 150))
                {
                    LOG("found color (%d, %d, %d) at (%d, %d)", color.m_r, color.m_g, color.m_b, position.m_x + idx, position.m_y);
                    return { position.m_x + idx + 15, position.m_y };
                }
            }

            return ivector2{ 0, 0 };
        }
    };
}
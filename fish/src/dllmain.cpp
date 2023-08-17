// dllmain.cpp : Defines the entry point for the DLL application.
#include <src/shared.hpp>

bool main()
{
    LOG_INFO("job_helper::fish @ github.com/clauadv/job_helper");
    LOG_INFO("if you have any questions, https://discord.gg/K7RNp2vtVq \n");

    const auto device_context = GetDC(nullptr);
    if (!device_context)
    {
        LOG_ERROR("failed to get device_context");
        std::this_thread::sleep_for(std::chrono::seconds(5));

        return true;
    }

    SetProcessDPIAware();

    const auto screen_resolution = shared::c_vector2<int>{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    if (screen_resolution.x != 1920 || screen_resolution.y != 1080)
    {
        LOG_ERROR("your screen resolution (%d, %d) is unsupported", screen_resolution.x, screen_resolution.y);
        std::this_thread::sleep_for(std::chrono::seconds(5));

        return true;
    }

    LOG_INFO("waiting for job interface");

    for (;;)
    {
        static auto marker_position = shared::c_vector2<int>{ 0, 0 };
        if (marker_position.zero())
        {
            marker_position = shared::c_pixel::find_marker_position(device_context, shared::c_vector2<int>{ 1920, 870 });
        }

        if (!marker_position.zero())
        {
            LOG_INFO("simulating key space");
            shared::c_input::simulate_key(VK_SPACE);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            LOG_INFO("reset marker position");
            marker_position = { 0, 0 };
        }

        static auto skillcheck_position = shared::c_vector2<int>{ 0, 0 };
        if (skillcheck_position.zero())
        {
            skillcheck_position = shared::c_pixel::find_skillcheck_position(device_context, shared::c_vector2<int>{ 812, 1027 }, shared::c_vector2<int>{ 300, 1 });
        }

        if (!skillcheck_position.zero())
        {
            const auto pixel = GetPixel(device_context, skillcheck_position.x, skillcheck_position.y);

            auto color = shared::c_color<int>::get_pixel_color(pixel);
            if (color.r_between(200, 255) && color.g_between(150, 255) && color.b_between(221, 255))
            {
                LOG_INFO("simulating key space");
                shared::c_input::simulate_key(VK_SPACE);

                LOG_INFO("reset skillcheck position");
                skillcheck_position = { 0, 0 };

                LOG_INFO("waiting 8 seconds before reeling the fishing rod again");
                std::this_thread::sleep_for(std::chrono::seconds(8));

                LOG_INFO("simulating key space \n");
                shared::c_input::simulate_key(VK_SPACE);
            }
        }
    }

    ReleaseDC(nullptr, device_context);

    return false;
}

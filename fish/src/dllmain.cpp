// dllmain.cpp : Defines the entry point for the DLL application.
#include <src/shared.hpp>

bool main()
{
    LOG("job_helper::fish @ github.com/clauadv/job_helper");
    LOG("if you have any questions, https://discord.gg/K7RNp2vtVq \n");

    const auto device_context = GetDC(nullptr);
    if (!device_context)
    {
        LOG_ERROR("failed to get device_context");
        this_thread::sleep_for(chrono::seconds(5));

        return true;
    }

    SetProcessDPIAware();

    const auto screen_resolution = shared::ivector2{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    if (screen_resolution.m_x != 1920 || screen_resolution.m_y != 1080)
    {
        LOG_ERROR("your screen resolution (%d, %d) is unsupported", screen_resolution.m_x, screen_resolution.m_y);
        this_thread::sleep_for(chrono::seconds(5));

        return true;
    }

    LOG("waiting for job interface");

    for (;;)
    {
        static auto marker_position = shared::ivector2{ 0, 0 };
        if (marker_position.zero())
        {
            marker_position = shared::c_pixel::find_marker_position(device_context, shared::ivector2{ 1920, 870 });
        }

        if (!marker_position.zero())
        {
            LOG("simulating key space");
            shared::c_input::simulate_key(VK_SPACE);

            this_thread::sleep_for(chrono::milliseconds(500));

            LOG("reset marker position");
            marker_position = { 0, 0 };
        }

        static auto skillcheck_position = shared::ivector2{ 0, 0 };
        if (skillcheck_position.zero())
        {
            skillcheck_position = shared::c_pixel::find_skillcheck_position(device_context, shared::ivector2{ 812, 1027 }, shared::ivector2{ 300, 1 });
            LOG("found skillcheck position at (%d, %d)", skillcheck_position.m_x, skillcheck_position.m_y);
        }

        if (!skillcheck_position.zero())
        {
            const auto pixel = GetPixel(device_context, skillcheck_position.m_x, skillcheck_position.m_y);

            auto color = shared::icolor::get_pixel_color(pixel);
            if (color.r_between(200, 255) && color.g_between(150, 255) && color.b_between(221, 255))
            {
                LOG("simulating key space");
                shared::c_input::simulate_key(VK_SPACE);

                LOG("reset skillcheck position");
                skillcheck_position = { 0, 0 };

                LOG("waiting 8 seconds before reeling the fishing rod again");
                this_thread::sleep_for(chrono::seconds(8));

                LOG("simulating key space \n");
                shared::c_input::simulate_key(VK_SPACE);
            }
        }
    }

    ReleaseDC(nullptr, device_context);

    return false;
}

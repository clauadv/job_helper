// dllmain.cpp : Defines the entry point for the DLL application.
#include <src/shared.hpp>

enum class request_stages : int
{
	none,
	accept,
	second_accept,
	close,
	done
};

bool main()
{
	LOG("job_helper::uber @ github.com/clauadv/job_helper");
	LOG("if you have any questions, https://discord.gg/K7RNp2vtVq \n");

	const auto device_context = GetDC(nullptr);
	if (!device_context)
	{
		LOG_ERROR("failed to get device_context");
		this_thread::sleep_for(chrono::seconds(5));

		return true;
	}

	SetProcessDPIAware();

	// @todo: support all resolutions
	const auto screen_resolution = shared::ivector2{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	if (screen_resolution.m_x != 1920 || screen_resolution.m_y != 1080)
	{
		LOG_ERROR("your screen resolution (%d, %d) is unsupported", screen_resolution.m_x, screen_resolution.m_y);
		this_thread::sleep_for(chrono::seconds(5));

		return true;
	}

	LOG("waiting for uber interface");

	auto stage = request_stages::none;

    for (;;)
    {
        switch (stage)
        {
            case request_stages::none:
            {
                static shared::ivector2 position{ 1730, 960 };
                const auto pixel = GetPixel(device_context, position.m_x, position.m_y);

                auto color = shared::icolor::get_pixel_color(pixel);
                if (!(color.r_between(230, 240) && color.g_between(230, 240) && color.b_between(235, 245)))
                    continue;

                LOG("simulating left click on uber request");
                shared::c_input::simulate_click(position);

                this_thread::sleep_for(chrono::milliseconds(100));
                stage = request_stages::accept;
                break;
            }

            case request_stages::accept:
            {
                static shared::ivector2 position{ 1720, 1008 };
                const auto pixel = GetPixel(device_context, position.m_x, position.m_y);

                const auto color = shared::icolor::get_pixel_color(pixel);
                if (color != shared::icolor{ 1, 1, 1 })
                    continue;

                LOG("accepting uber request");
                shared::c_input::simulate_click(position);

                this_thread::sleep_for(chrono::milliseconds(100));
                stage = request_stages::second_accept;
                break;
            }

            case request_stages::second_accept:
            {
                static shared::ivector2 position{ 1730, 960 };
                const auto pixel = GetPixel(device_context, position.m_x, position.m_y);

                const auto color = shared::icolor::get_pixel_color(pixel);
                if (color != shared::icolor{ 245, 245, 245 })
                    continue;

                LOG("accepting uber request again");
                shared::c_input::simulate_click(position);

                this_thread::sleep_for(chrono::milliseconds(100));
                stage = request_stages::close;
                break;
            }

            case request_stages::close:
            {
                LOG("closing uber menu \n");
                shared::c_input::simulate_key(VK_ESCAPE);

                this_thread::sleep_for(chrono::milliseconds(100));
                stage = request_stages::done;
                break;
            }

            case request_stages::done:
            {
                stage = request_stages::none;
                this_thread::sleep_for(chrono::seconds(1));
                break;
            }
        }
    }

	ReleaseDC(nullptr, device_context);

	return false;
}
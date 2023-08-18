// dllmain.cpp : Defines the entry point for the DLL application.
#include <src/shared.hpp>

static enum class request_stages : int
{
	none,
	accept,
	second_accept,
	close,
	done
};

bool main()
{
	LOG_INFO("job_helper::uber @ github.com/clauadv/job_helper");
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

	LOG_INFO("waiting for uber interface");

	auto stage = request_stages::none;

    for (;;)
    {
        switch (stage)
        {
            case request_stages::none:
            {
                static shared::c_vector2<int> position{ 1730, 960 };
                const auto pixel = GetPixel(device_context, position.x, position.y);

                auto color = shared::c_color<int>::get_pixel_color(pixel);
                if (!(color.r_between(230, 240) && color.g_between(230, 240) && color.b_between(235, 245)))
                    continue;

                LOG_INFO("simulating left click on uber request");
                shared::c_input::simulate_click(position);

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                stage = request_stages::accept;
                break;
            }

            case request_stages::accept:
            {
                static shared::c_vector2<int> position{ 1720, 1008 };
                const auto pixel = GetPixel(device_context, position.x, position.y);

                const auto color = shared::c_color<int>::get_pixel_color(pixel);
                if (color != shared::c_color{ 1, 1, 1 })
                    continue;

                LOG_INFO("accepting uber request");
                shared::c_input::simulate_click(position);

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                stage = request_stages::second_accept;
                break;
            }

            case request_stages::second_accept:
            {
                static shared::c_vector2<int> position{ 1730, 960 };
                const auto pixel = GetPixel(device_context, position.x, position.y);

                const auto color = shared::c_color<int>::get_pixel_color(pixel);
                if (color != shared::c_color{ 245, 245, 245 })
                    continue;

                LOG_INFO("accepting uber request again");
                shared::c_input::simulate_click(position);

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                stage = request_stages::close;
                break;
            }

            case request_stages::close:
            {
                LOG_INFO("closing uber menu \n");
                shared::c_input::simulate_key(VK_ESCAPE);

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                stage = request_stages::done;
                break;
            }

            case request_stages::done:
            {
                stage = request_stages::none;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            }
        }
    }

	ReleaseDC(nullptr, device_context);

	return false;
}
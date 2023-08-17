// dllmain.cpp : Defines the entry point for the DLL application.
#include <src/shared.hpp>

bool main()
{
	LOG_INFO("job_helper::electrician @ github.com/clauadv/job_helper");
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
		const auto pixel = GetPixel(device_context, 761, 393);

		auto color = shared::c_color<int>::get_pixel_color(pixel);
		if (color.r_between(110, 140) && color.g_between(120, 150) && color.b_between(120, 160))
		{
			static std::vector<shared::c_vector2<int>> positions = {
				{ 757, 440 }, { 823, 440 }, { 889, 440 }, { 955, 440 }, { 1021, 440 }, { 1087, 440 }, { 1153, 440 },
				{ 757, 560 }, { 823, 560 }, { 889, 560 }, { 955, 560 }, { 1021, 560 }, { 1087, 560 }, { 1153, 560 }
			};

			for (const auto& position : positions)
			{
				LOG_INFO("simulating left click at (%d, %d)", position.x, position.y);
				shared::c_input::simulate_click(position);

				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
		}
	}

	ReleaseDC(nullptr, device_context);

	return false;
}
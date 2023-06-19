// dllmain.cpp : Defines the entry point for the DLL application.
#include <src/shared.hpp>

bool main()
{
	spdlog::info("job_helper::electrician @ github.com/clauadv/job_helper");

	const auto device_context = GetDC(nullptr);
	if (!device_context)
	{
		spdlog::error("failed to get device_context");

		std::this_thread::sleep_for(std::chrono::seconds(5));

		return true;
	}

	// @todo: add support for all resolutions
	const std::pair<int, int> screen_resolution{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	if (screen_resolution.first != 1920 || screen_resolution.second != 1080)
	{
		spdlog::error("screen resolution unsupported, feel free to change the positions tho");

		std::this_thread::sleep_for(std::chrono::seconds(5));

		return true;
	}

	spdlog::info("waiting for job interface");


	for (;;)
	{
		const auto pixel = GetPixel(device_context, 761, 393);

		shared::color::c_color<int> color{ (pixel >> 16) & 0xff, (pixel >> 8) & 0xff, pixel & 0xff };
		if (color.r_between(100, 160) && color.g_between(110, 150) && color.b_between(110, 140))
		{
			static std::vector<std::pair<int, int>> positions = {
				{ 757, 440 }, { 823, 440 }, { 889, 440 }, { 955, 440 }, { 1021, 440 }, { 1087, 440 }, { 1153, 440 },
				{ 757, 560 }, { 823, 560 }, { 889, 560 }, { 955, 560 }, { 1021, 560 }, { 1087, 560 }, { 1153, 560 }
			};

			for (const auto& position : positions)
			{
				spdlog::info("simulating left click at ({}, {})", position.first, position.second);
				shared::input::simulate_click(position);

				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
		}
	}

	ReleaseDC(nullptr, device_context);

	return false;
}
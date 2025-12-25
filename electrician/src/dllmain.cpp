// dllmain.cpp : Defines the entry point for the DLL application.
#include <src/shared.hpp>

bool main()
{
	LOG("job_helper::electrician @ github.com/clauadv/job_helper");
	LOG("if you have any questions, https://discord.gg/K7RNp2vtVq \n");

	SetProcessDPIAware();

	const auto device_context = GetDC(nullptr);
	if (!device_context)
	{
		LOG_ERROR("failed to get device_context");
		this_thread::sleep_for(chrono::seconds(5));

		return true;
	}

	struct dc_guard
	{
		HDC dc;
		~dc_guard() { if (dc) { ReleaseDC(nullptr, dc); } }
	};
	const dc_guard guard{ device_context };

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
		const auto interface_pixel = GetPixel(device_context, 873, 280);
		const auto interface_color = shared::icolor::get_pixel_color(interface_pixel);
		if (interface_color.m_r != 255 && interface_color.m_g != 255 && interface_color.m_b != 255)
			continue;

		const auto pixel = GetPixel(device_context, 761, 393);
	
		const auto color = shared::icolor::get_pixel_color(pixel);
		if (color.r_between(110, 140) && color.g_between(120, 150) && color.b_between(120, 160))
		{
			static const vector<shared::ivector2> positions = {
				{ 757, 440 }, { 823, 440 }, { 889, 440 }, { 955, 440 }, { 1021, 440 }, { 1087, 440 }, { 1153, 440 },
				{ 757, 560 }, { 823, 560 }, { 889, 560 }, { 955, 560 }, { 1021, 560 }, { 1087, 560 }, { 1153, 560 }
			};

			for (const auto& position : positions)
			{
				const auto pixel_position = GetPixel(device_context, position.m_x, position.m_y);

				const auto position_color = shared::icolor::get_pixel_color(pixel_position);
				if (position_color.r_between(152, 157) && position_color.g_between(160, 165) && position_color.b_between(170, 175))
					continue;
				
				LOG("simulating left click at (%d, %d)", position.m_x, position.m_y);
				shared::c_input::simulate_click(position);

				this_thread::sleep_for(chrono::milliseconds(750));
			}
		}
		else
		{
			this_thread::sleep_for(chrono::milliseconds(10));
		}
	}

	return false;
}
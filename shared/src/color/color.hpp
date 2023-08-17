#pragma once

namespace shared
{
	template<typename T>
	class c_color
	{
	public:
		T r, g, b;

		bool operator==(const shared::c_color<T>& color) const
		{
			return (this->r == color.r && this->g == color.g && this->b == color.b);
		}

		bool operator!=(const shared::c_color<T>& color) const
		{
			return !(operator==(color));
		}

		bool r_between(T min, T max)
		{
			return this->r >= min && this->r <= max;
		}

		bool g_between(T min, T max)
		{
			return this->g >= min && this->g <= max;
		}

		bool b_between(T min, T max)
		{
			return this->b >= min && this->b <= max;
		}

		static shared::c_color<T> get_pixel_color(const COLORREF pixel, const bool inverse = false)
		{
			if (inverse)
			{
				return shared::c_color<T>{ GetBValue(pixel), GetGValue(pixel), GetRValue(pixel) };
			}

			return shared::c_color<T>{ GetRValue(pixel), GetGValue(pixel), GetBValue(pixel) };
		}
	};
}
#pragma once

namespace shared
{
	template<typename T>
	class c_color
	{
	public:
		T m_r, m_g, m_b;

		bool operator==(const c_color<T>& color) const
		{
			return (this->m_r == color.m_r && this->m_g == color.m_g && this->m_b == color.m_b);
		}

		bool operator!=(const c_color<T>& color) const
		{
			return !(operator==(color));
		}

		bool r_between(T min, T max) const
		{
			return this->m_r >= min && this->m_r <= max;
		}

		bool g_between(T min, T max) const
		{
			return this->m_g >= min && this->m_g <= max;
		}

		bool b_between(T min, T max) const
		{
			return this->m_b >= min && this->m_b <= max;
		}

		static c_color<T> get_pixel_color(const COLORREF pixel, const bool inverse = false)
		{
			if (inverse)
			{
				return c_color<T>{ GetBValue(pixel), GetGValue(pixel), GetRValue(pixel) };
			}

			return c_color<T>{ GetRValue(pixel), GetGValue(pixel), GetBValue(pixel) };
		}
	};

	typedef c_color<int> icolor;
}
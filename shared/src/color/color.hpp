#pragma once

namespace shared::color {
	template<typename t>
	class c_color {
	public:
		t r, g, b;

		inline bool operator==(const shared::color::c_color<t>& color) const {
			return (this->r == color.r && this->g == color.g && this->b == color.b);
		}

		inline bool operator!=(const shared::color::c_color<t>& color) const {
			return !(operator==(color));
		}

		inline bool r_between(t min, t max) {
			return this->r >= min && this->r <= max;
		}

		inline bool g_between(t min, t max) {
			return this->g >= min && this->g <= max;
		}

		inline bool b_between(t min, t max) {
			return this->b >= min && this->b <= max;
		}
	};
}
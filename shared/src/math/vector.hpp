#pragma once

namespace shared
{
	template<typename T>
	class c_vector2
	{
	public:
		T x, y;

		bool zero() const
		{
			return (this->x == 0.f && this->y == 0.f);
		}
	};
}
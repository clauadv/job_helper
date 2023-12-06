#pragma once

namespace shared
{
	template<typename T>
	class c_vector2
	{
	public:
		T m_x, m_y;

		bool zero() const
		{
			return (this->m_x == 0.f && this->m_y == 0.f);
		}
	};

	typedef c_vector2<int> ivector2;
}
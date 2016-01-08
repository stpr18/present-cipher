#pragma once
#include <cstdint>
#include <limits>

namespace util {
	template <class INT>
	class BitUtil
	{
	public:
		BitUtil() = delete;

		using UnsignedInt = typename std::make_unsigned<INT>::type;
		static const int digits = std::numeric_limits<UnsignedInt>::digits;

		static constexpr UnsignedInt BitMask()
		{
			return ~UnsignedInt(0);
		}

		static constexpr UnsignedInt LowBitMask(std::size_t bit)
		{
			return ~(BitMask() << bit);
		}

		static constexpr UnsignedInt HighBitMask(std::size_t bit)
		{
			return ~(BitMask() >> bit);
		}
	};
}
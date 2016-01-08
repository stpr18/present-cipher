#pragma once
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <limits>
#include "util.h"

namespace util {

	template <class T>
	class HexPrint
	{
		static_assert(std::is_integral<T>::value, "T must be integral");

	private:
		const T& value_;
		const std::size_t size_;

	public:
		HexPrint() = delete;
		HexPrint(const T& value, std::size_t size = BitUtil<T>::digits) : value_(value), size_(size)
		{
		}

		friend std::ostream& operator <<(std::ostream& ros, HexPrint<T> manip)
		{
			return manip(ros);
		}

		std::ostream& operator()(std::ostream& os)
		{
			std::ios::fmtflags f(os.flags());
			os << std::hex;
			os << std::setfill('0') << std::setw(size_ / 4) << value_;
			os.flags(f);

			return os;
		}
	};

}

#pragma once
#include <cstdint>
#include <array>
#include <iostream>
#include <iomanip>

class Present
{
private:
	static const bool DEBUG;

	static const unsigned int kBlockSize = 64;
	static const unsigned int kRounds = 32;
	static const unsigned int kSboxBits = 4;
	static const unsigned int kSboxSize = 1 << kSboxBits;

	using SboxType = std::array<uint64_t, kSboxSize>;
	using PboxType = std::array<uint64_t, 64>;
	using RoundKeys = std::array<uint64_t, kRounds>;

	static const SboxType kSbox;
	static const PboxType kPbox;

public:
	using Block = uint64_t;

	struct Key
	{
		uint64_t high;
		uint64_t low;
	};

	class KeyPrint
	{
	private:
		const Key& key_;
		const std::size_t highsize_, lowsize_;

	public:
		KeyPrint() = delete;
		KeyPrint(const Key& key, std::size_t highsize, std::size_t lowsize);
		std::ostream& operator()(std::ostream& os);

		friend std::ostream& operator <<(std::ostream& ros, KeyPrint manip)
		{
			return manip(ros);
		}
	};

private:
	RoundKeys rkey_;

public:
	Present();
	~Present();

	static void GenerateRoundKeys80(Key key, RoundKeys& rkey);
	static Block Encrypt(Block plain, const RoundKeys& rkey);

	void GenerateRoundKeys80(Key key)
	{
		GenerateRoundKeys80(key, rkey_);
	}

	Block Encrypt(Block plain)
	{
		return Encrypt(plain, rkey_);
	}
};

//std::ostream& operator<<(std::ostream& os, const Present& present);

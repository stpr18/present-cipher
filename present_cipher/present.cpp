#include <stdexcept>
#include "present.h"
#include "util.h"
#include "hexprint.h"

const bool Present::DEBUG = false;

Present::Present()
{
}


Present::~Present()
{
}

void Present::GenerateRoundKeys80(Key key, RoundKeys& rkey)
{
	const unsigned int kHighSize = 64;
	const unsigned int kLowSize = 16;
	const unsigned int kShift = 19;
	const unsigned int kRoundAddTarget = 15;

	if (DEBUG) {
		if ((key.low >> kLowSize) != 0)
			throw std::invalid_argument("key.low is not 16bit integer");
	}

	if (DEBUG) {
		std::cout << "Start GenerateRoundKeys:" << KeyPrint(key, kHighSize, kLowSize) << std::endl;
	}

	for (uint64_t round = 0; round < kRounds; ) {
		//set round key
		rkey[round] = key.high;

		if (DEBUG) {
			std::cout << "Round" << std::setfill('0') << std::setw(2) << (round + 1) << ":" << util::HexPrint<uint64_t>(key.high) << std::endl;
		}

		//step1
		uint64_t keyhigh = key.high;
		key.high = (keyhigh >> kShift) ^ (keyhigh << (kHighSize + kLowSize - kShift)) ^ (key.low << (kHighSize - kShift));
		key.low = (keyhigh >> (kShift - kLowSize)) & util::BitUtil<uint64_t>::LowBitMask(kLowSize);

		if (DEBUG) {
			std::cout << "-AfterShift:" << KeyPrint(key, kHighSize, kLowSize) << std::endl;
		}

		//step2
		key.high = (kSbox[key.high >> (kHighSize - kSboxBits)] << (kHighSize - kSboxBits)) ^ (key.high & util::BitUtil<uint64_t>::LowBitMask(kHighSize - kSboxBits));

		if (DEBUG) {
			std::cout << "-AfterSbox:" << KeyPrint(key, kHighSize, kLowSize) << std::endl;
		}

		//increment round
		++round;

		//step3
		key.high ^= (round >> (kLowSize - kRoundAddTarget));
		key.low ^= ((round & util::BitUtil<uint64_t>::LowBitMask(kLowSize - kRoundAddTarget)) << kRoundAddTarget);

		if (DEBUG) {
			std::cout << "-AfterAddRound:" << KeyPrint(key, kHighSize, kLowSize) << std::endl;
		}
	}

	if (DEBUG) {
		std::cout << "End GenerateRoundKeys" << std::endl;
	}
}

void Present::GenerateRoundKeys128(Key key, RoundKeys& rkey)
{
	const unsigned int kHighSize = 64;
	const unsigned int kLowSize = 64;
	const unsigned int kShift = 67;
	const unsigned int kRoundAddTarget = 62;

	//if (DEBUG) {
	//	if ((key.low >> kLowSize) != 0)
	//		throw std::invalid_argument("key.low is not 16bit integer");
	//}

	if (DEBUG) {
		std::cout << "Start GenerateRoundKeys:" << KeyPrint(key, kHighSize, kLowSize) << std::endl;
	}

	for (uint64_t round = 0; round < kRounds; ) {
		//set round key
		rkey[round] = key.high;

		if (DEBUG) {
			std::cout << "Round" << std::setfill('0') << std::setw(2) << (round + 1) << ":" << util::HexPrint<uint64_t>(key.high) << std::endl;
		}

		//step1
		uint64_t keyhigh = key.high;
		key.high = (keyhigh << (kHighSize + kLowSize - kShift)) ^ (key.low >> (kShift - kHighSize));
		key.low = (key.low << (kHighSize + kLowSize - kShift)) ^ (keyhigh >> (kShift - kLowSize));

		if (DEBUG) {
			std::cout << "-AfterShift:" << KeyPrint(key, kHighSize, kLowSize) << std::endl;
		}

		//step2
		key.high =
			(kSbox[key.high >> (kHighSize - kSboxBits)] << (kHighSize - kSboxBits)) ^
			(kSbox[(key.high >> (kHighSize - kSboxBits * 2)) & util::BitUtil<uint64_t>::LowBitMask(kSboxBits)] << (kHighSize - kSboxBits * 2)) ^
			(key.high & util::BitUtil<uint64_t>::LowBitMask(kHighSize - kSboxBits * 2));

		if (DEBUG) {
			std::cout << "-AfterSbox:" << KeyPrint(key, kHighSize, kLowSize) << std::endl;
		}

		//increment round
		++round;

		//step3
		key.high ^= (round >> (kLowSize - kRoundAddTarget));
		key.low ^= ((round & util::BitUtil<uint64_t>::LowBitMask(kLowSize - kRoundAddTarget)) << kRoundAddTarget);

		if (DEBUG) {
			std::cout << "-AfterAddRound:" << KeyPrint(key, kHighSize, kLowSize) << std::endl;
		}
	}

	if (DEBUG) {
		std::cout << "End GenerateRoundKeys" << std::endl;
	}
}

Present::Block Present::Encrypt(Block plain, const RoundKeys& rkey)
{
	if (DEBUG) {
		std::cout << "Start Encrypt:" << util::HexPrint<Block>(plain) << std::endl;
	}

	Block& text = plain;
	for (uint_fast16_t round = 0; round < (kRounds - 1); ++round) {
		if (DEBUG) {
			std::cout << "Round" << std::setfill('0') << std::setw(2) << (round + 1) << ":" << util::HexPrint<Block>(rkey[round]) << std::endl;
		}

		//add round key
		text ^= rkey[round];

		if (DEBUG) {
			std::cout << "-After AddRoundKey:" << util::HexPrint<Block>(text) << std::endl;
		}

		//sbox layer
		for (uint_fast16_t i = 0; i < (kBlockSize / kSboxBits); ++i) {
			text = (text >> kSboxBits) ^ (kSbox[(text & ((1 << kSboxBits) - 1))] << (kBlockSize - kSboxBits));
		}

		if (DEBUG) {
			std::cout << "-After SboxLayer:" << util::HexPrint<Block>(text) << std::endl;
		}

		//p layer
		Block ptext = 0;
		for (auto p : kPbox) {
			ptext ^= (text & 0x1) << p;
			text >>= 1;
		}
		text = ptext;

		if (DEBUG) {
			std::cout << "-After PLayer:" << util::HexPrint<Block>(text) << std::endl;
		}
	}

	//last round
	if (DEBUG) {
		std::cout << "Last Round:" << util::HexPrint<Block>(rkey[kRounds - 1]) << std::endl;
	}

	text ^= rkey[kRounds - 1];

	if (DEBUG) {
		std::cout << "Last Round:" << util::HexPrint<Block>(rkey[kRounds - 1]) << std::endl;
	}


	if (DEBUG) {
		std::cout << "End Encrypt:" << util::HexPrint<Block>(text) << std::endl;
	}

	return text;
}

Present::KeyPrint::KeyPrint(const Key& key, std::size_t highsize, std::size_t lowsize) : key_(key), highsize_(highsize), lowsize_(lowsize)
{
}

std::ostream& Present::KeyPrint::operator()(std::ostream& os)
{
	os << util::HexPrint<uint64_t>(key_.high, highsize_) << " " << util::HexPrint<uint64_t>(key_.low, lowsize_);
	return os;
}

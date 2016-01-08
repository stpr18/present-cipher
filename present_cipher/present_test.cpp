#define BOOST_TEST_MODULE PresentTest
#include <boost/test/unit_test.hpp>
#include "present.h"

BOOST_AUTO_TEST_CASE(present_test)
{
	Present present;

	present.GenerateRoundKeys80({ 0x0000000000000000, 0x0000 });
	BOOST_CHECK_EQUAL(present.Encrypt(0x0000000000000000), 0x5579c1387b228445);

	present.GenerateRoundKeys80({ 0x0000000000000000, 0x0000 });
	BOOST_CHECK_EQUAL(present.Encrypt(0xffffffffffffffff), 0xa112ffc72f68417b);

	present.GenerateRoundKeys80({ 0xffffffffffffffff, 0xffff });
	BOOST_CHECK_EQUAL(present.Encrypt(0x0000000000000000), 0xe72c46c0f5945049);

	present.GenerateRoundKeys80({ 0xffffffffffffffff, 0xffff });
	BOOST_CHECK_EQUAL(present.Encrypt(0xffffffffffffffff), 0x3333dcd3213210d2);

	present.GenerateRoundKeys128({ 0x0000000000000000, 0x0000000000000000 });
	BOOST_CHECK_EQUAL(present.Encrypt(0x0000000000000000), 0x96db702a2e6900af);

	present.GenerateRoundKeys128({ 0x0000000000000000, 0x0000000000000000 });
	BOOST_CHECK_EQUAL(present.Encrypt(0xffffffffffffffff), 0x3c6019e5e5edd563);

	present.GenerateRoundKeys128({ 0xffffffffffffffff, 0xffffffffffffffff });
	BOOST_CHECK_EQUAL(present.Encrypt(0x0000000000000000), 0x13238c710272a5d8);

	present.GenerateRoundKeys128({ 0xffffffffffffffff, 0xffffffffffffffff });
	BOOST_CHECK_EQUAL(present.Encrypt(0xffffffffffffffff), 0x628d9fbd4218e5b4);
}

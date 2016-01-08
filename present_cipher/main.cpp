#include <cstdlib>
#include <exception>
#include "present.h"

int main()
{
	try {
		Present present;
		present.GenerateRoundKeys80({ 0xffffffffffffffff, 0xffff });
		present.Encrypt(0xffffffffffffffff);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "catch unknown exception" << std::endl;
	}

	std::system("pause");
	return EXIT_SUCCESS;
}

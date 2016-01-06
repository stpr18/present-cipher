#include <cstdlib>
#include "present.h"

int main()
{
	Present present;
	present.GenerateRoundKeys80({ 0xffffffffffffffff, 0xffff });
	present.Encrypt(0xffffffffffffffff);
	std::system("pause");
	return EXIT_SUCCESS;
}

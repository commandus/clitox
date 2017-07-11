/**
 * @file clitox.cpp
 */
#include <argtable2.h>
#include <string>
#include "clitox.h"

int main(int argc, char** argv)
{
	ClitoxConfig config(argc, argv);
	if (config.error())
		exit(config.error());
	return 0;
}

/**
 * @file fmtit.cpp
 */

#include <string>
#include "fmtit.h"

int main(int argc, char** argv)
{
	FmtitConfig config(argc, argv);
	if (config.error())
		exit(config.error());

	return 0;
}

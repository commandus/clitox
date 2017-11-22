/**
 * @file fmtit.cpp
 */

#include <string>
#include <iostream>
#include "fmtit.h"

int main(int argc, char** argv)
{
	FmtitConfig config(argc, argv);
	if (config.error())
		exit(config.error());

	std::cout << config.templateName << " ";
	for (int i = 0; i < config.arguments.size(); i++)
	{
		std::cout << config.arguments[i] << " ";
	}
	
	return 0;
}

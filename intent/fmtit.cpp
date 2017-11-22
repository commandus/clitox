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

	std::vector<IntentTemplate> its;
	load_json_files(its, config.templateFileNames);
	for (int i = 0; i < its.size(); i++)
	{
		std::cout << its[i].getId() << "\t" << its[i].getName() << std::endl;
	}
	return 0;
}

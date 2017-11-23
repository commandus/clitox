/**
 * @file fmtit.cpp
 */

#include <string>
#include <iostream>
#include "fmtit.h"
#include "intentformatter.h"

int main(int argc, char** argv)
{
	FmtitConfig config(argc, argv);
	if (config.error())
		exit(config.error());

	std::vector<IntentTemplate> its;
	load_json_files(its, config.templateFileNames);

	switch (config.cmd) {
		case 1:
			{
				// list intent templates
				for (int i = 0; i < its.size(); i++)
				{
					std::cout << its[i].getName();
					IntentTemplate it = its[i];
					std::vector<VariableColumn> vcs = it.getVariableColumnList();
					for (int a = 0; a < vcs.size(); a++)
					{
						std::cout << " " << vcs[a].getName();
					}
					std::cout << std::endl;
				}
			}
			break;
		default:
			// format
			{
				IntentFormatter fmt(&its);
				std::string s = fmt.format(config.templateName, config.arguments);
				std::cout << s << std::endl;
			}
			break;
	}
	return 0;
}

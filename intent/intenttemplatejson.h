#include <string>
#include <vector>

#include "intenttemplate.h"

int parse
(
	std::vector<IntentTemplate> &retval,
	const char* json
);

int parse_file
(
	std::vector<IntentTemplate> &retval,
	const std::string file_name
);

int load_json_nodes
(
	std::vector<IntentTemplate> &retval,
	const std::vector<std::string> &file_names
);

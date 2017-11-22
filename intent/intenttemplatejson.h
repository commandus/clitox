#include <string>
#include <vector>

#include "intenttemplate.h"

/**
 * @brief Parse JSON document
 * @param retval Return vector of IntentTemplate
 * @return count of added templates
 */
int parse
(
	std::vector<IntentTemplate> &retval,
	const char* json
);

/**
 * @brief Parse JSON file (not URIs)
 * @param retval Return vector of IntentTemplate
 * @return count of added templates
 */
int parse_file
(
	std::vector<IntentTemplate> &retval,
	const std::string file_name
);

/**
 * @brief Parse JSON files or URIs
 * @param retval Return vector of IntentTemplate
 * @return count of added templates
 */
int load_json_files
(
	std::vector<IntentTemplate> &retval,
	const std::vector<std::string> &file_names
);

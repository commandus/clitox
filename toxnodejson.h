#include <string>
#include <vector>
#include <tox/tox.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

int parse
(
	std::vector<struct DHT_node> &retval,
	const char* json
);

int parse_file
(
	std::vector<struct DHT_node> &retval,
	const std::string file_name
);

int load_json_nodes
(
	std::vector<struct DHT_node> &retval,
	const std::vector<std::string> &file_names
);

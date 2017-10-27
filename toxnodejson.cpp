#include <fstream>
#include "toxnodejson.h"
#include "toxclient.h"

using namespace rapidjson;

int parse
(
	std::vector<struct DHT_node> &retval,
	const char* json
)
{
	Document d;
	d.Parse(json);
	Value &nodes = d["nodes"];
	if (nodes.IsArray())
	{
		for (SizeType i = 0; i < nodes.Size(); i++)
		{
			Value &node_ip = nodes[i]["ipv4"];
			Value &node_port = nodes[i]["port"];
			Value &node_key = nodes[i]["public_key"];
			if ((!node_ip.IsString()) || (!node_port.IsInt()) || (!node_key.IsString()))
				continue;
			struct DHT_node dn = {node_ip.GetString(), static_cast<uint16_t>(node_port.GetInt()), node_key.GetString()};
			retval.push_back(dn);
		}
	}
	return 0;
}

static std::string file2string(std::istream &strm)
{
	if (!strm)
		return "";
	return std::string((std::istreambuf_iterator<char>(strm)), std::istreambuf_iterator<char>());
}

static std::string file2string(const char *filename)
{
	if (!filename)
		return "";
	std::ifstream t(filename);
	return file2string(t);
}

static std::string file2string(const std::string &filename)
{
	return file2string(filename.c_str());
}

int load_json_nodes
(
	std::vector<struct DHT_node> &retval,
	const std::vector<std::string> &file_names
)
{
	for (std::vector<std::string>::const_iterator it(file_names.begin()); it != file_names.end(); ++it)
	{
		std::string json = file2string(*it);
		if (json.empty())
		{
			// Try load from Internet
		}
		parse(retval, json.c_str());
	}
}

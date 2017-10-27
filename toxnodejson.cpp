#include <fstream>
#include <curl/curl.h>
#include "toxnodejson.h"
#include "toxclient.h"

using namespace rapidjson;

int parse
(
	std::vector<struct DHT_node> &retval,
	const char *json
)
{
	if (!json)
		return 1;
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

static std::string file2string
(
	const std::string &filename
)
{
	return file2string(filename.c_str());
}


/**
  * @brief CURL write callback
  */
static size_t write_string(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static std::string url2string
(
	const std::string &filename
)
{
	std::string retval;
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (!curl) 
		return "";
	curl_easy_setopt(curl, CURLOPT_URL, filename.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &retval);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return retval;
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
			json = url2string(*it);
		}
		parse(retval, json.c_str());
	}
	return 0;
}

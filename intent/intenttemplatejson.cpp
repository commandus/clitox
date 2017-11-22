#include <fstream>
#include <curl/curl.h>

#include "intenttemplate.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

int parse
(
	std::vector<IntentTemplate> &retval,
	const char *json
)
{
	if (!json)
		return 1;
	Document d;
	d.Parse(json);
	
	Value &variables = d["variables"];
	std::vector<VariableColumn> vcs;
	if (variables.IsArray())
	{
		for (SizeType i = 0; i < variables.Size(); i++)
		{
			Value &name = variables[i]["name"];
			Value &hint = variables[i]["hint"];
			Value &predefined = variables[i]["predefined"];
			std::string n;
			if (name.IsString())
				n = name.GetString();
			std::string h;
			if (hint.IsString())
				h = hint.GetString();
			VariableColumn vc;
			vcs.push_back(vc);
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
 

int load_json_files
(
	std::vector<IntentTemplate> &retval,
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

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
	int r = 0;
	if (!json)
		return r;
	Document d;
	d.Parse(json);

	// load types list. Useless.
	std::vector<Variable> args;
	if (d.HasMember("types"))
	{
		Value &types = d["types"];
		if (types.IsArray())
		{
			for (SizeType i = 0; i < types.Size(); i++)
			{
				Variable v;
				v.setId(i);

				Value &typ = types[i];
				if (typ.HasMember("name"))
				{
					Value &name = typ["name"];
					if (name.IsString())
						v.setName(name.GetString());
				}
				std::string h;
				if (typ.HasMember("hint"))
				{
					Value &hint = typ["hint"];
					if (hint.IsString())
						v.setHint(hint.GetString());
				}
				
				args.push_back(v);
			}
		}
	}
	
	// load templates
	if (d.HasMember("templates"))
	{
		Value &templates = d["templates"];
		if (templates.IsArray())
		{
			for (SizeType i = 0; i < templates.Size(); i++)
			{
				IntentTemplate v;
				v.setId(i);

				Value &intenttemplate = templates[i];
				if (intenttemplate.HasMember("lang"))
				{
					Value &val = intenttemplate["lang"];
					if (val.IsString())
						v.setLang(val.GetString());
				}
				if (intenttemplate.HasMember("name"))
				{
					Value &val = intenttemplate["name"];
					if (val.IsString())
						v.setName(val.GetString());
				}
				if (intenttemplate.HasMember("description"))
				{
					Value &val = intenttemplate["description"];
					if (val.IsString())
						v.setDescription(val.GetString());
				}
				if (intenttemplate.HasMember("template"))
				{
					Value &val = intenttemplate["template"];
					if (val.IsString())
						v.setTemplateString(val.GetString());
				}

				if (intenttemplate.HasMember("variables"))
				{
					Value &variables = intenttemplate["variables"];
					if (variables.IsArray())
					{
						std::vector<VariableColumn> vcs;
						for (SizeType j = 0; j < variables.Size(); j++)
						{
							Value &variable = variables[j];
							VariableColumn vc;
							if (variable.HasMember("column"))
							{
								Value &val = variable["column"];
								if (val.IsInt())
									vc.setColumn(val.GetInt());
							}
							if (variable.HasMember("type-index"))
							{
								Value &val = variable["type-index"];
								if (val.IsInt())
									vc.setIdx(val.GetInt());
							}
							if (variable.HasMember("name"))
							{
								Value &val = variable["name"];
								if (val.IsString())
									vc.setName(val.GetString());
							}
							if (variable.HasMember("length"))
							{
								Value &val = variable["length"];
								if (val.IsInt())
									vc.setLen(val.GetInt());
							}
							if (variable.HasMember("encode"))
							{
								Value &val = variable["encode"];
								if (val.IsBool())
									vc.setEncode(val.GetBool());
							}
							vcs.push_back(vc);
						}
						v.setVariableColumnList(vcs);
					}
				}
				retval.push_back(v);
				r++;
			}
		}
	}
	return r;
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
	int r = 0;
	for (std::vector<std::string>::const_iterator it(file_names.begin()); it != file_names.end(); ++it)
	{
		std::string json = file2string(*it);
		if (json.empty())
		{
			// Try load from Internet
			json = url2string(*it);
		}
		r += parse(retval, json.c_str());
	}
	return r;
}


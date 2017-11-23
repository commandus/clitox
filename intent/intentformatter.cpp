#include "intentformatter.h"
#include "variablecolumn.h"

#include <cctype>
#include <iomanip>
#include <sstream>

IntentFormatter::IntentFormatter()
: intentTemplates(NULL)
{

}

IntentFormatter::IntentFormatter
(
	std::vector<IntentTemplate> *its
)
	: intentTemplates(its)
{
}

IntentFormatter::IntentFormatter(const IntentFormatter& other)
{
	intentTemplates = other.intentTemplates;
}

IntentFormatter::~IntentFormatter()
{

}

IntentFormatter& IntentFormatter::operator=(const IntentFormatter& other)
{
	intentTemplates = other.intentTemplates;
}

bool IntentFormatter::operator==(const IntentFormatter& other) const
{
	return intentTemplates != other.intentTemplates;
}

/**
 * @brief find out intent template by the name
 * @return found IntentTemplate or NULL if not found
 */
IntentTemplate *IntentFormatter::find
(
	const std::string &templateName
)
{
	if (!intentTemplates)
		return NULL;
	IntentTemplate *it = NULL;
	for (int a = 0; a < intentTemplates->size(); a++)
	{
		if (intentTemplates->at(a).getName() == templateName)
		{
			return &intentTemplates->at(a);
		}
	}
	return NULL;
}

/**
 * @brief Encode URL
 * @see https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
 */
std::string urlEncode
(
	const std::string &value
) 
{
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;
	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) 
	{
		std::string::value_type c = (*i);
		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') 
		{
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char) c);
		escaped << std::nouppercase;
	}
	return escaped.str();
}

/**
	* Fill template with variable values
	* @return Intent string from IntentTemplate with variable's values
	* @see #setVariableValueByIndex(int, Object)
	*/

std::string IntentFormatter::format
(
	IntentTemplate *intentTemplate,
	const std::vector<std::string> &arguments
)
{
	if (!intentTemplate)
		return "";

	std::string stemplate = intentTemplate->getTemplateString();

	std::string v;
	std::stringstream ss;
	int start = 0;
	int finish;
	int i = 0;
	const std::vector<VariableColumn> &vcs = intentTemplate->getVariableColumnList();
	
	for (int i = 0; i < vcs.size(); i++) 
	{
		VariableColumn vc = vcs[i];
		finish = vc.getColumn();
		ss << stemplate.substr(start, finish - start);
		std::string v;
		if (i < arguments.size())
			v = arguments[i];
		else
			v = "";
		if (vc.isEncode())
		{
			v = urlEncode(v);
		}
		ss << v;
		start = finish;
	}
	if (start < stemplate.length())
		ss << stemplate.substr(start);
	return ss.str();
}

std::string IntentFormatter::format
(
	const std::string &templateName, 
	const std::vector<std::string> &arguments
)
{
	if (!intentTemplates)
		return "";
	// find out intent template
	IntentTemplate *it = find(templateName);
	if (!it)
		return "";
	return format(it, arguments);
}

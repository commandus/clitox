/**
 * Intent template class
 */

#include <sstream>
#include "intenttemplate.h"

IntentTemplate::IntentTemplate
(
	const std::string &templateString,
	const std::string &name,
	const std::string &lang,
	const std::string &description,
	const std::vector<VariableColumn> &variableColumnList,
	const std::string &imageUri
) 
{
	this->templateString = templateString;
	this->name = name;
	this->lang = lang;
	this->description = description;
	this->variableColumnList = variableColumnList;
	this->imageUri = imageUri;
}


std::string IntentTemplate::getTemplateString() 
{
	return templateString;
}

void IntentTemplate::setTemplateString(const std::string &templateString) 
{
	this->templateString = templateString;
}

std::string IntentTemplate::getName() 
{
	return name;
}

void IntentTemplate::setName(const std::string &name) 
{
	this->name = name;
}

std::string IntentTemplate::getLang() 
{
	return lang;
}

void IntentTemplate::setLang(const std::string &lang) 
{
	this->lang = lang;
}

std::string IntentTemplate::getDescription() 
{
	return description;
}

void IntentTemplate::setDescription(const std::string &description) 
{
	this->description = description;
}

int IntentTemplate::getId() 
{
	return id;
}

void IntentTemplate::setId(int value) 
{
	id = value;
}

std::vector<VariableColumn> IntentTemplate::getVariableColumnList() {
	return variableColumnList;
}

void IntentTemplate::setVariableColumnList(const std::vector<VariableColumn> &variableColumnList) {
	this->variableColumnList = variableColumnList;
}

std::string IntentTemplate::getImageUri() 
{
	return imageUri;
}

void IntentTemplate::setImageUri(const std::string &imageUri) 
{
	this->imageUri = imageUri;
}

/**
	* Return string ready to parse with {@link IntentTemplateHelper#parse(std::string)}
	* Separators: '|'- intent template attrs, ','- VariableColumn list separator, ':'- VariableColumn attribute separator
	* where 0- lang 1-image uri 2- name, 3- description, 4- template string, 5..- "," separated column:variable:name:len:e index pairs array
	* lang|img|name|desc|template|column:variable:name:len:e,... e.g. 0:15:0:0:e,4:16:0:0:e
	* Special intent extra e.g.
	*      S.com.commandus.intent.fix.array=android.intent.extra.EMAIL,android.intent.extra.SUBS
	* @return std::string
	*/
std::string IntentTemplate::toString() 
{
	std::stringstream b;
	b << getLang() << SEPARATOR_1
			<< getImageUri() << SEPARATOR_1
			<< getName() << SEPARATOR_1
			<< getDescription() << SEPARATOR_1
			<< getTemplateString() << SEPARATOR_1;
	int sz1 = getVariableColumnList().size() - 1;
	int i = 0;
	for (VariableColumn vc: getVariableColumnList()) {
		b << vc.toString();
		if (i < sz1)
			b << SEPARATOR_LIST;
		i++;
	}
	return b.str();
}

/**
 * Intent template class
 * @file intenttemplate.h
 */
#ifndef INTENT_TEMPLATE_H
#define INTENT_TEMPLATE_H

#include <string>
#include <vector>
#include "variablecolumn.h"

const char SEPARATOR_1 = '|';
const char SEPARATOR_LIST = ',';
    
class IntentTemplate 
{
private:
	std::string templateString;
	std::string name;
	std::string lang;
	std::string description;
	std::vector<VariableColumn> variableColumnList;
	std::string imageUri;
	int id; // database only

public:
	IntentTemplate();
	IntentTemplate(
		const std::string &templateString,
		const std::string &name,
		const std::string &lang,
		const std::string &description,
		const std::vector<VariableColumn> &variableColumnList,
		const std::string &imageUri
	);
	
	std::string getTemplateString();

	void setTemplateString(const std::string &templateString);

	std::string getName();

	void setName(const std::string &name);

	std::string getLang();

	void setLang(const std::string &lang);

	std::string getDescription();

	void setDescription(const std::string &description);

	std::vector<VariableColumn> getVariableColumnList();

	void setVariableColumnList(const std::vector<VariableColumn> &variableColumnList);

	std::string getImageUri();

	void setImageUri(const std::string &imageUri);

	/**
	* Return string ready to parse with {@link IntentTemplateHelper#parse(std::string)}
	* Separators: '|'- intent template attrs, ','- VariableColumn list separator, ':'- VariableColumn attribute separator
	* where 0- lang 1-image uri 2- name, 3- description, 4- template string, 5..- "," separated column:variable:name:len:e index pairs array
	* lang|img|name|desc|template|column:variable:name:len:e,... e.g. 0:15:0:0:e,4:16:0:0:e
	* Special intent extra e.g.
	*      S.com.commandus.intent.fix.array=android.intent.extra.EMAIL,android.intent.extra.SUBS
	* @return std::string
	*/
	std::string toString();

	int getId();

	void setId(int value);
};

#endif

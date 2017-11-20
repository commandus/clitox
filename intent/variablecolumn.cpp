#include <sstream>
#include "variablecolumn.h"

static const char SEPARATOR_2 = ':';

/**
 * Variable at column of IntentTemplate
 * @see IntentTemplateHelper
 */
VariableColumn::VariableColumn
(
	enum Variable variable,
	int column,
	int idx,
	const std::string &name,
	int len,
	bool encode
)
{
	this->variable = variable;
	this->column = column;
	this->idx = idx;
	this->name = name;
	this->len = len;
	this->encode = encode;
}

enum Variable VariableColumn::getVariable() {
	return variable;
}

void VariableColumn::setVariable(enum Variable variable) {
	this->variable = variable;
}

int VariableColumn::getColumn() {
	return column;
}

void VariableColumn::setColumn(int column) {
	this->column = column;
}

int VariableColumn::getIdx() {
	return idx;
}

void VariableColumn::setIdx(int idx) {
	this->idx = idx;
}

std::string VariableColumn::getName() {
	return name;
}

void VariableColumn::setName(const std::string &name) {
	this->name = name;
}

int VariableColumn::getLen() {
	return len;
}

void VariableColumn::setLen(int len) {
	this->len = len;
}

bool VariableColumn::isEncode() {
	return encode;
}

void VariableColumn::setEncode(bool encode) {
	this->encode = encode;
}

/**
	* Return string ready to parse with {@link IntentTemplateHelper#parse(std::string)}
	* Called from {@link IntentTemplate#toString()}
	* Separators: '|'- intent template attrs, ','- VariableColumn list separator, ':'- VariableColumn attribute separator
	* where 0- lang 1-image uri 2- name, 3- description, 4- template string, 5..- "," separated column:variable:name:len:e index pairs array
	* lang|img|name|desc|template|column:variable:name:len:e,... e.g. 0:15:0:0:e,4:16:0:0:e
	* Special intent extra e.g.
	*      S.com.commandus.intent.fix.array=android.intent.extra.EMAIL,android.intent.extra.SUBS
	* @return as string
	* @see IntentTemplate#toString()
	*/
std::string VariableColumn::toString() 
{
	std::stringstream ss;
	ss << getColumn() << SEPARATOR_2 
		<< (int) getVariable() << SEPARATOR_2
		<< getName() << SEPARATOR_2
		<< getLen() << SEPARATOR_2
		<< (isEncode() ? "e" : "");
	return ss.str();
}

	// copy assignment
VariableColumn& VariableColumn::operator=(const VariableColumn& other)
{
	// self-assignment check expected
	if (this != &other) 
	{
		variable = other.variable;
		column = other.column;
		idx = other.idx;
		name = other.name;
		len = other.len;
		encode = other.encode;
	}
	return *this;
}

bool VariableColumn::operator==(const VariableColumn& other) const
{
    if (variable != other.variable)
        return false;
    if (column != other.column)
        return false;
    if (idx != other.idx)
        return false;
    if (name != other.name)
        return false;
    if (len != other.len)
        return false;
    if (encode != other.encode)
        return false;
    return true;
}

/**
 * column:variable:name:len:e
 */
VariableColumn::VariableColumn(const std::string &value)
{
    int i = 0;
    std::size_t b = 0;
    for (std::size_t it(value.find(':')); it != std::string::npos; ++it)
    {
        std::string s(b, it - b);
        switch (i) 
        {
            case 0:
                column = strtod(s.c_str(), NULL);
                break;
            case 1:
                variable = strtod(s.c_str(), NULL);
                break;
            case 2:
                name = s;
                break;
            case 3:
                len = strtod(s.c_str(), NULL);
                break;
            case 4:
                encode = s.find("e") != s.end();
                break;
            default:
                break;
        }
        i++;
    }
}

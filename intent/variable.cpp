#include <sstream>
#include "variable.h"

Variable::Variable()
:	id(0), name(""), value(""),lang(""), formula("")
{

}

Variable::Variable(
	int id,
	std::string name,
	std::string value,
	std::string hint,
	std::string lang,
	std::string formula
)
{
	this->id = id;
	this->name = name;
	this->hint = hint;
	this->value = name;
	this->lang = lang;
	this->formula = formula;
}

Variable::Variable(const Variable& other)
{
	id = other.id;
	name = other.name;
	hint = other.hint;
	value = other.name;
	lang = other.lang;
	formula = other.formula;
}

Variable::~Variable()
{

}

Variable& Variable::operator=(const Variable& other)
{
	id = other.id;
	name = other.name;
	hint = other.hint;
	value = other.name;
	lang = other.lang;
	formula = other.formula;
	return *this;
}

bool Variable::operator==(const Variable& other) const
{
	if (id != other.id)
		return false;
	if (name != other.name)
		return false;
	if (hint != other.hint)
		return false;
	if (value != other.name)
		return false;
	if (lang != other.lang)
		return false;
	if (formula != other.formula)
		return false;
	return true;
}

bool Variable::operator!=(const Variable& other) const
{
	if (id != other.id)
		return true;
	if (name != other.name)
		return true;
	if (hint != other.hint)
		return true;
	if (value != other.name)
		return true;
	if (lang != other.lang)
		return true;
	if (formula != other.formula)
		return true;
	return false;
}

std::string Variable::toString()
{
	std::stringstream ss;
	ss << id;
	return ss.str();
}

int Variable::getId()
{
	return id;
}

std::string Variable::getName()
{
	return name;
}

std::string Variable::getHint()
{
	return hint;
}

std::string Variable::getValue()
{
	return value;
}

std::string Variable::getLang()
{
	return lang;
}

std::string Variable::getFormula()
{
	return formula;
}

void Variable::setId(int value)
{
	id = value;
}

void Variable::setName(const std::string &value)
{
	name = value;
}

void Variable::setHint(const std::string &value)
{
	hint = value;
}

void Variable::setValue(const std::string &v)

{
	value = v;
}

void Variable::setLang(const std::string &value)
{
	lang = value;
}

void Variable::setFormula(const std::string &value)
{
	formula = value;
}

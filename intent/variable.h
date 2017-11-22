#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

class Variable
{
private:
	int id;
	std::string name;
	std::string hint;
	std::string value;
	std::string lang;
	std::string formula;
public:
	Variable();
	Variable(
		int id,
		std::string name,
		std::string hint,
		std::string value,
		std::string lang,
		std::string formula
	);
	
	Variable(const Variable& other);
	~Variable();
	Variable& operator=(const Variable& other);
	bool operator==(const Variable& other) const;
	bool operator!=(const Variable& other) const;
	std::string toString();

	int getId();
	std::string getName();
	std::string getHint();
	std::string getValue();
	std::string getLang();
	std::string getFormula();

	void setId(int value);
	void setName(const std::string &value);
	void setHint(const std::string &value);
	void setValue(const std::string &value);
	void setLang(const std::string &value);
	void setFormula(const std::string &value);
};

#endif // VARIABLE_H

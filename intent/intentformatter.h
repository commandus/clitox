#ifndef INTENTFORMATTER_H
#define INTENTFORMATTER_H

#include <vector>
#include <string>

#include "intenttemplate.h"

class IntentFormatter
{
private:
	std::vector<IntentTemplate> *intentTemplates;
public:
	IntentFormatter();
	IntentFormatter(std::vector<IntentTemplate> *its);
	IntentFormatter(const IntentFormatter& other);
	~IntentFormatter();
	IntentFormatter& operator=(const IntentFormatter& other);
	bool operator==(const IntentFormatter& other) const;
	
	IntentTemplate *find
	(
		const std::string &templateName
	);
	
	static std::string format
	(
		IntentTemplate *intentTemplate,
		const std::vector<std::string> &arguments
	);
	
	std::string format(
		const std::string &templateName, 
		const std::vector<std::string> &arguments
	);
};

#endif // INTENTFORMATTER_H

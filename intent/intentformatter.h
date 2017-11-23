#ifndef INTENTFORMATTER_H
#define INTENTFORMATTER_H

#include <vector>
#include "intenttemplate.h"

class IntentFormatter
{
private:
	const std::vector<IntentTemplate> *intentTemplates;
public:
	IntentFormatter();
	IntentFormatter(const std::vector<IntentTemplate> *its);
	IntentFormatter(const IntentFormatter& other);
	~IntentFormatter();
	IntentFormatter& operator=(const IntentFormatter& other);
	bool operator==(const IntentFormatter& other) const;
};

#endif // INTENTFORMATTER_H

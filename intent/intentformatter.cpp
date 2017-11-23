#include "intentformatter.h"

IntentFormatter::IntentFormatter()
: intentTemplates(NULL)
{

}

IntentFormatter::IntentFormatter
(
	const std::vector<IntentTemplate> *its
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

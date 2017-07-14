#ifndef TOXMESSAGE_H
#define TOXMESSAGE_H

#include <string>
#include <tox/tox.h>

class ToxMessage
{
public:
	int friend_number;
	TOX_MESSAGE_TYPE message_type;
	std::string message;
	ToxMessage();
	ToxMessage
	(
		int friend_number,
		TOX_MESSAGE_TYPE message_type,
		const std::string &message
	);
	~ToxMessage();
};

#endif // TOXMESSAGE_H

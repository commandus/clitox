#include "toxmessage.h"

ToxMessage::ToxMessage()
{

}

ToxMessage::ToxMessage
(
	int afriend_number,
	TOX_MESSAGE_TYPE amessage_type,
	const std::string &amessage
)
	: friend_number(afriend_number), message_type(amessage_type), message(amessage)
{
}

ToxMessage::~ToxMessage()
{

}

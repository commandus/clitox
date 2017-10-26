#include "toxreceiverstream.h"

ToxReceiverStream::ToxReceiverStream
(
	std::istream &i_stream,
	std::ostream &o_stream,
	std::ostream &e_stream
)
	: istream(i_stream), ostream(o_stream), estream(e_stream)
{
	estream << "Connecting.. ";
}

ToxReceiverStream::~ToxReceiverStream()
{
}

void ToxReceiverStream::onId
(
	// Tox *tox,
	ToxClient *toxclient,
	const std::string &valuehex
)
{
	estream << valuehex << std::endl;
}

void ToxReceiverStream::onConnectionStatus
(
	// Tox *tox,
	ToxClient *toxclient,
	TOX_CONNECTION value
)
{
	std::string s;
	switch(value) 
	{
		case TOX_CONNECTION_TCP:
			s = "TCP connection established";
			break;
		case TOX_CONNECTION_UDP:
			s = "UDP connection established";
			break;
		default:
			s = "offline";
	}
	estream << s << std::endl;
}

void ToxReceiverStream::onMessage
(
	// Tox *tox,
	ToxClient *toxclient,
	uint32_t friend_number, 
	const std::string &value,
	void *user_data
)
{
	ostream << toxclient->getFriendName(friend_number) << std::endl << value << std::endl;
}

bool ToxReceiverStream::onFriendRequest
(
	ToxClient *toxclient,
	const uint8_t *key, 
	const std::string &name,
	void *user_data
)
{
	estream << "Friend request " << name << std::endl;
	return true;
}

bool ToxReceiverStream::nextMessageTo
(
	uint32_t *friend_number,
	TOX_MESSAGE_TYPE *message_type,
	std::string *text
)
{
	bool r = messages.size();
	if (r)
	{
		ToxMessage m = messages.front();
		messages.pop();
		*friend_number = m.friend_number;
		*message_type = m.message_type;
		*text = m.message;
	}
	return r;
}

void ToxReceiverStream::readLine
(
	ToxClient *toxclient
)
{
	std::string line;
	std::getline(istream, line);
	ToxMessage m;
	m.friend_number = 0;
	m.message_type = TOX_MESSAGE_TYPE_NORMAL;
	m.message = line;
	messages.push(m);
}

	

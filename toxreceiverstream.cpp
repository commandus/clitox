#include "toxreceiverstream.h"

ToxReceiverStream::ToxReceiverStream
(
	std::istream &i_stream,
	std::ostream &o_stream,
	std::ostream &e_stream
)
	: istream(i_stream), ostream(o_stream), estream(e_stream)
{
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
			s = "TCP";
			break;
		case TOX_CONNECTION_UDP:
			s = "UDP";
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

void ToxReceiverStream::onFriendRequest
(
	// Tox *tox,
	ToxClient *toxclient,
	const uint8_t *key, 
	const std::string &name,
	void *user_data
)
{
	estream << "Friend request " << name << std::endl;
}


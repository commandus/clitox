#include "toxreceiverstream.h"

ToxReceiverStream::ToxReceiverStream
(
	std::istream &i_stream,
	std::ostream &o_stream,
	std::ostream &e_stream,
	TOX_MESSAGE_TYPE default_message_type
)
	: istream(i_stream), ostream(o_stream), estream(e_stream), defaultMessageType(default_message_type)
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

void ToxReceiverStream::execCommand
(
	std::ostream &strm,
	ToxClient *toxclient,
	const std::string &line
)
{
	if (!toxclient)
		return;
	if (line == "/status")
	{
		strm << "Name\tStatus";
		strm << std::endl;
		int sz = toxclient->getFriendSize();
		for (int i = 0; i < sz; i++)
		{
			std::string n = toxclient->getFriendName(i);
			if (n.empty())
				strm << i + 1;
			else
				strm << n;
			strm << "\t";
			TOX_USER_STATUS status = toxclient->getFriendStatus(i);
			std::string sstatus;
			switch(status)
			{
				case TOX_USER_STATUS_NONE:
					sstatus = "online";
					break;
				case TOX_USER_STATUS_BUSY:
					sstatus = "busy";
					break;
				case TOX_USER_STATUS_AWAY:
					sstatus = "away";
					break;
			}
			strm << (toxclient->getFriendIsConnected(i)? "connected" : "disconnected") << "\t"
				<< sstatus << "\t"
				<< toxclient->getFriendStatusString(i) << "\t";
			strm << std::endl;
		}
		strm << std::endl;
	}
}

bool ToxReceiverStream::isCommand
(
	const std::string &line
)
{
	bool r = false;
	if (line == "/status")
		r = true;
	return r;
}

void ToxReceiverStream::readLine
(
	ToxClient *toxclient
)
{
	std::string line;
	std::getline(istream, line);
	if (isCommand(line))
	{
		execCommand(std::cout, toxclient, line);
	}
	else
	{
		ToxMessage m;
		m.friend_number = 0;
		m.message_type = defaultMessageType;
		m.message = line;
		messages.push(m);
	}
}

	

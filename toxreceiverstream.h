#ifndef TOXRECEIVERSTREAM_H
#define TOXRECEIVERSTREAM_H

#include <inttypes.h>
#include <iostream>
#include <queue>
#include <tox/tox.h>
#include "toxreceiver.h"
#include "toxclient.h"
#include "toxmessage.h"

class ToxReceiverStream : public ToxReceiver
{
private:
	std::istream &istream;
	std::ostream &ostream;
	std::ostream &estream;
	std::queue<ToxMessage> messages;
public:
	ToxReceiverStream
	(
		std::istream &i_stream,
		std::ostream &o_stream,
		std::ostream &e_stream
	);
	~ToxReceiverStream();

	virtual void onId(
		// Tox *tox,
		ToxClient *toxclient,
		const std::string &valuehex
	);
	virtual void onConnectionStatus(
		// Tox *tox,
		ToxClient *toxclient,
		TOX_CONNECTION value
	);
	virtual void onMessage
	(
		// Tox *tox,
		ToxClient *toxclient,
		uint32_t friend_number, 
		const std::string &value,
		void *user_data
	);
	virtual bool onFriendRequest(
		// Tox *tox,
		ToxClient *toxclient,
		const uint8_t *key, 
		const std::string &name,
		void *user_data
	);
	virtual bool nextMessageTo
	(
		uint32_t *friend_number,
		TOX_MESSAGE_TYPE *message_type,
		std::string *m
	);
	void readLine
	(
		ToxClient *toxclient
	);
};

#endif // TOXRECEIVERSTREAM_H

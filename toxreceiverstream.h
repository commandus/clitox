#ifndef TOXRECEIVERSTREAM_H
#define TOXRECEIVERSTREAM_H

#include <iostream>
#include <tox/tox.h>
#include "toxreceiver.h"
#include "toxclient.h"

class ToxReceiverStream : public ToxReceiver
{
private:
	std::istream &istream;
	std::ostream &ostream;
	std::ostream &estream;
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
	) override;
	virtual void onConnectionStatus(
		// Tox *tox,
		ToxClient *toxclient,
		TOX_CONNECTION value
	) override;
	virtual void onMessage
	(
		// Tox *tox,
		ToxClient *toxclient,
		uint32_t friend_number, 
		const std::string &value,
		void *user_data
	) override;
	virtual void onFriendRequest(
		// Tox *tox,
		ToxClient *toxclient,
		const uint8_t *key, 
		const std::string &name,
		void *user_data
	) override;

};

#endif // TOXRECEIVERSTREAM_H

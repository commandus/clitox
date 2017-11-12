#ifndef TOXRECEIVER_H
#define TOXRECEIVER_H

#include <string>

class ToxClient;

class ToxReceiver
{
public:
	virtual void onId
	(
		// Tox *tox,
		ToxClient *toxclient,
		const std::string &valuehex
	) = 0;
	virtual void onConnectionStatus
	(
		// Tox *tox,
		ToxClient *toxclient,
		TOX_CONNECTION value
	) = 0;
	virtual void onMessage
	(
		// Tox *tox,
		ToxClient *toxclient,
		uint32_t friend_number, 
		const std::string &value,
		void *user_data
	) = 0;
	virtual bool onFriendRequest
	(
		// Tox *tox,
		ToxClient *toxclient,
		const uint8_t *key, 
		const std::string &name,
		void *user_data
	) = 0;
	
	virtual bool nextMessageTo
	(
		uint32_t  *friend_number,
		TOX_MESSAGE_TYPE *message_type,
		std::string *m
	) = 0;
};

#endif // TOXRECEIVER_H

#ifndef TOXRECEIVER_H
#define TOXRECEIVER_H

#include <string>

class ToxClient;

class ToxReceiver
{
public:
	virtual ~ToxReceiver
	(
		
	) {};
	virtual void onId
	(
		ToxClient *toxclient,
		const std::string &valuehex
	) = 0;
	virtual void onConnectionStatus
	(
		ToxClient *toxclient,
		TOX_CONNECTION value
	) = 0;
	virtual void onMessage
	(
		ToxClient *toxclient,
        TOX_MESSAGE_TYPE message_type,
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

	// do not reflect to Java. Called from putMessage reflected in ToxClient
	virtual void putMessage
	(
			ToxClient *toxclient,
			const TOX_MESSAGE_TYPE message_type,
			const uint32_t friend_number,
			const std::string &value
	) = 0;
};

#endif // TOXRECEIVER_H

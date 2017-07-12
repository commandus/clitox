/**
 * @file toxclient.h
 */

#ifndef TOXCLIENT_H
#define TOXCLIENT_H

#include <string>
#include <tox/tox.h>

#include "clientlist.h"
#include "toxreceiver.h"

class ToxClient
{
private:
	bool stopped;
	TOX_CONNECTION connectionStatus;
	std::string fileName;
	ToxReceiver *toxReceiver;
	Tox *tox;

	static std::string hex2bin(const std::string &value);
	void addToList();
	void rmFromList();

public:
	ToxClient
	(
		const std::string &filename,
		const std::string &nick_name,
		const std::string &status,
		ToxReceiver *toxreceiver
	);

	~ToxClient();
	
	Tox *getTox() const;
	static ToxClient *findByTox(Tox *);

	std::string getId();
	void setNick(const std::string &nick);
	void setStatus(const std::string &message);

	void connection_status
	(
		TOX_CONNECTION connection_status, 
		void *user_data
	);
	
	void friend_message
	(
		uint32_t friend_number, 
		TOX_MESSAGE_TYPE type, 
		const uint8_t *message,
		size_t length, 
		void *user_data
	);

	void friend_request
	(
		const uint8_t *public_key, 
		const uint8_t *message, 
		size_t length, 
		void *user_data
	);

	void sendFriendText
	(
		uint32_t friend_number, 
		const std::string &message
	);
	void sendFriendAction
	(
		uint32_t friend_number, 
		const std::string &action
	);
	TOX_CONNECTION getConnectionStatus();

	std::string getFriendName
	(
		uint32_t friend_number
	);
	
	int run();
	
	void stop();
};

#endif // TOXCLIENT_H

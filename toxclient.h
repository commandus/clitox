/**
 * @file toxclient.h
 */

#ifndef TOXCLIENT_H
#define TOXCLIENT_H

#include <string>
#include <tox/tox.h>

#include "clientlist.h"

class ToxClient
{
private:
	bool stop;
	uint8_t toxId[TOX_ADDRESS_SIZE];
	
	Tox *tox;
	int run();
	static std::string hex2bin(const std::string &value);
	void addToList();
	void rmFromList();
	
public:
	ToxClient
	(
		const std::string &nick_name,
		const std::string &status
	);
	~ToxClient();
	Tox *getTox();
	std::string getId();
	void setNick(const std::string &nick);
	void setStatus(const std::string &message);
	static ToxClient *findByTox(Tox *);
	void setConnectionStatus
	(
		TOX_CONNECTION connection_status, 
		void *user_data
	);
	
	void setMessage
	(
		uint32_t friend_number, 
		TOX_MESSAGE_TYPE type, 
		const uint8_t *message,
		size_t length, 
		void *user_data
	);

	void setFriendRequest
	(
		const uint8_t *public_key, 
		const uint8_t *message, 
		size_t length, 
		void *user_data
	);

};

#endif // TOXCLIENT_H

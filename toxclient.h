/**
 * @file toxclient.h
 */

#ifndef TOXCLIENT_H
#define TOXCLIENT_H

#include <string>
#include <vector>
#include <inttypes.h>

#include <tox/tox.h>

#include "clientlist.h"
#include "toxreceiver.h"

#ifdef __ANDROID__
#include "toxreceiverjava.h"
#endif

typedef struct DHT_node 
{
	std::string ip;
	uint16_t port;
	std::string key_hex;	//< [TOX_PUBLIC_KEY_SIZE * 2 + 1]
} DHT_node;

void write_tox
(
	const Tox *tox,
	const std::string &fn
);

void getDefaultNodes
(
	std::vector<struct DHT_node> &nodes
);

class ToxClient
{
private:
	bool stopped;
	TOX_CONNECTION connectionStatus;
	std::string fileName;
	ToxReceiver *toxReceiver;
    bool own_receiver;
	Tox *tox;
	std::vector <uint32_t> friends;

	void addToList();
	void rmFromList();

public:
	ToxClient();

	ToxClient
	(
		struct Tox_Options *toxoptions,
		const std::vector<struct DHT_node> &nodes,
		const std::string &filename,
		const std::string &nick_name,
		const std::string &status
	);

    ~ToxClient();

    void setReceiver
    (
        ToxReceiver *toxreceiver
    );

#ifdef __ANDROID__
    void setReceiverJava
    (
        JNIEnv *env,
        jobject obj
    );
#endif

    Tox *getTox() const;
	static ToxClient *findByTox(Tox *tox);
	std::string getIdHex();
	static std::string getIdHex(Tox *tox);
	static std::string getIdHex(const std::string &fn);

	std::string newId();
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

	void sendFriend
	(
		uint32_t friend_number, 
		TOX_MESSAGE_TYPE message_type,
		const std::string &message
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

	void clearFriends();
	
	uint32_t addFriend
	(
		const std::string &tox_id_hex
	);

	int run();
	
	void stop();
};

#endif // TOXCLIENT_H

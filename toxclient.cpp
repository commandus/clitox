/**
 * @file toxclient.cpp
 */
#include "toxclient.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <sodium/utils.h>
#include <tox/tox.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif

#ifdef _WIN32

#include <Windows.h>

void usleep(__int64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}
#else
#include <unistd.h>
#endif

static const DHT_node default_nodes[] =
	{
	{"178.62.250.138",             33445, "788236D34978D1D5BD822F0A5BEBD2C53C64CC31CD3149350EE27D4D9A2F9B6B"},
	{"2a03:b0c0:2:d0::16:1",       33445, "788236D34978D1D5BD822F0A5BEBD2C53C64CC31CD3149350EE27D4D9A2F9B6B"},
	{"tox.zodiaclabs.org",         33445, "A09162D68618E742FFBCA1C2C70385E6679604B2D80EA6E84AD0996A1AC8A074"},
	{"163.172.136.118",            33445, "2C289F9F37C20D09DA83565588BF496FAB3764853FA38141817A72E3F18ACA0B"},
	{"2001:bc8:4400:2100::1c:50f", 33445, "2C289F9F37C20D09DA83565588BF496FAB3764853FA38141817A72E3F18ACA0B"},
	{"128.199.199.197",            33445, "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09"},
	{"2400:6180:0:d0::17a:a001",   33445, "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09"},
	{"biribiri.org",               33445, "F404ABAA1C99A9D37D61AB54898F56793E1DEF8BD46B1038B9D822E8460FAB67"}
};

void getDefaultNodes
(
	std::vector<struct DHT_node> &nodes
)
{
	for (size_t i = 0; i < sizeof(default_nodes)/sizeof(DHT_node); i ++) 
	{
		nodes.push_back(default_nodes[i]);
	}
}

static std::string hex_to_bin
(
	const std::string &value
)
{
	size_t sz = value.size() / 2;
	std::string r;
	r.resize(sz);
	sodium_hex2bin((unsigned char *) r.c_str(), sz, value.c_str(), sz * 2, NULL, NULL, NULL);
	return r;
}

static std::string toHex
(
	const uint8_t *toxId,
	size_t size
)
{
	char tox_id_hex[TOX_ADDRESS_SIZE * 2 + 1];
	sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), toxId, size);
	for (size_t i = 0; i < size * 2; i++)
	{
		tox_id_hex[i] = toupper(tox_id_hex[i]);
	}
	return std::string(tox_id_hex, size * 2);
}

static std::string str_addr_hex
(
	const uint8_t *toxId
)
{
	return toHex(toxId, TOX_ADDRESS_SIZE);
}

static std::string str_key_hex
(
		const uint8_t *toxKey
)
{
	return toHex(toxKey, TOX_PUBLIC_KEY_SIZE);
}

void bootstrap
(
	Tox *tox,
	const std::vector<struct DHT_node> &nodes
)
{
	for (std::vector<struct DHT_node>::const_iterator it(nodes.begin()); it != nodes.end(); ++it) 
	{
		tox_bootstrap(tox, it->ip.c_str(), it->port, (unsigned char *) hex_to_bin(it->key_hex.c_str()).c_str(), NULL);
	}
}

void copyOptions
(
	struct Tox_Options *options,
	struct Tox_Options *src
)
{
	if (options)
	{
		tox_options_default(options);
		if (src)
		{
			// override from new settings
			options->ipv6_enabled = src->ipv6_enabled;
			options->udp_enabled = src->udp_enabled;
			options->local_discovery_enabled = src->local_discovery_enabled;
			options->proxy_type = src->proxy_type;
			options->proxy_host = src->proxy_host;
			options->proxy_port = src->proxy_port;
			options->start_port = src->start_port;
			options->end_port = src->end_port;
			options->tcp_port = src->tcp_port;
			options->hole_punching_enabled = src->hole_punching_enabled;
		}
	}
}

TOX_ERR_NEW readTox
(
	Tox **retval,
	struct Tox_Options *toxoptions,
	const std::string &fn
)
{
	enum TOX_ERR_NEW r = TOX_ERR_NEW_LOAD_BAD_FORMAT;
	struct Tox_Options options;
	copyOptions(&options, toxoptions);
	
	FILE *f = fopen(fn.c_str(), "rb");
	size_t read = 0;
	uint8_t *savedata = NULL;
	if (f) 
	{
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		savedata = (uint8_t*) malloc(fsize);

		read = fread(savedata, fsize, 1, f);
		fclose(f);

		options.savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
		options.savedata_data = savedata;
		options.savedata_length = fsize;

	} 
	if (read == 1)
		*retval = tox_new(&options, &r);
	else
		*retval = NULL;
	if (savedata)
		free(savedata);
	return r;
}

void writeTox
(
	const Tox *tox,
	const std::string &fn
)
{
    if (!tox)
        return;
	size_t size = tox_get_savedata_size(tox);
	uint8_t *savedata = (uint8_t *) malloc(size);
	tox_get_savedata(tox, savedata);

	FILE *f = fopen(fn.c_str(), "wb");
	if (f)
	{
		fwrite(savedata, size, 1, f);
		fclose(f);
	}
	free(savedata);
}

ToxClient *ToxClient::findByTox(Tox *tox)
{
	return (ToxClient *) ClientList::getInstance().get(tox);
}

std::string ToxClient::getIdHex()
{
	return ToxClient::getIdHex(tox);
}

std::string ToxClient::getIdHex(Tox *tox)
{
	uint8_t toxId[TOX_ADDRESS_SIZE];
	tox_self_get_address(tox, toxId);
	return str_addr_hex(toxId);
}

std::string ToxClient::getIdHex
(
	const std::string &fn
)
{
	Tox *tox;
	if (readTox(&tox, NULL, fn) != TOX_ERR_NEW_OK)
		return "";
	std::string r = getIdHex(tox);
	tox_kill(tox);
	return r;
}

void ToxClient::addToList()
{
	ClientList::getInstance().put(tox, this);
}

void ToxClient::rmFromList()
{
	ClientList::getInstance().remove(tox);
}

void friend_request_cb(
	Tox *tox, 
	const uint8_t *public_key, 
	const uint8_t *message, 
	size_t length, 
	void *user_data
)
{
	ToxClient *cli =  ToxClient::findByTox(tox);
	if (cli)
		cli->friendRequest(public_key, message, length, user_data);
}

void friend_message_cb
(
	Tox *tox, 
	uint32_t friend_number, 
	TOX_MESSAGE_TYPE type, 
	const uint8_t *message,
	size_t length, 
	void *user_data
)
{
	ToxClient *cli =  ToxClient::findByTox(tox);
	if (cli)
		cli->friendMessage(friend_number, type, message, length, user_data);
}

void self_connection_status_cb
(
	Tox *tox, 
	TOX_CONNECTION connection_status, 
	void *user_data
)
{
	ToxClient *cli =  ToxClient::findByTox(tox);
	if (cli)
		cli->setConnectionStatus(connection_status, user_data);
}

ToxClient::ToxClient()
	: stopped(false), connectionStatus(TOX_CONNECTION_NONE), fileName(""), toxReceiver(NULL), ownReceiver(false),
	tox(NULL), friendNoRequest(false), inviteMessage("Add me to friend")
{
}

ToxClient::ToxClient
(
	struct Tox_Options *toxoptions,
	const std::vector<struct DHT_node> &nodes,
	const std::string &filename,
	const std::string &nick,
	const std::string &status,
	bool friend_norequest,
	const std::string &invite_message
)
	: stopped(false), connectionStatus(TOX_CONNECTION_NONE), fileName(filename), toxReceiver(NULL), ownReceiver(false), 
		friendNoRequest(friend_norequest), inviteMessage(invite_message)
{
	// Tox_Options *options = tox_options_new(NULL);
	if (readTox(&tox, toxoptions, filename) != TOX_ERR_NEW_OK)
	{
		struct Tox_Options options;
		copyOptions(&options, toxoptions);
		TOX_ERR_NEW errnew;
		tox = tox_new(&options, &errnew);
		if (errnew == TOX_ERR_NEW_OK)
		{
			// save to file
			writeTox(tox, filename);
		}
		else
		{
			// std::cerr << "Error: " << errnew << std::endl;
			return;
		}
	
	}
	setNick(nick);
	setStatus(status);

	bootstrap(tox, nodes);
	writeTox(tox, filename);

	tox_callback_friend_request(tox, friend_request_cb);
	tox_callback_friend_message(tox, friend_message_cb);
	tox_callback_self_connection_status(tox, self_connection_status_cb);
	
	addToList();
}

void ToxClient::setReceiver
(
    ToxReceiver *value
)
{
    toxReceiver = value;
    ownReceiver = false;
}

#ifdef __ANDROID__
void ToxClient::setReceiverJava
(
        JNIEnv *env,
        jobject obj
)
{
    __android_log_write(ANDROID_LOG_INFO, "ToxClient set receiver", " native");
    toxReceiver = new ToxReceiverJava(env, obj);
    own_receiver = true;
}
#endif

int ToxClient::run()
{
	stopped = false;
	tox_self_set_status(tox, TOX_USER_STATUS_NONE);
	tox_self_set_status_message(tox, (const uint8_t *) "oooo", 4, NULL);
	
	while (!stopped) 
	{
		if (toxReceiver)
		{
			
			TOX_MESSAGE_TYPE message_type;
			std::string m;
			uint32_t friend_number;
			while (toxReceiver->nextMessageTo(&friend_number, &message_type, &m))
			{
				if (friend_number != 0)
				{
					sendFriend(friend_number, message_type, m);
				}
				else
				{
					for (std::vector<uint32_t>::const_iterator it(friends.begin()); it != friends.end(); ++it)
					{
						sendFriend(*it, message_type, m);
					}
				}
			}
		}
		tox_iterate(tox, NULL);
		usleep(tox_iteration_interval(tox) * 1000);
	}
	tox_self_set_status(tox, TOX_USER_STATUS_AWAY);
	return 0;
}

void ToxClient::stop()
{
	stopped = true;
}

ToxClient::~ToxClient()
{
	writeTox(tox, fileName);
	rmFromList();
	tox_kill(tox);
    if (toxReceiver) {
        if (ownReceiver) {
            delete toxReceiver;
        }
    }
}

Tox *ToxClient::getTox() const
{
	return tox;
}

void ToxClient::setNick(const std::string &nick)
{
	tox_self_set_name(tox, (const uint8_t*) nick.c_str(), nick.length(), NULL);
}

void ToxClient::setStatus(const std::string &message)
{
	tox_self_set_status_message(tox, (const uint8_t *) message.c_str(), message.length(), NULL);
}

void ToxClient::setConnectionStatus
(
	TOX_CONNECTION connection_status, 
	void *user_data
)
{
	this->connectionStatus = connection_status;
	if (toxReceiver)
		toxReceiver->onConnectionStatus(this, connection_status);
}

TOX_CONNECTION ToxClient::getConnectionStatus()
{
	return this->connectionStatus;
}

void ToxClient::friendMessage
(
	uint32_t friend_number, 
	TOX_MESSAGE_TYPE type, 
	const uint8_t *message,
	size_t length, 
	void *user_data
)
{
	if (toxReceiver)
		toxReceiver->onMessage(this, friend_number, std::string((char *) message, length), user_data);
}

void ToxClient::friendRequest
(
	const uint8_t *public_key, 
	const uint8_t *message, 
	size_t length, 
	void *user_data
)
{
	if (toxReceiver)
		toxReceiver->onFriendRequest(this, public_key, std::string((char *) message, length), user_data);
	tox_friend_add_norequest(tox, public_key, NULL);
	writeTox(tox, fileName);
}

void ToxClient::sendFriend
(
	uint32_t friend_number, 
	TOX_MESSAGE_TYPE message_type,
	const std::string &message
)
{
	tox_friend_send_message(tox, friend_number, message_type, (const uint8_t*) message.c_str(), message.length(), NULL);
}

void ToxClient::sendFriendText
(
	uint32_t friend_number, 
	const std::string &message
)
{
	sendFriend(friend_number, TOX_MESSAGE_TYPE_NORMAL, message);
}

void ToxClient::sendFriendAction
(
	uint32_t friend_number, 
	const std::string &action
)
{
	sendFriend(friend_number, TOX_MESSAGE_TYPE_ACTION, action);
}

std::string ToxClient::getFriendId
(
        uint32_t friend_number
)
{
    uint8_t cid[TOX_PUBLIC_KEY_SIZE * 2];
    int size = tox_friend_get_public_key(tox, friend_number, cid, NULL);
    return str_key_hex(cid);
}

std::string ToxClient::getFriendName
(
	uint32_t friend_number
)
{
    int size = tox_friend_get_name_size(tox, friend_number, 0);
	if (size > TOX_MAX_NAME_LENGTH)
		size = TOX_MAX_NAME_LENGTH;
	
	uint8_t name[TOX_MAX_NAME_LENGTH];
    tox_friend_get_name(tox, friend_number, name, 0);
	return std::string((char *) name, size);
}

void ToxClient::clearFriends
(
)
{
	friends.clear();
}

uint32_t ToxClient::addFriend
(
	const std::string &tox_id_hex
)
{
	std::string toxid = hex_to_bin(tox_id_hex);
	TOX_ERR_FRIEND_ADD e;
	uint32_t r = tox_friend_by_public_key(tox, (const uint8_t *) toxid.c_str(), NULL);
	if (r == (uint32_t) -1)     // 4294967295
	{
		if (friendNoRequest)
			r = tox_friend_add_norequest(tox, (const uint8_t *)toxid.c_str(), &e);
		else
			r = tox_friend_add(tox, (const uint8_t *)toxid.c_str(), (const uint8_t *)inviteMessage.c_str(), inviteMessage.size(), &e);
		if (r == (uint32_t) -1) // 4294967295
			std::cerr << "Error " << e << std::endl;
	}
	if (r != (uint32_t) -1)     // 4294967295
		friends.push_back(r);
	return r;
}

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

// TODO load bootstrap and parse json
/*
 * https://nodes.tox.chat/json
 * {
	"last_scan": 1499754490,
	"last_refresh": 1499754502,
	"nodes": [{
		"ipv4": "biribiri.org",
		"ipv6": "-",
		"port": 33445,
		"tcp_ports": [3389, 33445],
		"public_key": "F404ABAA1C99A9D37D61AB54898F56793E1DEF8BD46B1038B9D822E8460FAB67",
		"maintainer": "nurupo",
		"location": "US",
		"status_udp": true,
		"status_tcp": true,
		"version": "2016010100",
		"motd": "Welcome, stranger #5866. I'm up for 4d 02h 19m 35s, running since Jul 07 04:08:35 UTC. If I get outdated, please ping my maintainer at nurupo.contributions@gmail.com",
		"last_ping": 1499754490
	},...
	*/

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

static std::string str_addr_hex
(
	const uint8_t *toxId
)
{
	char tox_id_hex[TOX_ADDRESS_SIZE * 2 + 1];
	sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), toxId, TOX_ADDRESS_SIZE);
	for (size_t i = 0; i < TOX_ADDRESS_SIZE * 2; i ++) 
	{
		tox_id_hex[i] = toupper(tox_id_hex[i]);
	}
	return std::string(tox_id_hex, TOX_ADDRESS_SIZE * 2);
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
 
bool read_tox
(
	Tox **retval,
	struct Tox_Options *toxoptions,
	const std::string &fn
)
{
	bool r = false;
	struct Tox_Options options;
	tox_options_default(&options);
	if (toxoptions)
	{
		// override from new settings
	    options.ipv6_enabled = toxoptions->ipv6_enabled;
		options.udp_enabled = toxoptions->udp_enabled;
		options.local_discovery_enabled = toxoptions->local_discovery_enabled;
		options.proxy_type = toxoptions->proxy_type;
		options.proxy_host = toxoptions->proxy_host;
		options.proxy_port = toxoptions->proxy_port;
		options.start_port = toxoptions->start_port;
		options.end_port = toxoptions->end_port;
		options.tcp_port = toxoptions->tcp_port;
		options.hole_punching_enabled = toxoptions->hole_punching_enabled;

	}
	
	FILE *f = fopen(fn.c_str(), "rb");
	if (f) 
	{
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char *savedata = (char *) malloc(fsize);

		size_t read = fread(savedata, fsize, 1, f);
		fclose(f);

		options.savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
		options.savedata_data = (uint8_t *) savedata;
		options.savedata_length = fsize;

		free(savedata);
		r = read == 1;
	} 

	*retval = tox_new(&options, NULL);
	return r;
}

void write_tox
(
	const Tox *tox,
	const std::string &fn
)
{
    if (!tox)
        return;
	size_t size = tox_get_savedata_size(tox);
	char *savedata = (char *) malloc(size);
	tox_get_savedata(tox, (uint8_t *) savedata);

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
	if (!read_tox(&tox, NULL, fn))
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
		cli->friend_request(public_key, message, length, user_data);
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
		cli->friend_message(friend_number, type, message, length, user_data);
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
		cli->connection_status(connection_status, user_data);
}

ToxClient::ToxClient()
	: stopped(false), connectionStatus(TOX_CONNECTION_NONE), fileName(""), toxReceiver(NULL), own_receiver(false)
{
	tox = tox_new(NULL, NULL);
}

ToxClient::ToxClient
(
	struct Tox_Options *toxoptions,
	const std::vector<struct DHT_node> &nodes,
	const std::string &filename,
	const std::string &nick,
	const std::string &status
)
	: stopped(false), connectionStatus(TOX_CONNECTION_NONE), fileName(filename), toxReceiver(NULL), own_receiver(false)
{
	// Tox_Options *options = tox_options_new(NULL);
	if (!read_tox(&tox, toxoptions, filename))
	{
		// create a new identifier and save to file
		newId();
		write_tox(tox, filename);
	}
	setNick(nick);
	setStatus(status);

	bootstrap(tox, nodes);

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
    own_receiver = false;
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
	write_tox(tox, fileName);
	rmFromList();
	tox_kill(tox);
    if (toxReceiver) {
        if (own_receiver) {
            delete toxReceiver;
        }
    }
}

Tox *ToxClient::getTox() const
{
	return tox;
}

std::string ToxClient::newId()
{
	uint8_t toxId[TOX_ADDRESS_SIZE];
	tox_self_get_address(tox, toxId);
	std::string r = str_addr_hex(toxId);

	if (toxReceiver)
		toxReceiver->onId(this, r);
	return r;
}

void ToxClient::setNick(const std::string &nick)
{
	tox_self_set_name(tox, (const uint8_t*) nick.c_str(), nick.length(), NULL);
}

void ToxClient::setStatus(const std::string &message)
{
	tox_self_set_status_message(tox, (const uint8_t *) message.c_str(), message.length(), NULL);
}

void ToxClient::connection_status
(
	TOX_CONNECTION connection_status, 
	void *user_data
)
{
	connectionStatus = connection_status;
	if (toxReceiver)
		toxReceiver->onConnectionStatus(this, connection_status);
}

TOX_CONNECTION ToxClient::getConnectionStatus()
{
	return connectionStatus;
}

void ToxClient::friend_message
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

void ToxClient::friend_request
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
	write_tox(tox, fileName);
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
	// tox_friend_add(tox, (const uint8_t *) tox_id_hex.c_str(), (const uint8_t *) tox_id_hex.c_str(), tox_id_hex.length(), NULL);
	TOX_ERR_FRIEND_ADD e;
	uint32_t r = tox_friend_by_public_key(tox, (const uint8_t *) toxid.c_str(), NULL);
	if (r == (uint32_t) -1)     // 4294967295
	{
		r = tox_friend_add_norequest(tox, (const uint8_t *) toxid.c_str(), &e);
		if (r == (uint32_t) -1) // 4294967295
			std::cerr << "Error " << e << std::endl;
	}
	if (r != (uint32_t) -1)     // 4294967295
		friends.push_back(r);
	return r;
}

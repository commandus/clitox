/**
 * @file toxclient.cpp
 */
#include "toxclient.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sodium/utils.h>
#include <tox/tox.h>




typedef struct DHT_node {
	const char *ip;
	uint16_t port;
	const char key_hex[TOX_PUBLIC_KEY_SIZE*2 + 1];
} DHT_node;

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

static const DHT_node nodes[] =
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

std::string ToxClient::hex2bin(const std::string &value)
{
	size_t sz = value.size() / 2;
	std::string r;
	r.resize(sz);
	sodium_hex2bin((unsigned char *) r.c_str(), sz, value.c_str(), sz * 2, NULL, NULL, NULL);
	return r;
}

ToxClient *ToxClient::findByTox(Tox *tox)
{
	return (ToxClient *) ClientList::getInstance().get(tox);
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
		cli->setFriendRequest(public_key, message, length, user_data);
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
		cli->setMessage(friend_number, type, message, length, user_data);
}

void self_connection_status_cb(
	Tox *tox, 
	TOX_CONNECTION connection_status, 
	void *user_data
)
{
	ToxClient *cli =  ToxClient::findByTox(tox);
	if (cli)
		cli->setConnectionStatus(connection_status, user_data);
}

ToxClient::ToxClient
(
	const std::string &nick,
	const std::string &status 
)
	: stop(false)
{
	
	// Tox_Options *options = tox_options_new(NULL);
	tox = tox_new(NULL, NULL);
	setNick(nick);
	setStatus(status);

	for (size_t i = 0; i < sizeof(nodes)/sizeof(DHT_node); i ++) 
	{
		tox_bootstrap(tox, nodes[i].ip, nodes[i].port, (unsigned char *) hex2bin(nodes[i].key_hex).c_str(), NULL);
	}

	tox_self_get_address(tox, toxId);

	tox_callback_friend_request(tox, friend_request_cb);
	tox_callback_friend_message(tox, friend_message_cb);
	tox_callback_self_connection_status(tox, self_connection_status_cb);
	
	addToList();
}

int ToxClient::run()
{
	while (!stop) {
		tox_iterate(tox, NULL);
		usleep(tox_iteration_interval(tox) * 1000);
	}
	tox_kill(tox);
}

ToxClient::~ToxClient()
{
	rmFromList();
	tox_kill(tox);
}

Tox *ToxClient::getTox()
{
	return tox;
}

std::string ToxClient::getId()
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0') << std::uppercase;
	for (size_t i = 0; i < sizeof(toxId); i++) 
		ss << std::setw(2) << static_cast<unsigned>(toxId[i]);
	return ss.str();
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
	switch (connection_status) 
	{
		case TOX_CONNECTION_NONE:
			printf("Offline\n");
			break;
		case TOX_CONNECTION_TCP:
			printf("Online, using TCP\n");
			break;
		case TOX_CONNECTION_UDP:
			printf("Online, using UDP\n");
			break;
	}
}

void ToxClient::setMessage
(
	uint32_t friend_number, 
	TOX_MESSAGE_TYPE type, 
	const uint8_t *message,
	size_t length, 
	void *user_data
)
{
	// echo
	tox_friend_send_message(tox, friend_number, type, message, length, NULL);
}

void ToxClient::setFriendRequest
(
	const uint8_t *public_key, 
	const uint8_t *message, 
	size_t length, 
	void *user_data
)
{
	tox_friend_add_norequest(tox, public_key, NULL);
}

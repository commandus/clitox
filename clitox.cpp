/**
 * @file clitox.cpp
 */

#include <thread>
#include <string>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <argtable2.h>

#include "clitox.h"
#include "toxclient.h"
#include "toxreceiverstream.h"
#include "toxnodejson.h"

ToxClient *toxclient  = NULL;
bool stopRequest = false;

#ifdef _WIN32

void setSignalHandler(int signal)
{
}

#else
void signalHandler(int signal)
{
	switch(signal)
	{
	case SIGINT:
		stopRequest = true;
		if (toxclient)
			toxclient->stop();
		std::cerr << MSG_INTERRUPTED << std::endl;
		break;
	default:
		break;
	}
}

void setSignalHandler(int signal)
{
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = &signalHandler;
	sigaction(signal, &action, NULL);
}
#endif

void read_loop
(
	ToxClient *toxclient,
	ToxReceiverStream *toxreceiverstream,
	bool stopRequest
)
{
	while(!stopRequest)
	{
		toxreceiverstream->readLine(toxclient);
	}
}

int main(int argc, char** argv)
{
	// Signal handler
	setSignalHandler(SIGINT);

	ClitoxConfig config(argc, argv);
	if (config.error())
		exit(config.error());

	// load from command line options
	struct Tox_Options toxoptions;
	config.getToxOptions(&toxoptions);
	
	// get bootstrap
	std::vector<struct DHT_node> nodes;
	load_json_nodes(nodes, config.nodes_json);
	if (nodes.empty())
		getDefaultNodes(nodes);

	toxclient = new ToxClient(&toxoptions, nodes, config.file_name, config.nick_name, config.status_message, 
		config.friend_norequest, config.message_friend_request);

	std::string r = toxclient->getIdHex();

	switch (config.cmd) 
	{
		case CMD_PRINT_TOX_ID:
			{
				std::cout << r << std::endl;
			}
			break;
		default:
		{
			std::cerr << "ToxID: " << r << std::endl;
			
			ToxReceiverStream toxreceiverstream(std::cin, std::cout, std::cerr, config.message_type_action?TOX_MESSAGE_TYPE_ACTION: TOX_MESSAGE_TYPE_NORMAL);
			toxclient->setReceiver(&toxreceiverstream);
			
			toxclient->clearFriends();
			for (std::vector<std::string>::const_iterator it(config.ids_to.begin()); it != config.ids_to.end(); ++it)
			{
				toxclient->addFriend(*it);
			}
			std::thread t(read_loop, toxclient,  &toxreceiverstream, stopRequest);
			t.detach();
			toxclient->run();
		}
	}

	delete toxclient;
	return 0;
}

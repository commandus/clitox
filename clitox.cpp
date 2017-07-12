/**
 * @file clitox.cpp
 */
#include <argtable2.h>
#include <string>
#include <cstring>
#include <iostream>
#include <signal.h>

#include "clitox.h"

#include "toxclient.h"
#include "toxreceiverstream.h"

ToxClient *toxclient  = NULL;

void signalHandler(int signal)
{
	switch(signal)
	{
	case SIGINT:
		if (toxclient)
			toxclient->stop();
		std::cerr << MSG_INTERRUPTED << std::endl;
		break;
	case SIGHUP:
		std::cerr << MSG_RELOAD_CONFIG_REQUEST << " nothing to do" << std::endl;
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

int main(int argc, char** argv)
{
    // Signal handler
    setSignalHandler(SIGINT);
    setSignalHandler(SIGHUP);

	ClitoxConfig config(argc, argv);
	if (config.error())
		exit(config.error());
	
	switch (config.cmd) 
	{
		case CMD_GET_ID:
			// std::cout << ToxClient::getId() << std::endl;
			break;
		default:
		{
			ToxReceiverStream toxreceiverstream(std::cin, std::cout, std::cerr);
			toxclient = new ToxClient(config.file_name, config.nick_name, config.status_message, &toxreceiverstream);
			toxclient->run();
			delete toxclient;
		}
	}

	return 0;
}

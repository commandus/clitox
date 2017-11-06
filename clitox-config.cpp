#include "clitox-config.h"
#include <iostream>
#include <argtable2.h>

static const char* progname = "clitox";
#define DEF_TOX_FILE_NAME			"clitox.tox"
#define DEF_MESSAGE_FRIEND_REQUEST	"Add me"

ClitoxConfig::ClitoxConfig()
	: cmd(CMD_RW), file_name(""), nick_name(""), status_message(""), friend_norequest(false)
{
}

ClitoxConfig::ClitoxConfig
(
	int argc,
	char* argv[]
)
{
	defaultToxOptions(&toxoptions);
	errorcode = parseCmd(argc, argv);
}

/**
 * Parse command line into ClitoxConfig class
 * Return 0- success
 *        1- show help and exit, or command syntax error
 *        2- output file does not exists or can not open to write
 **/
int ClitoxConfig::parseCmd
(
	int argc,
	char* argv[]
)
{
	// GTFS https://developers.google.com/transit/gtfs/reference/?csw=1
	struct arg_lit *a_print_tox_id = arg_lit0("i", "id", "Print Tox ID");
	struct arg_str *a_file_name = arg_str0("f", "file", "<file>", "Tox configuration file. Default " DEF_TOX_FILE_NAME);
	struct arg_str *a_nick_name = arg_str0("n", "name", "<nick name>", "nick name");
	struct arg_str *a_status_message = arg_str0("s", "status", "<text>", "initial status message");
	struct arg_str *a_ids_to = arg_strn(NULL, NULL, "<Tox ID>", 0, 100, "Send to clients by TOX identifier");
	struct arg_str *a_nodes_json = arg_strn("b", "boot", "<file name>", 0, 100, "Bootstrap nodes file");
	struct arg_lit *a_friend_norequest = arg_lit0("Q", "norequest", "Do not request to add me to friend list");
	struct arg_lit *a_message_type_action = arg_lit0("T", "type", "message type is action");
	
	struct arg_str *a_message_friend_request = arg_str0("a", "add_message", "<text>", "friend request message. Default " DEF_MESSAGE_FRIEND_REQUEST);
	
	// Tox options
	struct arg_lit *a_ipv6_disabled = arg_lit0(NULL, "no-ipv6", "Disable IPv6");
    struct arg_lit *a_udp_disabled = arg_lit0(NULL, "no-udp", "Disable UDP");
    struct arg_lit *a_local_discovery_disabled = arg_lit0(NULL, "no-local", "Disable local discovery.");
	struct arg_str *a_proxy_type = arg_str0(NULL, "proxy", "none|http|socks5", "Set proxy type. Default none.");
	struct arg_str *a_proxy_host = arg_str0(NULL, "proxy_host", "IP address or name", "Proxy host address or name.");
	struct arg_int *a_proxy_port = arg_int0(NULL, "proxy_port", "Port number", "Proxy host port.");
	struct arg_int *a_start_port = arg_int0(NULL, "start_port", "Port number", "Default 33445.");
	struct arg_int *a_end_port = arg_int0(NULL, "end_port", "Port number", "Default 33545.");
	struct arg_int *a_tcp_port = arg_int0(NULL, "tcp_relay_port", "Port number", "TCP relay server port. Default 0 (disable).");
	struct arg_lit *hole_punching_disabled = arg_lit0(NULL, "hole_punching_off", "Disable UDP hole-punching in toxcore.");

	struct arg_lit *a_help = arg_lit0("h", "help", "Show this help");
	struct arg_end *a_end = arg_end(20);

	void* argtable[] = { 
		a_print_tox_id, a_file_name, a_nick_name, a_ids_to, a_status_message,
		a_nodes_json, a_friend_norequest, a_message_type_action, a_message_friend_request,
		
		a_ipv6_disabled, a_udp_disabled, a_local_discovery_disabled, a_proxy_type,
		a_proxy_host, a_proxy_port, a_start_port, a_end_port, a_tcp_port,
		hole_punching_disabled,
		
		a_help, a_end 
	};

	int nerrors;

	// verify the argtable[] entries were allocated successfully
	if (arg_nullcheck(argtable) != 0)
	{
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}
	// Parse the command line as defined by argtable[]
	nerrors = arg_parse(argc, argv, argtable);

	if (a_print_tox_id->count)
		cmd = CMD_PRINT_TOX_ID;
	else
		cmd = CMD_RW;

	if (cmd == CMD_RW)
	{
		if (a_ids_to->count == 0)
		{
			std::cerr << "Tox ID missed." << std::endl;
			nerrors++;
		}
	}

	if (a_file_name->count)
		file_name = *a_file_name->sval;
	else
		file_name = DEF_TOX_FILE_NAME;

	if (a_nick_name->count)
		nick_name = *a_nick_name->sval;

	if (a_status_message->count)
		status_message = *a_status_message->sval;

	for (int i = 0; i < a_ids_to->count; i++)
	{
		ids_to.push_back(a_ids_to->sval[i]);
	}

	toxoptions.ipv6_enabled = a_ipv6_disabled->count == 0;
	toxoptions.udp_enabled = a_udp_disabled->count == 0;
	toxoptions.local_discovery_enabled = a_local_discovery_disabled->count == 0;
	if (a_proxy_type->count)
	{
		std::string pt(*a_proxy_type->sval);
		if (pt == "http")
			toxoptions.proxy_type = TOX_PROXY_TYPE_HTTP;
		else
			if (pt == "socks5")
				toxoptions.proxy_type = TOX_PROXY_TYPE_SOCKS5;
			else
				toxoptions.proxy_type = TOX_PROXY_TYPE_NONE;
	}

	if (a_proxy_host->count)
	{
		proxy_host = *a_proxy_host->sval;
		toxoptions.proxy_host = proxy_host.c_str();
	}
	else
	{
		proxy_host = "";
		toxoptions.proxy_host = NULL;
	}
	
	if (a_proxy_port->count)
		toxoptions.proxy_port = *a_proxy_port->ival;
	if (a_start_port->count)
		toxoptions.start_port = *a_start_port->ival;
	if (a_end_port->count)
		toxoptions.end_port = *a_end_port->ival;
	if (a_tcp_port->count)
		toxoptions.tcp_port = *a_tcp_port->ival;
	if (hole_punching_disabled->count)
		toxoptions.hole_punching_enabled = false;

	for (int i = 0; i < a_nodes_json->count; i++)
	{
		nodes_json.push_back(a_nodes_json->sval[i]);
	}
	
	friend_norequest = (a_friend_norequest->count > 0);
	message_type_action = (a_message_type_action > 0);

	if (a_message_friend_request->count) {
		message_friend_request = std::string(*a_message_friend_request->sval);
	}
	else
		message_friend_request = DEF_MESSAGE_FRIEND_REQUEST;

	// special case: '--help' takes precedence over error reporting
	if ((a_help->count) || nerrors)
	{
		if (nerrors)
			arg_print_errors(stderr, a_end, progname);
		std::cerr << "Usage: " << progname << std::endl;
		arg_print_syntax(stderr, argtable, "\n");
		std::cerr << "Simplest Tox CLI client" << std::endl;
		arg_print_glossary(stderr, argtable, "  %-25s %s\n");
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}

	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	return 0;
}


int ClitoxConfig::error()
{
	return errorcode;
}

void ClitoxConfig::defaultToxOptions(struct Tox_Options *retval)
{
	if (!retval)
		return;
    retval->ipv6_enabled = false;
    retval->udp_enabled = true;
	retval->local_discovery_enabled = true;
	retval->proxy_type = TOX_PROXY_TYPE_NONE;
	retval->proxy_host = NULL;
    retval->proxy_port = 0;
    retval->start_port = 0;
	retval->end_port = 0;
    retval->tcp_port = 0;
	retval->hole_punching_enabled = true;
}

void ClitoxConfig::getToxOptions(struct Tox_Options *retval)
{
	if (!retval)
		return;
	defaultToxOptions(retval);
    retval->ipv6_enabled = toxoptions.ipv6_enabled;
    retval->udp_enabled = toxoptions.udp_enabled;
	retval->local_discovery_enabled = toxoptions.local_discovery_enabled;
	retval->proxy_type = toxoptions.proxy_type;
	retval->proxy_host = toxoptions.proxy_host;
    retval->proxy_port = toxoptions.proxy_port;
    retval->start_port = toxoptions.start_port;
	retval->end_port = toxoptions.end_port;
    retval->tcp_port = toxoptions.tcp_port;
	retval->hole_punching_enabled = toxoptions.hole_punching_enabled;
}

#include "clitox-config.h"
#include <argtable2.h>

static const char* progname = "clitox";
#define DEF_TOX_FILE_NAME	"clitox.tox"

ClitoxConfig::ClitoxConfig()
	: cmd(CMD_RW), file_name(""), nick_name(""), status_message("")
{
}

ClitoxConfig::ClitoxConfig
(
	int argc,
	char* argv[]
)
{
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
	struct arg_lit *a_new_id = arg_lit0("c", "create", "create a new Tox ID");
	struct arg_str *a_file_name = arg_str0("f", "file", "<TOX file name>", "Tox configuration file. Default " DEF_TOX_FILE_NAME);
	struct arg_str *a_nick_name = arg_str0("n", "name", "<nick name>", "nick name");
	struct arg_str *a_status_message = arg_str0("s", "status", "<status message>", "initial status message");
	struct arg_str *a_ids_to = arg_strn("t", "to", "<identifier>", 0, 100, "Send to clients by TOX identifier");
	struct arg_lit *a_help = arg_lit0("h", "help", "Show this help");
	struct arg_end *a_end = arg_end(20);

	void* argtable[] = { 
		a_new_id, a_file_name, a_nick_name, a_ids_to, a_status_message,
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

	if (a_new_id->count)
		cmd = CMD_GET_ID;

	if (cmd == CMD_RW)
	{
		if (a_ids_to->count == 0)
		{
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

	// special case: '--help' takes precedence over error reporting
	if ((a_help->count) || nerrors)
	{
		if (nerrors)
			arg_print_errors(stderr, a_end, progname);
		printf("Usage: %s\n",  progname);
		arg_print_syntax(stdout, argtable, "\n");
		printf("clitox\n");
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
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

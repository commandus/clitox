#include "pkt2mon-config.h"
#include <argtable2.h>

#define DEF_BUFFER_SIZE				4096
#define DEF_PROTO_FOLDER			"proto"
#define DEF_URL_CONTROL				"tcp://0.0.0.0:50000"
#define DEF_URL_OUTPUT				"tcp://0.0.0.0:50001"
#define DEF_TCP_RECEIVER			"tcp://127.0.0.1:50052"
#define DEF_FONT_FILE_PATH			"Roboto-Medium.ttf"
#define DEF_FONT_SIZE				16
#define DEF_FONT_SIZE_S				"16"
#define DEF_WIDTH					640
#define DEF_WIDTH_S					"640"
#define DEF_HEIGHT					480
#define DEF_HEIGHT_S				"480"



static const char* progname = "pkt2mon";


MonitorConfig::MonitorConfig()
	: errorcode(0), proto_path(DEF_PROTO_FOLDER), url_control(DEF_URL_CONTROL), url_output(DEF_URL_OUTPUT), 
	url_tcp_receiver(DEF_TCP_RECEIVER),
	path_font(DEF_FONT_FILE_PATH), buffer_size(DEF_BUFFER_SIZE), 
	font_size(DEF_FONT_SIZE), width(DEF_WIDTH), height(DEF_HEIGHT)
{
	
}

MonitorConfig::MonitorConfig
(
	int argc,
	char* argv[]
)
{
	errorcode = parseCmd(argc, argv);
}

/**
 * Parse command line into MonitorConfig class
 * Return 0- success
 *        1- show help and exit, or command syntax error
 *        2- output file does not exists or can not open to write
 **/
int MonitorConfig::parseCmd
(
	int argc,
	char* argv[]
)
{
	// GTFS https://developers.google.com/transit/gtfs/reference/?csw=1
	struct arg_file *a_proto_path = arg_file0("p", "proto", "<directory path>", "Proto file directory. Default " DEF_PROTO_FOLDER);
	struct arg_str *a_url_control = arg_str0("c", "control", "<URL>", "Control URL e.g. tcp://84.237.111.36:50000. Default " DEF_URL_CONTROL);
	struct arg_str *a_url_output = arg_str0("o", "output", "<URL>", "message bus URL e.g. tcp://84.237.111.36:50001. Default " DEF_URL_OUTPUT);
	struct arg_str *a_url_tcp_receiver = arg_str0("t", "tcp", "<URL>", "TCP test address e.g. tcp://84.237.111.36:50052. Default " DEF_TCP_RECEIVER);
	
	struct arg_int *a_buffer_size = arg_int0("b", "buffer", "<size>", "Receiver buffer size. Default 4096");
	
	struct arg_str *a_path_font = arg_str0("f", "font-file", "<file path>", "Font file path. Default " DEF_FONT_FILE_PATH);
	struct arg_int *a_font_size = arg_int0("s", "font-size", "<size>", "Font size. Default " DEF_FONT_SIZE_S);

	struct arg_int *a_width = arg_int0("w", "screen-width", "<size>", "Initial window width. Default " DEF_WIDTH_S);
	struct arg_int *a_height = arg_int0("h", "screen-height", "<size>", "Initial window height. Default " DEF_HEIGHT_S);

	struct arg_lit *a_help = arg_lit0("h", "help", "Show this help");
	struct arg_end *a_end = arg_end(20);

	void* argtable[] = { 
		a_proto_path, a_url_control, a_url_output, a_url_tcp_receiver,
		a_path_font, a_width, a_height,
		a_buffer_size,
		a_font_size,
		a_help, a_end };

	int nerrors;

	// verify the argtable[] entries were allocated successfully
	if (arg_nullcheck(argtable) != 0)
	{
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}
	// Parse the command line as defined by argtable[]
	nerrors = arg_parse(argc, argv, argtable);

	// special case: '--help' takes precedence over error reporting
	if ((a_help->count) || nerrors)
	{
		if (nerrors)
			arg_print_errors(stderr, a_end, progname);
		printf("Usage: %s\n",  progname);
		arg_print_syntax(stdout, argtable, "\n");
		printf("pkt2mon\n");
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}

	if (a_proto_path->count)
		proto_path = *a_proto_path->filename;
	else
		proto_path = DEF_PROTO_FOLDER;

	if (a_url_control->count)
		url_control = *a_url_control->sval;
	else
		url_control = DEF_URL_CONTROL;

	if (a_url_tcp_receiver->count)
		url_tcp_receiver = *a_url_tcp_receiver->sval;
	else
		url_tcp_receiver = DEF_TCP_RECEIVER;

	if (a_url_output->count)
		url_output = *a_url_output->sval;
	else
		url_output = DEF_URL_OUTPUT;

	if (a_buffer_size->count)
		buffer_size = *a_buffer_size->ival;
	else
		buffer_size = DEF_BUFFER_SIZE;

	if (a_path_font->count)
		path_font = *a_path_font->sval;
	else
		path_font = DEF_FONT_FILE_PATH;
	if (a_font_size->count)
		font_size = *a_font_size->ival;
	else
		font_size = DEF_FONT_SIZE;
	if (a_width->count)
		width = *a_width->ival;
	else
		width = DEF_WIDTH;
	if (a_height->count)
		height = *a_height->ival;
	else
		height = DEF_HEIGHT;

	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	return 0;
}


int MonitorConfig::error()
{
	return errorcode;
}

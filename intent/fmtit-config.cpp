#include "fmtit-config.h"
#include <iostream>
#include <argtable2.h>

static const char* progname = "fmtit";
#define DEF_FILE_NAME			"intenttemplates.json"

FmtitConfig::FmtitConfig()
	: templateName("")
{
}

FmtitConfig::FmtitConfig
(
	int argc,
	char* argv[]
)
{
	errorcode = parseCmd(argc, argv);
}

/**
 * Parse command line into FmtitConfig class
 * Return 0- success
 *        1- show help and exit, or command syntax error
 *        2- output file does not exists or can not open to write
 **/
int FmtitConfig::parseCmd
(
	int argc,
	char* argv[]
)
{
	// GTFS https://developers.google.com/transit/gtfs/reference/?csw=1
	struct arg_str *a_file_names = arg_strn("f", "file", "<file>", 0, 100, "Intent template file name. Default " DEF_FILE_NAME);
	struct arg_str *a_template_name = arg_str1(NULL, NULL, "<template name>", "Template name");
	struct arg_str *a_arguments = arg_strn(NULL, NULL, "<arg>", 0, 200, "Argument");

	struct arg_lit *a_help = arg_lit0("h", "help", "Show this help");
	struct arg_end *a_end = arg_end(20);

	void* argtable[] = { 
		a_file_names, a_template_name, a_arguments,
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

	for (int i = 0; i < a_file_names->count; i++)
	{
		templateFileNames.push_back(a_file_names->sval[i]);
	}
	if (templateFileNames.size() == 0)
		templateFileNames.push_back(DEF_FILE_NAME);

	if (a_template_name->count) 
	{
		templateName = std::string(*a_template_name->sval);
	}
	else
	{
		nerrors++;
	}

	// special case: '--help' takes precedence over error reporting
	if ((a_help->count) || nerrors)
	{
		if (nerrors)
			arg_print_errors(stderr, a_end, progname);
		std::cerr << "Usage: " << progname << std::endl;
		arg_print_syntax(stderr, argtable, "\n");
		std::cerr << "Intent template formatter" << std::endl;
		arg_print_glossary(stderr, argtable, "  %-25s %s\n");
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}

	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	return 0;
}


int FmtitConfig::error()
{
	return errorcode;
}

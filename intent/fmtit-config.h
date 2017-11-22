/**
  * fmtit options
  * @file fmtit-config.h
  **/

#ifndef FMTIT_CONFIG_H
#define FMTIT_CONFIG_H

#include <string>
#include <vector>

/**
 * Command line interface (CLI) tool configuration structure
 */
class FmtitConfig
{
private:
	/**
	* Parse command line into FmtitConfig class
	* Return 0- success
	*        1- show help and exit, or command syntax error
	*        2- output file does not exists or can not open to write
	**/
	int parseCmd
	(
		int argc,
		char* argv[]
	);
	int errorcode;
public:
	int cmd;											///< 0- format, 1- show available intents with args
	std::vector<std::string> templateFileNames;			///< JSON intent template file names
	std::string templateName;							///< arguments
	std::vector<std::string> arguments;					///< arguments
	FmtitConfig();
	FmtitConfig
	(
		int argc,
		char* argv[]
	);
	int error();
};

#endif

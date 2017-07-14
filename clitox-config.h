/**
  * clitox options
  * @file clitox-config.h
  **/

#ifndef CLITOX_CONFIG_H
#define CLITOX_CONFIG_H

#include <string>
#include <vector>

#define CMD_RW				0
#define CMD_PRINT_TOX_ID	1

/**
 * Command line interface (CLI) tool configuration structure
 */
class ClitoxConfig
{
private:
	/**
	* Parse command line into ClitoxConfig class
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
	int cmd;
	std::string file_name;							///< Tox file
	std::string nick_name;							///< nick name
	std::string status_message;						///< initial status message
	std::vector<std::string> ids_to;				///< TOX identifier (in hex)

	ClitoxConfig();
	ClitoxConfig
	(
		int argc,
		char* argv[]
	);
	int error();
};



#endif
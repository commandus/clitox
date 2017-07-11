/**
  * clitox options
  * @file clitox-config.h
  **/

#ifndef CLITOX_CONFIG_H
#define CLITOX_CONFIG_H

#include <string>
#include <vector>

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
	// Protobuf
	std::string id;									///< TOX identifier (in hex)
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

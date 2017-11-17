/**
 * Variable at column of IntentTemplate
 * @see IntentTemplateHelper
 */
#ifndef VARIABLE_COLUMN_H
#define VARIABLE_COLUMN_H

#include <string>

typedef enum Variable 
{
	NONE,
	PHONE,
	IMEI,
	NETWORKID,
	ID,
	NAME,
	NOW,
	LATITUDE,
	LONGITUDE,
	CID,
	OSNAME,
	OSVER,
	MODEL,
	URL_PHOTO,
	URL_AUDIO,
	URL_VIDEO,
	SEL_PHONE,
	SEL_EMAIL,
	SEL_APP,
	INPUT_PHONE,
	INPUT_EMAIL,
	INPUT_HTTP,
	INPUT_NUMBER,
	INPUT_DATE,
	INPUT_TIME,
	INPUT_LINE,
	INPUT_TEXT
} Variable;

class VariableColumn
{
private:
	enum Variable variable;
	int column;
	int idx;
	std::string name;
	int len;
	bool encode;

public:
	VariableColumn
	(
		enum Variable variable,
		int column,
		int idx,
		const std::string &name,
		int len,
		bool encode
	);
	
	enum Variable getVariable();

    void setVariable(enum Variable variable);

    int getColumn();

    void setColumn(int column);

    int getIdx();

    void setIdx(int idx);

    std::string getName();

    void setName(const std::string &name);

    int getLen();

    void setLen(int len);

    bool isEncode();

    void setEncode(bool encode);


	/**
	 * Return string ready to parse with {@link IntentTemplateHelper#parse(std::string)}
	 * Called from {@link IntentTemplate#toString()}
	 * Separators: '|'- intent template attrs, ','- VariableColumn list separator, ':'- VariableColumn attribute separator
	 * where 0- lang 1-image uri 2- name, 3- description, 4- template string, 5..- "," separated column:variable:name:len:e index pairs array
	 * lang|img|name|desc|template|column:variable:name:len:e,... e.g. 0:15:0:0:e,4:16:0:0:e
	 * Special intent extra e.g.
	 *      S.com.commandus.intent.fix.array=android.intent.extra.EMAIL,android.intent.extra.SUBS
	 * @return std::string
	 * @see IntentTemplate#toString()
	 */
	std::string toString();
	
	VariableColumn& operator=(const VariableColumn& other);
};

#endif

#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <map>

class ClientList
{
public:
	
	static ClientList& getInstance()
	{
		static ClientList instance; // Guaranteed to be destroyed.
		return instance;
	}
	void *get(void *key);
	void put(void *key, void *value);
	void remove(void *key);
private:
	ClientList();                    // Constructor? (the {} brackets) are needed here.

	ClientList(ClientList const&);     // Don't Implement
	void operator=(ClientList const&); // Don't implement
	
	std::map<void *, void*> map;
	
};
#endif // CLIENTLIST_H

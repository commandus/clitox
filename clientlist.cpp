#include "clientlist.h"

ClientList::ClientList()
{

}

void *ClientList::get(void *key)
{
	std::map<void *, void *>::const_iterator it = map.find(key);
	if (it != map.end())
	{
		return it->second;
	}
	else
		return NULL;
}

void ClientList::put(void *key, void *value)
{
	map[key] = value;
}

void ClientList::remove(void *key)
{
	std::map<void *, void *>::const_iterator it = map.find(key);
	if (it != map.end())
		map.erase(it);
}

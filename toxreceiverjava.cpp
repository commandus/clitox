#include "toxreceiverjava.h"
#ifdef __ANDROID__

#include "sodium.h"
#include "sodium/utils.h"

ToxReceiverJava::ToxReceiverJava
(
		JNIEnv *env,
        jobject obj
)
    :   jenv(env)
{
    jcls = env->GetObjectClass(obj);
    jOnId = env->GetMethodID(jcls, "onId", "(Ljava/lang/String;)V");
    jOnConnectionStatus = env->GetMethodID(jcls, "onConnectionStatus", "(ILjava/lang/String;)V");
    jOnMessage = env->GetMethodID(jcls, "onMessage", "(ILjava/lang/String;Ljava/lang/String;)V");
    jOnFriendRequest = env->GetMethodID(jcls, "onFriendRequest", "(Ljava/lang/String;Ljava/lang/String;)V");
    jNextMessageTo = env->GetMethodID(jcls, "nextMessageTo", "(IILjava/lang/String;)V");
}

ToxReceiverJava::~ToxReceiverJava()
{

}

void ToxReceiverJava::onId
(
	// Tox *tox,
	ToxClient *toxclient,
	const std::string &valuehex
)
{
    if (jOnId) {
        jstring jstr = jenv->NewStringUTF(valuehex.c_str());
        jenv->CallVoidMethod(jcls, jOnId, jstr);
    }
}

void ToxReceiverJava::onConnectionStatus
(
	// Tox *tox,
	ToxClient *toxclient,
	TOX_CONNECTION value
)
{
	std::string s;
	switch(value) 
	{
		case TOX_CONNECTION_TCP:
			s = "TCP connection established";
			break;
		case TOX_CONNECTION_UDP:
			s = "UDP connection established";
			break;
		default:
			s = "offline";
	}
    if (jOnConnectionStatus) {
        jstring jstr = jenv->NewStringUTF(s.c_str());
        jenv->CallVoidMethod(jcls, jOnConnectionStatus, (int) value, jstr);
    }
}

void ToxReceiverJava::onMessage
(
	ToxClient *toxclient,
	uint32_t friend_number, 
	const std::string &value,
	void *user_data
)
{
    if (jOnConnectionStatus) {
        jstring jFriendName = jenv->NewStringUTF(toxclient->getFriendName(friend_number).c_str());
        jstring jValue = jenv->NewStringUTF(value.c_str());
        jenv->CallVoidMethod(jcls, jOnMessage, friend_number, jFriendName, jValue);
    }
}

bool ToxReceiverJava::onFriendRequest
(
	ToxClient *toxclient,
	const uint8_t *key, 
	const std::string &name,
	void *user_data
)
{
    if (jOnConnectionStatus) {
        jstring jFriendName = jenv->NewStringUTF(name.c_str());
        char tox_id_hex[TOX_ADDRESS_SIZE * 2 + 1];
        tox_id_hex[TOX_ADDRESS_SIZE * 2] = '\0';
        sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), key, TOX_ADDRESS_SIZE);
        for (size_t i = 0; i < TOX_ADDRESS_SIZE * 2; i ++)
        {
            tox_id_hex[i] = toupper(tox_id_hex[i]);
        }
        jstring jKeyHex = jenv->NewStringUTF(tox_id_hex);
        jenv->CallVoidMethod(jcls, jOnId, jKeyHex, jFriendName);
    }
	return true;
}

bool ToxReceiverJava::nextMessageTo
(
	uint32_t *friend_number,
	TOX_MESSAGE_TYPE *message_type,
	std::string *text
)
{
	bool r = messages.size();
	if (r)
	{
		ToxMessage m = messages.front();
		messages.pop();
		*friend_number = m.friend_number;
		*message_type = m.message_type;
		*text = m.message;
	}
	return r;
}

void ToxReceiverJava::readLine
(
	ToxClient *toxclient
)
{
	std::string line("Test");
	ToxMessage m;
	m.friend_number = 0;
	m.message_type = TOX_MESSAGE_TYPE_NORMAL;
	m.message = line;
	messages.push(m);
}

#endif

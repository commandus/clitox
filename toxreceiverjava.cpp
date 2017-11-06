#include "toxreceiverjava.h"
#ifdef __ANDROID__

#include "sodium.h"
#include "sodium/utils.h"
#include <android/log.h>

ToxReceiverJava::ToxReceiverJava
(
		JNIEnv *env,
        jobject obj,
		defaultMessageType(default_message_type)
)
{
    __android_log_write(ANDROID_LOG_INFO, "ToxReceiverJava", "Init");
    jenv = env;
    jobj = reinterpret_cast<jobject> (jenv->NewGlobalRef(obj));
    jcls = reinterpret_cast<jclass>(jenv->NewGlobalRef(jenv->GetObjectClass(jobj)));

    jOnId = jenv->GetMethodID(jcls, "onId", "(Ljava/lang/String;)V");
    jOnConnectionStatus = jenv->GetMethodID(jcls, "onConnectionStatus", "(ILjava/lang/String;)V");
    jOnMessage = jenv->GetMethodID(jcls, "onMessage", "(ILjava/lang/String;Ljava/lang/String;)V");
    jOnFriendRequest = jenv->GetMethodID(jcls, "onFriendRequest", "(Ljava/lang/String;Ljava/lang/String;)V");
    jNextMessageTo = jenv->GetMethodID(jcls, "nextMessageTo", "(IILjava/lang/String;)V");
}

ToxReceiverJava::~ToxReceiverJava()
{
    jenv->DeleteGlobalRef(jobj);
    jenv->DeleteGlobalRef(jcls);
    __android_log_write(ANDROID_LOG_INFO, "ToxReceiverJava", "Done");
}

void ToxReceiverJava::onId
(
	// Tox *tox,
	ToxClient *toxclient,
	const std::string &valuehex
)
{
	// jmethodID jOnId = jenv->GetMethodID(jcls, "onId", "(Ljava/lang/String;)V");
    if (jOnId) {
        jstring jstr = jenv->NewStringUTF(valuehex.c_str());
        jenv->CallVoidMethod(jobj, jOnId, jstr);
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
    // jmethodID jOnConnectionStatus = jenv->GetMethodID(jcls, "onConnectionStatus", "(ILjava/lang/String;)V");
    if (jOnConnectionStatus) {
        jstring jstr = jenv->NewStringUTF(s.c_str());
		jint jvalue = value;
        jenv->CallVoidMethod(jobj, jOnConnectionStatus, jvalue, jstr);
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
	// jmethodID jOnMessage = jenv->GetMethodID(jcls, "onMessage", "(ILjava/lang/String;Ljava/lang/String;)V");
    if (jOnMessage) {
        jstring jFriendName = jenv->NewStringUTF(toxclient->getFriendName(friend_number).c_str());
        jstring jValue = jenv->NewStringUTF(value.c_str());
        jenv->CallVoidMethod(jobj, jOnMessage, friend_number, jFriendName, jValue);
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
    // jmethodID jOnFriendRequest = jenv->GetMethodID(jcls, "onFriendRequest", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (jOnFriendRequest) {
        jstring jFriendName = jenv->NewStringUTF(name.c_str());
        char tox_id_hex[TOX_ADDRESS_SIZE * 2 + 1];
        tox_id_hex[TOX_ADDRESS_SIZE * 2] = '\0';
        sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), key, TOX_ADDRESS_SIZE);
        for (size_t i = 0; i < TOX_ADDRESS_SIZE * 2; i ++)
        {
            tox_id_hex[i] = toupper(tox_id_hex[i]);
        }
        jstring jKeyHex = jenv->NewStringUTF(tox_id_hex);
        jenv->CallVoidMethod(jobj, jOnFriendRequest, jKeyHex, jFriendName);
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
	m.message_type defaultMessageType;
	m.message = line;
	messages.push(m);
}

#endif

#ifndef TOXRECEIVERJAVA_H
#define TOXRECEIVERJAVA_H
#ifdef __ANDROID__
#include <jni.h>
#include <android/log.h>

#include <inttypes.h>
#include <iostream>
#include <queue>
#include <tox/tox.h>
#include "toxreceiver.h"
#include "toxclient.h"
#include "toxmessage.h"

class ToxReceiverJava : public ToxReceiver
{
private:
    JNIEnv *jenv;
    jclass jcls;
    jobject jobj;
	jmethodID jOnId;
	jmethodID jOnConnectionStatus;
	jmethodID jOnMessage;
	jmethodID jOnFriendRequest;
	jmethodID jNextMessageTo;

	std::queue<ToxMessage> messages;
public:
	ToxReceiverJava
	(
        JNIEnv *env,
        jobject obj
	);
    virtual ~ToxReceiverJava();

	virtual void onId(
		// Tox *tox,
		ToxClient *toxclient,
		const std::string &valuehex
	);
	virtual void onConnectionStatus(
		// Tox *tox,
		ToxClient *toxclient,
		TOX_CONNECTION value
	);
	virtual void onMessage
	(
		// Tox *tox,
		ToxClient *toxclient,
		uint32_t friend_number, 
		const std::string &value,
		void *user_data
	);
	virtual bool onFriendRequest(
		// Tox *tox,
		ToxClient *toxclient,
		const uint8_t *key, 
		const std::string &name,
		void *user_data
	);
	virtual bool nextMessageTo
	(
		uint32_t *friend_number,
		TOX_MESSAGE_TYPE *message_type,
		std::string *m
	);
	void readLine
	(
		ToxClient *toxclient
	);
};

#endif
#endif

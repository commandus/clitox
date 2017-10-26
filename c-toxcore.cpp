#include "c-toxcore.h"
#include "toxclient.h"
extern "C"
JNIEXPORT jstring JNICALL
Java_com_commandus_rtox_Native_initTox
(
    JNIEnv *jenv,
    jobject jcaller,
    jobject jobj
)
{
    ToxClient client;
    client.setReceiverJava(jenv, jobj);

    struct Tox_Options options;

    tox_options_default(&options);
    Tox *tox = tox_new(&options, NULL);
    std::stringstream ss;
    ss << "Tox " << options.tcp_port;
    std::string hello = ss.str();
    return jenv->NewStringUTF(hello.c_str());
}

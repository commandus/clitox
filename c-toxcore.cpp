#include "c-toxcore.h"
#include "toxclient.h"

std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

extern "C"
{

JNIEXPORT jlong JNICALL
Java_com_commandus_rtox_ToxClient_createClient(
        JNIEnv *jenv,
        jobject jcaller,
        jobject jobj,
        jstring filename,
        jstring nick,
        jstring status
)
{
    struct Tox_Options *toxoptions = NULL;
    std::vector<struct DHT_node> nodes;

    std::string sfilename = jstring2string(jenv, filename);
    std::string snick = jstring2string(jenv, nick);
    std::string sstatus = jstring2string(jenv, status);
    ToxClient *client = new ToxClient(
        toxoptions, nodes, sfilename, snick, sstatus
    );
    client->setReceiverJava(jenv, jobj);
    return (jlong) client;
}

JNIEXPORT void JNICALL
Java_com_commandus_rtox_ToxClient_destroyClient(
        JNIEnv *jenv,
        jobject jcaller,
        jlong jclient
)
{
    ToxClient *client = (ToxClient *) jclient;
    if (client)
        delete client;
}

JNIEXPORT jstring JNICALL
Java_com_commandus_rtox_ToxClient_getId(
        JNIEnv *jenv,
        jobject jcaller,
        jlong jclient
)
{
    ToxClient *client = (ToxClient *) jclient;
    std::string r = "";
    if (client)
        r = client->getIdHex();
    return jenv->NewStringUTF(r.c_str());
}

JNIEXPORT jint JNICALL
Java_com_commandus_rtox_ToxClient_run(
        JNIEnv *jenv,
        jobject jcaller,
        jlong jclient
)
{
    int r = -1;
    ToxClient *client = (ToxClient *) jclient;
    if (client)
        r = client->run();
    return r;
}

JNIEXPORT jstring JNICALL
Java_com_commandus_rtox_ToxClient_getFriendId(
        JNIEnv *jenv,
        jobject jcaller,
        jlong jclient,
        jint  jfriend
)
{
    ToxClient *client = (ToxClient *) jclient;
    std::string r = "";
    if (client)
        r = client->getFriendId(jfriend);
    return jenv->NewStringUTF(r.c_str());
}

}
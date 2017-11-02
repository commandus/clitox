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
        jobjectArray jnodes,
        jstring filename,
        jstring nick,
        jstring status
)
{
    struct Tox_Options *toxoptions = NULL;

    std::vector<struct DHT_node> nodes;
    jsize nodesCount = jenv->GetArrayLength(jnodes);
    jclass java_node_cls = jenv->FindClass("com/commandus/data/BootstrapNode");
    jfieldID fidIP = jenv->GetFieldID(java_node_cls, "ip", "Ljava/lang/String;");
    jfieldID fidPort = jenv->GetFieldID(java_node_cls, "port", "I");
    jfieldID fidKey = jenv->GetFieldID(java_node_cls, "publicKey", "Ljava/lang/String;");
    for (jsize i = 0; i < nodesCount; i++) {
        jobject o = jenv->GetObjectArrayElement(jnodes, i);
        jstring jip = (jstring) jenv->GetObjectField(o, fidIP);
        jint jport = jenv->GetIntField(o, fidPort);
        jstring jpublickey = (jstring) jenv->GetObjectField(o, fidKey);
        struct DHT_node n;
        n.ip = jstring2string(jenv, jip);
        n.port = jport;
        n.key_hex = jstring2string(jenv, jpublickey);
        nodes.push_back(n);
    }

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
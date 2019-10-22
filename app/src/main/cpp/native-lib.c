#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"

#define SIG_NUMBER_MAX 32

static JavaVM *j_vm = NULL;
static jclass j_class = NULL;

static jint debug = 1;

/**
 * 静态注册的Native方法
 * @param env
 * @param arg
 * @return
 */
JNIEXPORT jstring JNICALL Java_com_cloudwise_ndk_NDKUtil_stringFromJNI(
        JNIEnv *env,
        jclass arg) {
    jstring hello = "Hello ---- from ---- C";
    if(debug == 1){
        LOGE("stringFromJNI ---- Hello ---- from ---- C");
    }

    return (*env)->NewStringUTF(env, hello);
}

/**
 * 静态注册的Native方法
 * @param env
 * @param arg
 * @param type
 */
JNIEXPORT void JNICALL Java_com_cloudwise_ndk_NDKUtil_setJNILogEnable(
        JNIEnv *env,
        jclass arg, jint type){
    if(type == 1){
        debug = 1;
    } else {
        debug = 0;
    }
}

/**
 * 动态注册的Native方法
 * @param env
 * @param arg
 * @return
 */
JNIEXPORT jint JNICALL getVersionCode(JNIEnv *env, jclass arg){
    if(debug == 1){
        LOGE("getVersionCode ----------- 10");
    }
    return 10;
}

/**
 * 动态注册的Native方法
 * @param env
 * @param arg
 * @param code
 * @return
 */
JNIEXPORT jstring JNICALL getVersion(JNIEnv *env, jclass arg, jint code){
    if(debug == 1){
        LOGE("getVersion ----------- 1.2.6");
    }
    jstring ver = "1.2.6";
    return (*env)->NewStringUTF(env, ver);
}

/**
 * C调用Java方法（带参数不带返回值）
 */
void callVoidFromJava(){
    JNIEnv *env;
    (*j_vm)->AttachCurrentThread(j_vm, &env, NULL);
    jmethodID methodid = (*env)->GetStaticMethodID(env, j_class, "getVoidToC", "(ILjava/lang/String;)V");
    (*env)->CallStaticVoidMethod(env, j_class, methodid, 10, (*env)->NewStringUTF(env, "C-Name"));
    if(debug == 1) {
        LOGE("callVoidFromJava Java To C");
    }
}

/**
 * C调用Java方法（带参数带返回值）
 */
void callStringFromJava(){
    JNIEnv *env;
    (*j_vm)->AttachCurrentThread(j_vm, &env, NULL);
    jmethodID methodid = (*env)->GetStaticMethodID(env, j_class, "getStringToC", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring str = (jstring)(*env)->CallStaticObjectMethod(env, j_class, methodid,(*env)->NewStringUTF(env, "C-Name"));
    char* java = (char*)(*env)->GetStringUTFChars(env, str, NULL);
    if(debug == 1) {
        LOGE("callStringFromJava Java To C : %s", java);
    }
}

/**
 * 动态注册的Native方法，然后调用Java方法
 * @param env
 * @param arg
 */
JNIEXPORT void JNICALL callJavaString(JNIEnv *env, jclass arg) {
    callStringFromJava();
}

/**
 * 动态注册的Native方法，然后调用Java方法
 * @param env
 * @param arg
 */
JNIEXPORT void JNICALL callJavaVoid(JNIEnv *env, jclass arg) {
    callVoidFromJava();
}

static JNINativeMethod mMethods[] = {
        {"getVersionCode", "()I", (void*)getVersionCode},
        {"getVersion", "(I)Ljava/lang/String;", (void*)getVersion},
        {"callJavaString", "()V", (void*)callJavaString},
        {"callJavaVoid", "()V", (void*)callJavaVoid}
};

static int registerNativeMethods(JNIEnv* env, const char* className, const JNINativeMethod* gMethods, int numMethods){
    jclass clazz;
    clazz = (*env)->FindClass(env, className);

    if(clazz == NULL){
        return -1;
    }

    j_class = (jclass)(*env) -> NewGlobalRef(env, (jobject)clazz);

    if((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0){
        return -1;
    }

    return 0;
}

static int registerNative(JNIEnv* env){
    return registerNativeMethods(env, "com/cloudwise/ndk/NDKUtil", mMethods, sizeof(mMethods)/ sizeof(mMethods[0]));
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
    LOGE("JNI_OnLoad ------------------------------ ");
    JNIEnv* env = NULL;
    jint result = -1;
    /*
     * JavaVM::GetEnv 原型为 jint (*GetEnv)(JavaVM*, void**, jint);
     * GetEnv()函数返回的  Jni 环境对每个线程来说是不同的，
     * 由于Dalvik虚拟机通常是Multi-threading的。每一个线程调用JNI_OnLoad()时，
     * 所用的JNI Env是不同的，因此我们必须在每次进入函数时都要通过vm->GetEnv重新获取
     */
    if((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_4) != JNI_OK){
        return -1;
    }

    if(registerNative(env) != JNI_OK){
        return -1;
    }

    j_vm = vm;

    //cloudwise_init(1);

    result = JNI_VERSION_1_4;
    return result;
}

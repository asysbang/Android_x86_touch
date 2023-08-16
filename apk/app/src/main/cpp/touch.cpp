#include <jni.h>

// Write C++ code here.
//
// Do not forget to dynamically load the C++ library into your application.
//
// For instance,
//
// In MainActivity.java:
//    static {
//       System.loadLibrary("touch");
//    }
//
// Or, in MainActivity.kt:
//    companion object {
//      init {
//         System.loadLibrary("touch")
//      }
//    }
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "android/log.h"


#define LOG_TAG    "touch_native"
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, __VA_ARGS__)

int clientfd;

extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_getWidth(JNIEnv *env, jobject thiz) {
    const char* msg = "widt";
    write(clientfd, msg, strlen(msg));
    //char buf[100];
//    int read_len = read(clientfd, &buf, sizeof(buf));
//    LOGE("========client read %s ",buf);
// ???????????????接收字符串没问题，但是int不对，应该是数据大小或者大小尾端问题？？？？？？？？？？？？1987208563
    int width=-1;
    int read_len = read(clientfd, &width, sizeof(width));
    LOGE("========width %d xxx %d",width,read_len);
    return width;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_getHeight(JNIEnv *env, jobject thiz) {
    const char* msg = "heig";
    write(clientfd, msg, strlen(msg));
    //char buf[100];
//    int read_len = read(clientfd, &buf, sizeof(buf));
//    LOGE("========client send %s ",msg);
// ???????????????接收字符串没问题，但是int不对，应该是数据大小或者大小尾端问题？？？？？？？？？？？？1987208563
    int height=-1;
    int read_len = read(clientfd, &height, sizeof(height));
    LOGE("========height %d xxx %d",height,read_len);
    return height;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_getRgb(JNIEnv *env, jobject thiz, jint x, jint y) {
    LOGE("========Java_com_asysbang_touch_NativeHelper_getRgb");
    const char* msg = "rgb1";
    write(clientfd, msg, strlen(msg));
    int px = x;
    write(clientfd, &px, sizeof(px));
    int py = y;
    write(clientfd, &py, sizeof(py));
    unsigned char r ,g,b;
    int read_len = read(clientfd, &r, sizeof(r));

    read_len = read(clientfd, &g, sizeof(g));

    read_len = read(clientfd, &b, sizeof(b));
    LOGE("========native getRgb is  [%d,%d,%d]",r,g,b);
    return r;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_getPng(JNIEnv *env, jobject thiz) {
    LOGE("========Java_com_asysbang_touch_NativeHelper_getPng");
    const char* msg = "png";
    write(clientfd, msg, strlen(msg));
    LOGE("========getPng write ");
    unsigned char rgb_data[1024 * 768 * 4];
    int read_len = read(clientfd, &rgb_data, sizeof(rgb_data));
    LOGE("========getPng %d ",read_len);
    return 1;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_getNewPng(JNIEnv *env, jobject thiz) {
    LOGE("========native getNewPng");
    const char* msg = "png1";
    write(clientfd, msg, strlen(msg));
    LOGE("========getPng write over");
    //read(clientfd, &g, sizeof(g));
    //unsigned char rgb_data[1024 * 768 *4];
//    LOGE("========getPng111 %d ",sizeof(rgb_data));
    //int read_len = read(clientfd, rgb_data, sizeof(rgb_data));
    //LOGE("========getPng222 %d ",read_len);

    unsigned char rgb_data[1024 * 4*100];
    int read_len = read(clientfd, rgb_data, sizeof(rgb_data));
    while (read_len>0) {
        LOGE("========read data %d",read_len);
        read_len = read(clientfd, rgb_data, sizeof(rgb_data));
    }

//    unsigned char rgb_data1[1024 * 768 *2];
//    LOGE("========getPng %d ",sizeof(rgb_data1));
//    int read_len1 = read(clientfd, &rgb_data1, sizeof(rgb_data1));
//    LOGE("========getPng %d ",read_len1);
    return -11;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_connectServer(JNIEnv *env, jobject thiz) {
    clientfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (-1 == clientfd) {
        LOGE("========client read1 111111111111");
        return -1;
    }
    sockaddr_un clientaddr;
    bzero(&clientaddr, sizeof(clientaddr));
    strcpy(clientaddr.sun_path + 1, "/data/user/0/com.asysbang.touch/files/mysocket");
    clientaddr.sun_family = AF_LOCAL;
    socklen_t addrlen = 1 + strlen("/data/user/0/com.asysbang.touch/files/mysocket")
                        + sizeof(clientaddr.sun_family);
    int ret = connect(clientfd, (struct sockaddr *) &clientaddr, addrlen);
    if (-1 == ret) {
        LOGE("========client read1 2222222");
        return -2;
    }
    return 1;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_asysbang_touch_NativeHelper_testGetByte(JNIEnv *env, jobject thiz) {
    // TODO: implement testGetByte()
    return nullptr;
}

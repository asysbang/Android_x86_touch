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
    const char* msg = "width";
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
    const char* msg = "height";
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
Java_com_asysbang_touch_NativeHelper_getPng(JNIEnv *env, jobject thiz) {
    const char* msg = "png";
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
Java_com_asysbang_touch_NativeHelper_connectServer(JNIEnv *env, jobject thiz) {
    clientfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (-1 == clientfd) {
        LOGE("========client read1 111111111111");
        return -1;
    }
    sockaddr_un clientaddr;
    bzero(&clientaddr, sizeof(clientaddr));
    strcpy(clientaddr.sun_path + 1, "/tmp/mysocket");
    clientaddr.sun_family = AF_LOCAL;
    socklen_t addrlen = 1 + strlen("/tmp/mysocket")
                        + sizeof(clientaddr.sun_family);
    int ret = connect(clientfd, (struct sockaddr *) &clientaddr, addrlen);
    if (-1 == ret) {
        LOGE("========client read1 2222222");
        return -2;
    }
    return 1;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_getRgb(JNIEnv *env, jobject thiz, jint x, jint y) {
    LOGE("========Java_com_asysbang_touch_NativeHelper_getRgb");
    const char* msg = "rgb";
    write(clientfd, msg, strlen(msg));
    int px = x;
    write(clientfd, &px, sizeof(px));
    int py = y;
    write(clientfd, &py, sizeof(py));
    unsigned char r ,g,b;
    int read_len = read(clientfd, &r, sizeof(r));
    LOGE("========height %d xxx %d",r,read_len);

    read_len = read(clientfd, &g, sizeof(g));
    LOGE("========height %d xxx %d",g,read_len);

    read_len = read(clientfd, &b, sizeof(b));
    LOGE("========height %d xxx %d",b,read_len);
    return r;
}
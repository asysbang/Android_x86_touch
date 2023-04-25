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
extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_getWidth(JNIEnv *env, jobject thiz) {
    // TODO: implement getWidth()
    int clientfd;
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
    const char* msg = "client send hello\n";
    write(clientfd, msg, strlen(msg));
    //char buf[100];
//    int read_len = read(clientfd, &buf, sizeof(buf));
//    LOGE("========client read %s ",buf);
// ???????????????接收字符串没问题，但是int不对，应该是数据大小或者大小尾端问题？？？？？？？？？？？？
    int width=-1;
    int read_len = read(clientfd, &width, sizeof(width));
    LOGE("========client read1 %d xxx %d",width,read_len);
    return width;
}
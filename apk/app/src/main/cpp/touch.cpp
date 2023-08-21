#include <jni.h>
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
    const char *msg = "widt";
    write(clientfd, msg, strlen(msg));
    //char buf[100];
//    int read_len = read(clientfd, &buf, sizeof(buf));
//    LOGE("========client read %s ",buf);
// ???????????????接收字符串没问题，但是int不对，应该是数据大小或者大小尾端问题？？？？？？？？？？？？1987208563
    int width = -1;
    int read_len = read(clientfd, &width, sizeof(width));
    LOGE("========width %d xxx %d", width, read_len);
    return width;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_getHeight(JNIEnv *env, jobject thiz) {
    const char *msg = "heig";
    write(clientfd, msg, strlen(msg));
    //char buf[100];
//    int read_len = read(clientfd, &buf, sizeof(buf));
//    LOGE("========client send %s ",msg);
// ???????????????接收字符串没问题，但是int不对，应该是数据大小或者大小尾端问题？？？？？？？？？？？？1987208563
    int height = -1;
    int read_len = read(clientfd, &height, sizeof(height));
    LOGE("========height %d xxx %d", height, read_len);
    return height;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_asysbang_touch_NativeHelper_getNewPng(JNIEnv *env, jobject thiz) {
    LOGE("========native getNewPng");
    const char *msg = "png1";
    write(clientfd, msg, strlen(msg));
    LOGE("========getPng write over");
    //read(clientfd, &g, sizeof(g));
    //unsigned char rgb_data[1024 * 768 *4];
//    LOGE("========getPng111 %d ",sizeof(rgb_data));
    //int read_len = read(clientfd, rgb_data, sizeof(rgb_data));
    //LOGE("========getPng222 %d ",read_len);
    //jintArray jntarray = (*env).NewIntArray(1024 * 768 * 4);
    //数组转化为 jint* 类型
    //jint *native_array = (*env).GetIntArrayElements(jntarray, NULL);

//    unsigned char *buf;
//    read(clientfd, buf, 3);
//    LOGE("========native getRgb is 000 [%d,%d,%d]", buf[0], buf[1], buf[2]);
//    jcharArray nativeArray = (*env).NewCharArray(3);
//    jchar *nByte = (*env).GetCharArrayElements(nativeArray, NULL);
//    for (int i = 0; i < 3; i++) {
//        nByte[i] = buf[i];
//    }


    int targetSize = 1024 * 4 * 768;
    LOGE("========read 1111");
    unsigned char *targetData = (unsigned char *) malloc(targetSize * sizeof(unsigned char));
    size_t getSize = 0;
    unsigned char *targetDataPoint = targetData;

    while (getSize < targetSize) {
        int readLen = read(clientfd, targetDataPoint, targetSize - getSize);
        if (readLen > 0) {
            getSize += readLen;
            targetDataPoint += readLen;
            LOGE("========read getSize is %d", getSize);
        } else {
            LOGE("========read error ??????????????");
        }
    }
    LOGE("========read over %d,%d,%d", targetData[1024 * 4 * 53 + 53 * 4],
         targetData[1024 * 4 * 53 + 53 * 4 + 1], targetData[1024 * 4 * 53 + 53 * 4 + 2]);

    jbyteArray nativeArray = (*env).NewByteArray(targetSize);
    jbyte *native_array = (*env).GetByteArrayElements(nativeArray, NULL);
    for (int i = 0; i < targetSize; i++) {
        native_array[i] = targetData[i];
    }


    //(*env).SetByteArrayRegion(nativeArray, 0, targetSize, reinterpret_cast<const jbyte *>(targetData));

    LOGE("========after into byte [%d,%d,%d]", native_array[1024 * 4 * 53 + 53 * 4],
         native_array[1024 * 4 * 53 + 53 * 4 + 1], native_array[1024 * 4 * 53 + 53 * 4 + 2]);

//    unsigned char rgb_data[1024 * 4 * 100];
//    int t = 0;
//    int read_len = read(clientfd, rgb_data, sizeof(rgb_data));
//    while (read_len > 0) {
//        t += read_len;
////        LOGE("========read1111111 data %d", read_len);// 1536*2048
//        LOGE("========read 111111t %d", t);// 1536*2048
//        read_len = read(clientfd, rgb_data, sizeof(rgb_data));
//        LOGE("========read1111111 data %d", read_len);// 1536*2048
//
//    }
//    LOGE("========read111 over");
//    LOGE("========read 111111t %d", t);// 1536*2048
//    unsigned char rgb_data1[1024 * 768 *2];
//    LOGE("========getPng %d ",sizeof(rgb_data1));
//    int read_len1 = read(clientfd, &rgb_data1, sizeof(rgb_data1));
//    LOGE("========getPng %d ",read_len1);
    //(*env).ReleaseIntArrayElements(jntarray, native_array, 0);
    (*env).ReleaseByteArrayElements(nativeArray, native_array, 0);
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
JNIEXPORT jintArray JNICALL
Java_com_asysbang_touch_NativeHelper_getRgbNew(JNIEnv *env, jobject thiz, jint x, jint y) {
    LOGE("========Java_com_asysbang_touch_NativeHelper_getRgbNew");
    const char *msg = "rgb1";
    write(clientfd, msg, strlen(msg));
    int px = x;
    write(clientfd, &px, sizeof(px));
    int py = y;
    write(clientfd, &py, sizeof(py));
//
//    jbyteArray nativeArray = (*env).NewByteArray(3);
//    jbyte* nByte  = (*env).GetByteArrayElements(nativeArray,NULL);
//    read(clientfd, nByte, sizeof(nByte));

    unsigned char r, g, b;
    int read_len = read(clientfd, &r, sizeof(r));
    read_len = read(clientfd, &g, sizeof(g));
    read_len = read(clientfd, &b, sizeof(b));
    LOGE("========native getRgb is  [%d,%d,%d]", r, g, b);

    jintArray jntarray = (*env).NewIntArray(3);
    //数组转化为 jint* 类型
    jint *native_array = (*env).GetIntArrayElements(jntarray, NULL);
    native_array[0] = r;
    native_array[1] = g;
    native_array[2] = b;
    (*env).ReleaseIntArrayElements(jntarray, native_array, 0);
    //返回
    return jntarray;

}
extern "C"
JNIEXPORT jcharArray JNICALL
Java_com_asysbang_touch_NativeHelper_getRgb(JNIEnv *env, jobject thiz, jint x, jint y) {
    LOGE("========Java_com_asysbang_touch_NativeHelper_getRgb");
    const char *msg = "rgb1";
    write(clientfd, msg, strlen(msg));
    int px = x;
    write(clientfd, &px, sizeof(px));
    int py = y;
    write(clientfd, &py, sizeof(py));
//??????????????????????????????????怎么转换？？？？？？？？？？？？？？
    unsigned char *buf;
    read(clientfd, buf, 3);
    LOGE("========native getRgb is 000 [%d,%d,%d]", buf[0], buf[1], buf[2]);
    jcharArray nativeArray = (*env).NewCharArray(3);
    jchar *nByte = (*env).GetCharArrayElements(nativeArray, NULL);
    for (int i = 0; i < 3; i++) {
        nByte[i] = buf[i];
    }
    LOGE("========native getRgb is [%d,%d,%d]", nByte[0], nByte[1], nByte[2]);
    (*env).ReleaseCharArrayElements(nativeArray, nByte, 0);
    return nativeArray;
}
extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_asysbang_touch_NativeHelper_getRgb1(JNIEnv *env, jobject thiz, jint x, jint y) {
    LOGE("========Java_com_asysbang_touch_NativeHelper_getRgb");
    const char *msg = "rgb1";
    write(clientfd, msg, strlen(msg));
    int px = x;
    write(clientfd, &px, sizeof(px));
    int py = y;
    write(clientfd, &py, sizeof(py));
    unsigned char buf[3];
    read(clientfd, buf, 3);
    LOGE("========native getRgb is 111111 [%d,%d,  %d]", buf[0], buf[1], buf[2]);
    jbyteArray nativeArray = (*env).NewByteArray(3);
    (*env).SetByteArrayRegion(nativeArray, 0, 3, reinterpret_cast<const jbyte *>(buf));
    LOGE("========native getRgb is 111111 [%d,%d,  %d]", nativeArray[0], nativeArray[1],
         nativeArray[2]);


    return nativeArray;
}
extern "C"
JNIEXPORT jintArray JNICALL
Java_com_asysbang_touch_NativeHelper_testBitmap(JNIEnv *env, jobject thiz, jintArray pix,
                                                jint width, jint height) {
    LOGE("========Java_com_asysbang_touch_NativeHelper_testBitmap");
    const char *msg = "png1";
    write(clientfd, msg, strlen(msg));
    int targetSize = 1024 * 4 * 768;
    unsigned char *targetData = (unsigned char *) malloc(targetSize * sizeof(unsigned char));
    size_t getSize = 0;
    unsigned char *targetDataPoint = targetData;
    while (getSize < targetSize) {
        int readLen = read(clientfd, targetDataPoint, targetSize - getSize);
        if (readLen > 0) {
            getSize += readLen;
            targetDataPoint += readLen;
            LOGE("========read getSize is %d", getSize);
        } else {
            LOGE("========read error ??????????????");
        }
    }
    LOGE("========read over %d,%d,%d", targetData[1024 * 4 * 53 + 53 * 4],
         targetData[1024 * 4 * 53 + 53 * 4 + 1], targetData[1024 * 4 * 53 + 53 * 4 + 2]);
    jintArray nativeArray = env->NewIntArray(targetSize / 4);
    jint *native_array = env->GetIntArrayElements(nativeArray, NULL);
    int index = 0;
    for (int i = 0; i < targetSize;) {

        int r = targetData[i];
        int g = targetData[i + 1];
        int b = targetData[i + 2];
        int a = targetData[i + 3];
        //转argb
        native_array[index] = ((r << 16) | (g << 8) | (b) | (a << 24));
        if (index == 1024 * 53 + 53) {
            LOGE("=======xxxxxx %d %d %d", r, g, b, a);
        }
        index++;
        i = i + 4;
    }
    LOGE("========after into pixel is %d", native_array[1024 * 53 + 53]);
    env->ReleaseIntArrayElements(nativeArray, native_array, 0);
    return nativeArray;
}
extern "C"
JNIEXPORT jintArray JNICALL
Java_com_asysbang_touch_NativeHelper_getBitmapPixels(JNIEnv *env, jobject thiz,jint width, jint height) {
    const char *msg = "png1";
    write(clientfd, msg, strlen(msg));
    int targetSize = width * 4 * height;
    unsigned char *targetData = (unsigned char *) malloc(targetSize * sizeof(unsigned char));
    size_t getSize = 0;
    unsigned char *targetDataPoint = targetData;
    while (getSize < targetSize) {
        int readLen = read(clientfd, targetDataPoint, targetSize - getSize);
        if (readLen > 0) {
            getSize += readLen;
            targetDataPoint += readLen;
        } else {
            LOGE("========read error ??????????????");
        }
    }

    jintArray nativeArray = env->NewIntArray(targetSize / 4);
    jint *native_array = env->GetIntArrayElements(nativeArray, NULL);
    int index = 0;
    for (int i = 0; i < targetSize;) {
        int r = targetData[i];
        int g = targetData[i + 1];
        int b = targetData[i + 2];
        int a = targetData[i + 3];
        //转argb
        native_array[index] = ((r << 16) | (g << 8) | (b) | (a << 24));
        index++;
        i = i + 4;
    }
    env->ReleaseIntArrayElements(nativeArray, native_array, 0);
    return nativeArray;
}
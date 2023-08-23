#define LOG_TAG "karl_service"
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#include <stdio.h>

uint32_t screen_width, screen_height, screen_stride;
size_t size = 0;
static  void *screen_base = NULL;

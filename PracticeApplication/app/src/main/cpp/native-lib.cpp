#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/bitmap.h>
#include <string.h>
#include <iostream>
#include <dirent.h>

#define TAG "fsg"

// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)



extern "C"
JNIEXPORT jstring

JNICALL
Java_com_example_admin_practiceapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_admin_practiceapplication_MainActivity_passBitmap(JNIEnv *env, jobject instance,
                                                                   jobject bitmap) {

    if(NULL==bitmap){
        LOGE("bitmap is null");
        return;
    }
    AndroidBitmapInfo info;
    int result=AndroidBitmap_getInfo(env,bitmap,&info);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("AndroidBitmap_getInfo failed, result: %d", result);
        return;
    }
    LOGD("bitmap width: %d, height: %d, format: %d, stride: %d", info.width, info.height,
         info.format, info.stride);
    unsigned char *addrPtr;
    result=AndroidBitmap_lockPixels(env,bitmap, reinterpret_cast<void **>(&addrPtr));
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("AndroidBitmap_lockPixels failed, result: %d", result);
        return;
    }
    // 执行图片操作的逻辑
    int length = info.stride * info.height;
    for (int i = 0; i < length; ++i) {
        LOGD("value: %x", addrPtr[i]);
    }
    // 像素信息不再使用后需要解除锁定
    result = AndroidBitmap_unlockPixels(env, bitmap);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("AndroidBitmap_unlockPixels failed, result: %d", result);
    }
}

void showAllFiles(std::string dir_name){
    DIR *dir=opendir(dir_name.c_str());
    if (NULL == dir) {
        LOGE("Can not open dir. Check path or permission!");
        return;
    }
    struct  dirent *file;
    while ((file=readdir(dir)) != NULL){
        if(strcmp(file->d_name,".") == 0||strcmp(file->d_name,"..") == 0){
            //LOGD("ignore. and ..");
            continue;
        }
        if(file->d_type ==DT_DIR){
            std::string filepath=dir_name+"/"+file->d_name;
            showAllFiles(filepath);
        } else if(strstr(file->d_name,"mp4")!=NULL){
            LOGI("filePath:%s/%s",dir_name.c_str(),file->d_name);
        } else{
            //LOGI("filePath:%s/%s",dir_name.c_str(),file->d_name);
        }
    }
    closedir(dir);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_admin_practiceapplication_MainActivity_showDir(JNIEnv *env, jobject instance,
                                                                jstring dirPath_) {
    const char *dirPath = env->GetStringUTFChars(dirPath_, 0);
    if(NULL==dirPath){
        LOGE("dir is null");
        return;
    }
    showAllFiles(std::string(dirPath));
    env->ReleaseStringUTFChars(dirPath_, dirPath);
}


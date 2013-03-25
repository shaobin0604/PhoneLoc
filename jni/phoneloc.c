/*
 * Phone Location of China
 *
 * Base on code from Geesun
 * Modified by cytown
 * Last edited 2009.11.13
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <assert.h>
#include <android/log.h>

#define DEBUG

#ifdef DEBUG
#define TAG "phonelocjni"
#endif

#define MAX_PHONE_LEN   48
#define MAX_PHONE_CN_LEN  100

typedef struct known_phone_info{
    char known_phone[MAX_PHONE_LEN];
    char known_phone_cn[MAX_PHONE_LEN];
} known_phone_info_t;

static known_phone_info_t g_known_phone[] = {
    {"13800138000","001,中国移动客服"},
    {"1008611","001,中国移动客服"},
};

static const  int KNOWN_PREFIX_LEN = 6;
static const char* KNOWN_PREFIX[] = {"0086", "106", "12520", "17951", "17909", "12593"};
static int exists = 0;

int file_exists(const char * filename) {
    if (exists != 0) return exists > 0 ? 0 : -1;
#ifdef DEBUG
     __android_log_print(ANDROID_LOG_DEBUG, TAG, "check data file: %s", filename);
#endif
    FILE * file;
    if (file = fopen(filename, "r")) {
        fclose(file);
        exists = 1;
        return 0;
    }
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_ERROR, TAG, "DATA FILE (%s) NOT EXIST!", filename);
#endif
    exists = -1;
    return exists;
}

int isInterPhone(char * phone, int len) {
    if (strncmp(phone, "00", 2) == 0) {
        return 0;
    }
    return -1;
}

void formatPhone(const char* phone, int len, char* nphone) {
    if (phone == NULL || nphone == NULL) {
        return;
    }
    // shouldn't length over 40!
    if (len > 40) len = 40;
    strncpy(nphone, phone, len);
    char* pch = strchr(nphone, '-');
    while (pch != NULL) {
        int pos = pch - nphone;
        memmove(nphone + pos, nphone + pos + 1, len - pos);
        pch = strchr(nphone, '-');
    }

    if (nphone[0] == '+') {
        if (strncmp(nphone, "+00", 3) != 0) {
            memmove(nphone + 2, nphone + 1, len);
            memmove(nphone, "00", 2);
        } else {
            memmove(nphone, nphone + 1, len);
        }
    }
    int i;
    for (i = 0; i < KNOWN_PREFIX_LEN; i++) {
        int l = strlen(KNOWN_PREFIX[i]);
        if (strncmp(nphone, KNOWN_PREFIX[i], l) == 0) {
            memmove(nphone, nphone+l, len);
            break;
        }
    }
    if (pch=strchr(nphone, '#')) {
        pch[0] = 0x00;
    }
    if (pch=strchr(nphone, '*')) {
        pch[0] = 0x00;
    }
#ifdef DEBUG
     __android_log_print(ANDROID_LOG_DEBUG, TAG, "after format: %s", nphone);
#endif
}

JNIEXPORT jstring JNICALL
getPhoneLocationJni( JNIEnv* env, jclass thiz, jstring phone, jstring jLocDataFilePath ) {
    const char* phone2 = NULL;
    const char* locDataFilePath = NULL;
    jboolean is_copy;
    phone2 = (*env)->GetStringUTFChars (env, phone, &is_copy);
    locDataFilePath = (*env)->GetStringUTFChars(env, jLocDataFilePath, NULL);
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "called [%s], locDataFilePath [%s]", phone2, locDataFilePath);
#endif
    if (phone2 == NULL) {
        return NULL;
    }
    int len = strlen(phone2);
    if (len < 3) {
        return NULL;
    }

    char nphone[48];
    memset(nphone, 0x00, sizeof(nphone));
    formatPhone(phone2, len, nphone);
    
    (*env)->ReleaseStringUTFChars(env, phone, phone2);
    
    len = strlen(nphone);
    if (len < 3) {
        return NULL;
    }

    char location[48];
    char locationCode[48];
    memset(locationCode,0x00,48);
    memset(location,0x00,48);

    if (isInterPhone(nphone, len) >= 0) {
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "inter phone[%s]", nphone);
#endif
        int pos = len > 6 ? 6 : len;
        char m[8];
        memset(m, 0x00, 8);
        int i;
        for (i = 0; i < 7-pos; i++) {
            m[i] = '9';
        }
        strncpy(m+7-pos, nphone, pos);
        for (; pos >= 3; pos--) {
            int num = atol(&m[0]);
            if (getLocationInfoEx(num, location, locationCode, locDataFilePath) >= 0) {
                (*env)->ReleaseStringUTFChars(env, jLocDataFilePath, locDataFilePath);
                return (*env)->NewStringUTF(env, locationCode);
            }
            memmove(m + 1, m, 6);
            m[7] = 0x00;
        }
        (*env)->ReleaseStringUTFChars(env, jLocDataFilePath, locDataFilePath);
        return NULL;
    }
    if (nphone[0] == '0') {
        if (nphone[1] == '1' || nphone[1] == '2') {
            nphone[3] = 0x00;
        } else if (len >= 4) {
            nphone[4] = 0x00;
        } else {
            (*env)->ReleaseStringUTFChars(env, jLocDataFilePath, locDataFilePath);
            return NULL;
        }
    } else {
        int i;
        for (i = 0; i <sizeof(g_known_phone) /sizeof(known_phone_info_t); i++) {
            int l = strlen(g_known_phone[i].known_phone);
            if (strncmp(nphone, g_known_phone[i].known_phone, l) == 0) {
                (*env)->ReleaseStringUTFChars(env, jLocDataFilePath, locDataFilePath);
                return (*env)->NewStringUTF(env, g_known_phone[i].known_phone_cn);
            }
        }
        if (len >= 7) {
            nphone[7] = 0x00;
        }
    }
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "find %s", nphone);
#endif
    int num = atol(nphone);
    if (getLocationInfoEx(num, location, locationCode, locDataFilePath) >= 0) {
        (*env)->ReleaseStringUTFChars(env, jLocDataFilePath, locDataFilePath);
        return (*env)->NewStringUTF(env, locationCode);
    }
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "return emptystr");
#endif
    (*env)->ReleaseStringUTFChars(env, jLocDataFilePath, locDataFilePath);
    return NULL;
}

int getLocationInfoEx(int num, char * location, char * locationCode, const char* locDataFilePath) {
    if (file_exists(locDataFilePath) < 0) {
#ifdef DEBUG
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "data file not exist!");
#endif
        return -1;
    }

    getLocationInfo(locDataFilePath, num, location, locationCode);
#ifdef DEBUG
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "return is %d, %s, %d, %s", strlen(location), location, strlen(locationCode), locationCode);
#endif
    if (location[0] == ' ' && location[1] == 0x00) return -1;
    strcat(locationCode, ",");
    strcat(locationCode, location);
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "[%d] == %s", num, locationCode);
#endif
    return 0;
}

/*
#ifdef DEBUG
    jstring sss = getPhoneLocationJni(env, thiz, "+88613501330000");
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "%s", sss[0]);
    sss = getPhoneLocationJni(env, thiz, "0085213501330-000");
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "%s", sss);
    sss = getPhoneLocationJni(env, thiz, "008613501330-000");
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "%s", sss);
    sss = getPhoneLocationJni(env, thiz, "+8613501330000");
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "%s", sss);
    sss = getPhoneLocationJni(env, thiz, "125-20-15-01037-0000#888");
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "%s", sss);
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "===============");
#endif
*/


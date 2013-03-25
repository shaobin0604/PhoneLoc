#ifndef _NATIVEHELPER_LOG_H
#define _NATIVEHELPER_LOG_H

#include <android/log.h>
#include <pthread.h>

#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

#define  SLOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  SLOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  SLOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  SLOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  SLOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#ifdef USE_THREAD_ID

#ifdef AACD_LOGLEVEL_VERBOSE
#define LOGV(fmt, args...)	SLOGV("[%u: %d: %s] _____ " fmt "\n", pthread_self(), __LINE__, __FUNCTION__, ##args)
#else
#define LOGV(fmt, args...) //
#endif

#ifdef AACD_LOGLEVEL_DEBUG
#define LOGD(fmt, args...)	SLOGD("[%u: %d: %s] _____ " fmt "\n", pthread_self(), __LINE__, __FUNCTION__, ##args)
#else
#define LOGD(fmt, args...) //
#endif

#ifdef AACD_LOGLEVEL_INFO
#define LOGI(fmt, args...)	SLOGI("[%u: %d: %s] _____ " fmt "\n", pthread_self(), __LINE__, __FUNCTION__, ##args)
#else
#define LOGI(fmt, args...) //
#endif

#ifdef AACD_LOGLEVEL_WARN
#define LOGW(fmt, args...)	SLOGW("[%u: %d: %s] _____ " fmt "\n", pthread_self(), __LINE__, __FUNCTION__, ##args)
#else
#define LOGW(fmt, args...) //
#endif

#ifdef AACD_LOGLEVEL_ERROR
#define LOGE(fmt, args...)	SLOGE("[%u: %d: %s] _____ " fmt "\n", pthread_self(), __LINE__, __FUNCTION__, ##args)
#else
#define LOGE(fmt, args...) //
#endif

#else

#ifdef AACD_LOGLEVEL_VERBOSE
#define LOGV(fmt, args...)	SLOGV("[%d: %s] _____ " fmt "\n", __LINE__, __FUNCTION__, ##args)
#else
#define LOGV(fmt, args...) //
#endif

#ifdef AACD_LOGLEVEL_DEBUG
#define LOGD(fmt, args...)	SLOGD("[%d: %s] _____ " fmt "\n", __LINE__, __FUNCTION__, ##args)
#else
#define LOGD(fmt, args...) //
#endif

#ifdef AACD_LOGLEVEL_INFO
#define LOGI(fmt, args...)	SLOGI("[%d: %s] _____ " fmt "\n", __LINE__, __FUNCTION__, ##args)
#else
#define LOGI(fmt, args...) //
#endif

#ifdef AACD_LOGLEVEL_WARN
#define LOGW(fmt, args...)	SLOGW("[%d: %s] _____ " fmt "\n", __LINE__, __FUNCTION__, ##args)
#else
#define LOGW(fmt, args...) //
#endif

#ifdef AACD_LOGLEVEL_ERROR
#define LOGE(fmt, args...)	SLOGE("[%d: %s] _____ " fmt "\n", __LINE__, __FUNCTION__, ##args)
#else
#define LOGE(fmt, args...) //
#endif

#endif /* USE_THREAD_ID */

#endif /* _NATIVEHELPER_LOG_H */

#ifndef MUTEX_H
#define MUTEX_H

// Platform
#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
    #define PLATFORM_WINDOWS 1
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    #include <pthread.h>
    #define PLATFORM_POSIX 1
#else
    #error "Unsupported platform"
#endif

// Platform-independent lock/unlock wrappers
static inline void Mutex_lock(void *lock) {
    #if PLATFORM_WINDOWS
        EnterCriticalSection((CRITICAL_SECTION*)lock);
    #elif PLATFORM_POSIX
        pthread_mutex_lock((pthread_mutex_t*)lock);
    #endif
}

static inline void Mutex_unlock(void *lock) {
    #if PLATFORM_WINDOWS
        LeaveCriticalSection((CRITICAL_SECTION*)lock);
    #elif PLATFORM_POSIX
        pthread_mutex_unlock((pthread_mutex_t*)lock);
    #endif
}

static inline void Mutex_init(void *lock) {
    #if PLATFORM_WINDOWS
        InitializeCriticalSection((CRITICAL_SECTION*)lock);
    #elif PLATFORM_POSIX
        pthread_mutex_init((pthread_mutex_t*)lock, NULL);
    #endif
}

static inline void Mutex_destroy(void *lock) {
    #if PLATFORM_WINDOWS
        DeleteCriticalSection((CRITICAL_SECTION*)lock);
    #elif PLATFORM_POSIX
        pthread_mutex_destroy((pthread_mutex_t*)lock);
    #endif
}

#endif // MUTEX_H
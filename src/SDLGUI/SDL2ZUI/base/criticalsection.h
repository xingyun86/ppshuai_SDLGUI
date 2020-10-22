
#ifndef BASE_CRITICALSECTION_H__
#define BASE_CRITICALSECTION_H__

#ifdef _MSC_VER
#include <windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#endif

//#ifdef _DEBUG
#define CS_TRACK_OWNER 1
//#endif  // _DEBUG

#if CS_TRACK_OWNER
#define TRACK_OWNER(x) x
#else  // !CS_TRACK_OWNER
#define TRACK_OWNER(x)
#endif  // !CS_TRACK_OWNER

namespace base {

#ifdef _MSC_VER
    class CriticalSection {
    public:
        CriticalSection() {
            InitializeCriticalSection(&crit_);
            // Windows docs say 0 is not a valid thread id
            TRACK_OWNER(thread_ = 0);
        }
        ~CriticalSection() {
            DeleteCriticalSection(&crit_);
        }
        void Enter() {
            EnterCriticalSection(&crit_);
            TRACK_OWNER(thread_ = GetCurrentThreadId());
        }
        void Leave() {
            TRACK_OWNER(thread_ = 0);
            LeaveCriticalSection(&crit_);
        }

//#if CS_TRACK_OWNER
        bool CurrentThreadIsOwner() const { return thread_ == GetCurrentThreadId(); }
//#endif  // CS_TRACK_OWNER

    private:
        CRITICAL_SECTION crit_;
        TRACK_OWNER(DWORD thread_);  // The section's owning thread id
    };
#else
    class CriticalSection {
    public:
        CriticalSection() {
            pthread_mutexattr_t mutex_attribute;
            pthread_mutexattr_settype(&mutex_attribute, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&mutex_, &mutex_attribute);
        }
        ~CriticalSection() {
            pthread_mutex_destroy(&mutex_);
        }
        void Enter() {
            pthread_mutex_lock(&mutex_); 
            TRACK_OWNER(thread_ = gettid());
        }
        void Leave() {
            TRACK_OWNER(thread_ = 0);
            pthread_mutex_unlock(&mutex_);
        }
//#if CS_TRACK_OWNER
        bool CurrentThreadIsOwner() const { return thread_ == gettid(); }
//#endif  // CS_TRACK_OWNER

    private:
        pthread_mutex_t mutex_;
        TRACK_OWNER(pid_t thread_);  // The section's owning thread id
    };
#endif // WIN32

    // CritScope, for serializing exection through a scope

    class CritScope {
    public:
        CritScope(CriticalSection* pcrit) {
            pcrit_ = pcrit;
            pcrit_->Enter();
        }
        ~CritScope() {
            pcrit_->Leave();
        }
    private:
        CriticalSection* pcrit_;
    };

} // namespace base

#endif // BASE_CRITICALSECTION_H__

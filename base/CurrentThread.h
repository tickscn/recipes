#ifndef DAINA_CURRENTTHREAD_H
#define DAINA_CURRENTTHREAD_H
#include "Types.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

namespace daina {
namespace CurrentThread {
    extern __thread int cachedTid;
    extern __thread char tidString[32];   //< for logging
    extern __thread int tidStringLength;  //< for logging
    extern __thread const char* threadName;
    void cacheTid();

    inline int tid()
    {
#if __has_builtin(__builtin_expect)
        if (__builtin_expect(cachedTid == 0, 0))
#else
        if (cachedTid == 0)
#endif
        {
            cacheTid();
        }
        return cachedTid;
    }
    inline const char* getTidString()
    {
        return tidString;
    }
    inline const int getTidStringLength()
    {
        return tidStringLength;
    }
    bool isMastrThread();
    string stackTrace(bool demangle);
}  // namespace CurrentThread
}  // namespace daina

#endif  // DAINA_CURRENTTHREAD_H

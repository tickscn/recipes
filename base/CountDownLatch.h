#ifndef DAINA_COUNTDOWNLATCH_H
#define DAINA_COUNTDOWNLATCH_H
#include "noncopyable.h"
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace daina {
class CountDownLatch : noncopyable {
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
    int getCount() const;

private:
    mutable std::mutex _mutex;
    std::condition_variable _cv;
    int _count;
};
}  // namespace daina
#endif  // DAINA_COUNTDOWNLATCH

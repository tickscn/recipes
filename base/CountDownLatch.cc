#include "CountDownLatch.h"

using namespace daina;

CountDownLatch::CountDownLatch(int count) : _count(count) {}

void CountDownLatch::wait()
{
    std::unique_lock<std::mutex> lk(_mutex);
    if (_count > 0)
    {
        _cv.wait(lk);
    }
}

void CountDownLatch::countDown()
{
    std::unique_lock<std::mutex> lk(_mutex);
    --_count;
    if (_count <= 0)
    {
        _cv.notify_all();
    }
}

int CountDownLatch::getCount() const
{
    std::lock_guard<std::mutex> lk(_mutex);
    return _count;
}

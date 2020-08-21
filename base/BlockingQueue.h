#ifndef DAINA_BLOCKINGQUEUE_H
#define DAINA_BLOCKINGQUEUE_H
#include "noncopyable.h"
#include <cassert>
#include <condition_variable>
#include <deque>
#include <mutex>
namespace daina {
template <typename T>
class BlockingQueue : noncopyable {
public:
    BlockingQueue() = default;
    void put(T&& x)
    {
        std::lock_guard<std::mutex> lk(_mutex);
        _queue.push_back(std::move(x));
        _notEmpty.notify_one();
    }

    void put(const T& x)
    {
        std::lock_guard<std::mutex> lk(_mutex);
        _notEmpty.notify_one();
    }

    T take()
    {
        std::unique_lock<std::mutex> lk(_mutex);
        while (_queue.empty())
        {
            _notEmpty.wait(lk);
        }
        assert(!_queue.empty());
        T ret(std::move(_queue.front()));
        _queue.pop_front();
        return ret;
    }
    size_t size() const
    {
        std::lock_guard<std::mutex> lk(_mutex);
        return _queue.size();
    }

private:
    mutable std::mutex _mutex;
    std::condition_variable _notEmpty;
    std::deque<T> _queue;
};
}  // namespace daina
#endif  // DAINA_BLOCKINGQUEUE_H

#include "base/BlockingQueue.h"
#include "base/CountDownLatch.h"
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

class Test {
public:
    Test(int numThreads) : _latch(numThreads)
    {
        for (int i = 0; i < numThreads; ++i)
        {
            _threads.emplace_back(new std::thread(std::bind(&Test::threadFunc, this)));
        }
    }

    void run(int times)
    {
        printf("waiting for count down latch\n");
        _latch.wait();
        printf("all threads started\n");
        for (int i = 0; i < times; ++i)
        {
            char buf[32];
            snprintf(buf, sizeof buf, "hello %d", i);
            _queue.put(buf);
            printf(" put data = %s, size = %zd\n", buf, _queue.size());
        }
    }

    void joinAll()
    {
        for (size_t i = 0; i < _threads.size(); ++i)
        {
            _queue.put("stop");
        }

        for (auto& thr : _threads)
        {
            thr->join();
        }
    }

private:
    void threadFunc()
    {
        _latch.countDown();
        bool running = true;
        while (running)
        {
            std::string d(_queue.take());
            printf(" Get Date = %s, rest size = %zd\n", d.c_str(), _queue.size());
            running = (d != "stop");
        }
    }

private:
    daina::BlockingQueue<std::string> _queue;
    daina::CountDownLatch _latch;
    std::vector<std::unique_ptr<std::thread>> _threads;
};
void testMove()
{
    daina::BlockingQueue<std::unique_ptr<int>> queue;
    queue.put(std::unique_ptr<int>(new int(42)));
    std::unique_ptr<int> x = queue.take();
    printf("took %d\n", *x);
    *x = 123;
    queue.put(std::move(x));
    std::unique_ptr<int> y = queue.take();
    printf("took %d\n", *y);
}

int main()
{
    printf("pid=%d\n", ::getpid());
    Test t(2);
    t.run(100);
    t.joinAll();
    testMove();
    return 0;
}

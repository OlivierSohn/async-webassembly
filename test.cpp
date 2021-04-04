#include <stdio.h>

#include <iostream>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <stack>
#include <future>
#include <vector>

#define USE_ASYNC 1

#if USE_ASYNC
struct Asyncs {
    using F = std::function<void(void)>;

    Asyncs(int) {}

    ~Asyncs() {
        for (auto & f : fut)
            f.wait();
    }

    void push(F f) {
        fut.push_back(std::async(f));
    }
private:
    std::vector<std::future<void>> fut;
};
using AsyncTasks = Asyncs;

#else // USE_ASYNC

struct Threads {
    using F = std::function<void(void)>;
    
    Threads(int nThreads) {
        for (int i=0; i<nThreads; ++i){
            workers.emplace_back([this](){ work(); });
        }
    }

    ~Threads() {
        for (auto & t : workers)
            push({});
        for (auto & t : workers)
            t.join();
    }

    void push(F f) {
        std::lock_guard<std::mutex> l(m);
        s.push(f);
    }
private:
    std::stack<F> s;
    mutable std::mutex m;
    std::vector<std::thread> workers;
    
    void work() {
        bool yield = false;
        for(;;) {
            if (yield) {
                std::this_thread::yield();
                yield = false;
            }
            F f;
            {
                std::lock_guard<std::mutex> l(m);
                if (s.empty())
                {
                    yield = true;
                    continue;
                }
                f = s.top();
                s.pop();
            }
            if (!f)
                return; // sentinel
            f();
        }
    }
};

using AsyncTasks = Threads;

#endif // USE_ASYNC

// This function makes sure that 'i' tasks are running concurrently,
// then returns
void concurrentTasks(int i)
{
    {
        AsyncTasks tasks(i);
        
        std::atomic_int count(0);
        std::cout << i << "..." << std::endl;
        for (int j=0; j<i; ++j) {
            tasks.push([&count, i](){
                ++count;
                while(count < i) {
                    std::this_thread::yield();
                }
            });
        }
    }
    std::cout << i << std::endl;
}

int main( int argc, const char *argv[] ) {
    std::cout << std::thread::hardware_concurrency() << " concurrency" << std::endl;

    for (int i=1; i<2*std::thread::hardware_concurrency(); ++i)
    {
        for (int k=0; k<2; ++k)
        {
            concurrentTasks(i);
        }
    }
    std::cout << "done" << std::endl;
    return 0;
}

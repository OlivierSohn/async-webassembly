#include <stdio.h>

#include <iostream>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <stack>
#include <future>
#include <vector>

// To test with std::thread, write 0 here:
#define USE_ASYNC 1

#if USE_ASYNC
struct Asyncs {
    using F = std::function<void(void)>;

    std::string getName() const { return "std::async"; }

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

template<bool detach>
struct Threads {
    using F = std::function<void(void)>;

    std::string getName() const {
        return std::to_string(workers.size()) +
        (detach? " detached" : "") +
        " std::thread";
    }

    Threads(int nThreads) {
        {
            std::lock_guard l(m);
            running.resize(nThreads, true);
        }
        for (int i=0; i<nThreads; ++i){
            workers.emplace_back([this, i](){ work(i); });
            if constexpr (detach)
                workers.back().detach();
        }
    }

    ~Threads() {
        for (auto & t : workers)
            push({});
        if constexpr (detach)
        {
            for (int i=0, sz=running.size(); i<sz; ++i)
            {
            retry:
                {
                    std::lock_guard<std::mutex> l(m);
                    if (!running[i])
                        continue;
                }
                std::this_thread::yield();
                goto retry;
            }
        }
        else
        {
            for (auto & t : workers)
                t.join();
        }
    }

    void push(F f) {
        std::lock_guard<std::mutex> l(m);
        s.push(f);
    }
private:
    std::stack<F> s;
    mutable std::mutex m;
    std::vector<std::thread> workers;
    std::vector<bool> running;
    
    void work(int i) {
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
                break; // sentinel
            f();
        }
        {
            std::lock_guard<std::mutex> l(m);
            running[i] = false;
        }
    }
};

// if you want to run detached threads, use 'true' here:
using AsyncTasks = Threads<false>;

#endif // USE_ASYNC

// This function makes sure that 'i' tasks are able to run _concurrently_,
// then returns
bool concurrentTasks(int i)
{
    std::atomic_int count_breaks(0);
    {
        AsyncTasks tasks(i);
        std::cout << "using " << tasks.getName() << std::endl;
        std::atomic_int count(0);
        std::cout << i << "..." << std::endl;
        for (int j=0; j<i; ++j) {
            tasks.push([&count, &count_breaks, i](){
                ++count;
                auto const start = std::chrono::high_resolution_clock::now();
                while(count < i) {
                    std::this_thread::yield();

                    // To avoid deadlocks, break after one second
                    // (deadlocks occur with std::thread, but not with std::async)
                    auto const now = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
                    if (duration.count() > 1000) {
                        ++count_breaks;
                        break;
                    }
                }
            });
        }
    }
    std::cout << i << " with " << count_breaks.load() << " breaks" << std::endl;
    return count_breaks == 0;
}

int main( int argc, const char *argv[] ) {
    std::cout << std::thread::hardware_concurrency() << " concurrency" << std::endl;

    for (int i=1; i<2*std::thread::hardware_concurrency(); ++i)
    {
        for (int k=0; k<2; ++k)
        {
            if (!concurrentTasks(i))
                std::cout << "Failed : could not have " << i << " tasks running at the same time." << std::endl;
        }
    }
    std::cout << "done" << std::endl;
    throw std::logic_error("lo");
    return 0;
}

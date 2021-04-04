### What is it?

I wrote this program to understand how I can to run parallel code using WebAsssembly / emscriptem.

The program runs an increasing number of tasks in parallel,
using std::async by default, or using std::thread if you change the relevant define : `#define USE_ASYNC 0`.

### Conclusions

After doing some tests in Google Chrome my conclusions are that:
- parallelism can be achieved using std::async, but not using explicit std::thread(s) because it results in deadlocks, nomatter how big the initial pool size is.
- if we run many std::async in parallel such that the thread pool needs to grow, it will grow, but unrecoverable exceptions will be thrown if there is not enough memory and the memory is not allowed to grow.

### Examples of command lines parameters used to compile

`./emcc ../../../emscriptem_async/test.cpp -s WASM=1 -s NO_DISABLE_EXCEPTION_CATCHING -s EXCEPTION_DEBUG -s USE_PTHREADS -s PROXY_TO_PTHREAD=1 -s ALLOW_MEMORY_GROWTH=1 -s PTHREADS_DEBUG=1 -std=c++11 -o test.html`

or, for faster creation of threads:

`./emcc ../../../emscriptem_async/test.cpp -s WASM=1 -s NO_DISABLE_EXCEPTION_CATCHING -s EXCEPTION_DEBUG -s USE_PTHREADS -s INITIAL_MEMORY=167772160 -s PTHREAD_POOL_SIZE=30 -s PTHREAD_POOL_DELAY_LOAD=1 -s PROXY_TO_PTHREAD=1 -s PTHREADS_DEBUG=1 -std=c++11 -o test.html`

Note that we could combine `-s ALLOW_MEMORY_GROWTH=1` and `INITIAL_MEMORY=167772160` but I've observed that growing the memory is really slow, so I prefered to forbid it.

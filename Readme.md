### What is it?

I wrote this program to understand how I can to run parallel code using WebAsssembly / emscriptem.

The program runs an increasing number of tasks in parallel, and makes sure that these tasks are actually running _at the same time_.

We can use std::async (by default), or std::thread by changing the define : `#define USE_ASYNC 0`.

### Conclusions

After doing some tests in Google Chrome my conclusions are that:
- We can run tasks _at the same time_ using std::async, but not using explicit std::thread(s). With std::threads, we can start the threads but they won't run _at the same time_, nomatter how big the initial pool size is.
  - See the Annex below for logs.
- If we run many std::async in parallel such that the thread pool needs to grow, it will grow, but unrecoverable exceptions will be thrown if there is not enough memory and the memory is not allowed to grow.

### Examples of command lines parameters used to compile

`./emcc ../../../emscriptem_async/test.cpp -s WASM=1 -s NO_DISABLE_EXCEPTION_CATCHING -s EXCEPTION_DEBUG -s USE_PTHREADS -s PROXY_TO_PTHREAD=1 -s ALLOW_MEMORY_GROWTH=1 -s PTHREADS_DEBUG=1 -std=c++11 -o test.html`

or, for faster creation of threads:

`./emcc ../../../emscriptem_async/test.cpp -s WASM=1 -s NO_DISABLE_EXCEPTION_CATCHING -s EXCEPTION_DEBUG -s USE_PTHREADS -s INITIAL_MEMORY=167772160 -s PTHREAD_POOL_SIZE=30 -s PTHREAD_POOL_DELAY_LOAD=1 -s PROXY_TO_PTHREAD=1 -s PTHREADS_DEBUG=1 -std=c++11 -o test.html`

Note that we could combine `-s ALLOW_MEMORY_GROWTH=1` and `INITIAL_MEMORY=167772160` but I've observed that growing the memory is really slow, so I prefered to forbid it.

### Annex

Output using std::async. All tasks were running _at the same time_.

```
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
12 concurrency
using std::async
1...
1 with 0 breaks
using std::async
1...
1 with 0 breaks
using std::async
2...
2 with 0 breaks
using std::async
2...
2 with 0 breaks
using std::async
3...
3 with 0 breaks
using std::async
3...
3 with 0 breaks
using std::async
4...
4 with 0 breaks
using std::async
4...
4 with 0 breaks
using std::async
5...
5 with 0 breaks
using std::async
5...
5 with 0 breaks
using std::async
6...
6 with 0 breaks
using std::async
6...
6 with 0 breaks
using std::async
7...
7 with 0 breaks
using std::async
7...
7 with 0 breaks
using std::async
8...
8 with 0 breaks
using std::async
8...
8 with 0 breaks
using std::async
9...
9 with 0 breaks
using std::async
9...
9 with 0 breaks
using std::async
10...
10 with 0 breaks
using std::async
10...
10 with 0 breaks
using std::async
11...
11 with 0 breaks
using std::async
11...
11 with 0 breaks
using std::async
12...
12 with 0 breaks
using std::async
12...
12 with 0 breaks
using std::async
13...
13 with 0 breaks
using std::async
13...
13 with 0 breaks
using std::async
14...
14 with 0 breaks
using std::async
14...
14 with 0 breaks
using std::async
15...
15 with 0 breaks
using std::async
15...
15 with 0 breaks
using std::async
16...
16 with 0 breaks
using std::async
16...
16 with 0 breaks
using std::async
17...
17 with 0 breaks
using std::async
17...
17 with 0 breaks
using std::async
18...
18 with 0 breaks
using std::async
18...
18 with 0 breaks
using std::async
19...
19 with 0 breaks
using std::async
19...
19 with 0 breaks
using std::async
20...
20 with 0 breaks
using std::async
20...
20 with 0 breaks
using std::async
21...
21 with 0 breaks
using std::async
21...
21 with 0 breaks
using std::async
22...
22 with 0 breaks
using std::async
22...
22 with 0 breaks
using std::async
23...
23 with 0 breaks
using std::async
23...
23 with 0 breaks
done
```

Output, using std::thread.
Some lines start with "Failed :" and indicate that the threads have been instantiated but could not get the tasks to run all _at the same time_.

```
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
Allocating a new web worker from http://127.0.0.1:8887/emsdk/upstream/emscripten/test.worker.js
12 concurrency
using 1 std::thread
1...
1 with 0 breaks
using 1 std::thread
1...
1 with 0 breaks
using 2 std::thread
2...
2 with 0 breaks
using 2 std::thread
2...
2 with 1 breaks
Failed : could not have 2 tasks running at the same time.
using 3 std::thread
3...
3 with 0 breaks
using 3 std::thread
3...
3 with 2 breaks
Failed : could not have 3 tasks running at the same time.
using 4 std::thread
4...
4 with 1 breaks
Failed : could not have 4 tasks running at the same time.
using 4 std::thread
4...
4 with 2 breaks
Failed : could not have 4 tasks running at the same time.
using 5 std::thread
5...
5 with 3 breaks
Failed : could not have 5 tasks running at the same time.
using 5 std::thread
5...
5 with 4 breaks
Failed : could not have 5 tasks running at the same time.
using 6 std::thread
6...
6 with 5 breaks
Failed : could not have 6 tasks running at the same time.
using 6 std::thread
6...
6 with 0 breaks
using 7 std::thread
7...
7 with 6 breaks
Failed : could not have 7 tasks running at the same time.
using 7 std::thread
7...
7 with 5 breaks
Failed : could not have 7 tasks running at the same time.
using 8 std::thread
8...
8 with 7 breaks
Failed : could not have 8 tasks running at the same time.
using 8 std::thread
8...
8 with 6 breaks
Failed : could not have 8 tasks running at the same time.
using 9 std::thread
9...
9 with 4 breaks
Failed : could not have 9 tasks running at the same time.
using 9 std::thread
9...
9 with 6 breaks
Failed : could not have 9 tasks running at the same time.
using 10 std::thread
10...
10 with 6 breaks
Failed : could not have 10 tasks running at the same time.
using 10 std::thread
10...
10 with 2 breaks
Failed : could not have 10 tasks running at the same time.
using 11 std::thread
11...
11 with 6 breaks
Failed : could not have 11 tasks running at the same time.
using 11 std::thread
11...
11 with 5 breaks
Failed : could not have 11 tasks running at the same time.
using 12 std::thread
12...
12 with 5 breaks
Failed : could not have 12 tasks running at the same time.
using 12 std::thread
12...
12 with 5 breaks
Failed : could not have 12 tasks running at the same time.
using 13 std::thread
13...
13 with 7 breaks
Failed : could not have 13 tasks running at the same time.
using 13 std::thread
13...
13 with 5 breaks
Failed : could not have 13 tasks running at the same time.
using 14 std::thread
14...
14 with 5 breaks
Failed : could not have 14 tasks running at the same time.
using 14 std::thread
14...
14 with 13 breaks
Failed : could not have 14 tasks running at the same time.
using 15 std::thread
15...
15 with 8 breaks
Failed : could not have 15 tasks running at the same time.
using 15 std::thread
15...
15 with 6 breaks
Failed : could not have 15 tasks running at the same time.
using 16 std::thread
16...
16 with 7 breaks
Failed : could not have 16 tasks running at the same time.
using 16 std::thread
16...
16 with 5 breaks
Failed : could not have 16 tasks running at the same time.
using 17 std::thread
17...
17 with 6 breaks
Failed : could not have 17 tasks running at the same time.
using 17 std::thread
17...
17 with 5 breaks
Failed : could not have 17 tasks running at the same time.
using 18 std::thread
18...
18 with 6 breaks
Failed : could not have 18 tasks running at the same time.
using 18 std::thread
18...
18 with 6 breaks
Failed : could not have 18 tasks running at the same time.
using 19 std::thread
19...
19 with 6 breaks
Failed : could not have 19 tasks running at the same time.
using 19 std::thread
19...
19 with 5 breaks
Failed : could not have 19 tasks running at the same time.
using 20 std::thread
20...
20 with 6 breaks
Failed : could not have 20 tasks running at the same time.
using 20 std::thread
20...
20 with 4 breaks
Failed : could not have 20 tasks running at the same time.
using 21 std::thread
21...
21 with 5 breaks
Failed : could not have 21 tasks running at the same time.
using 21 std::thread
21...
21 with 7 breaks
Failed : could not have 21 tasks running at the same time.
using 22 std::thread
22...
22 with 6 breaks
Failed : could not have 22 tasks running at the same time.
using 22 std::thread
22...
22 with 7 breaks
Failed : could not have 22 tasks running at the same time.
using 23 std::thread
23...
23 with 7 breaks
Failed : could not have 23 tasks running at the same time.
using 23 std::thread
23...
23 with 6 breaks
Failed : could not have 23 tasks running at the same time.
done
```

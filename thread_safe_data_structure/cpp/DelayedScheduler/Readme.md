## Delayed Scheduler

This implementation is to learn/practice multithreading from the excellent code of [ThreadPool by progschj](https://github.com/progschj/ThreadPool), and [Scheduler by Bosma](https://github.com/Bosma/Scheduler).


#### Some Implementation Details

ThreadPool

* A plain queue is maintained. Each enqueued task is wrapped as an asynchronous task and then pushed to the queue.
* For synchronization, a mutex and a condition_variable are used to execute the queue of tasks orderly.

Scheduler

* A multimap<Time, Task> is maintained to keep track of the task queues.
* For synchronization, a mutex is maintained to execute the task orderly.
* Task "In some time" is implemented by condition_variable.wait_for()
* Task "At some time" is implemented by condition_variable.wait_until()
* Support "cron" format and "every some time" as well.


#### Example
File Scheduler_demo.cpp



<!-- #### Tutorial -->


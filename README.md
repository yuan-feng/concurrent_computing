## Multithreading

My Thread-safe Data-Structure 


#### [BusyWaitLock](./cpp/BusyWaitLock)
Implementation and Test of TAS lock and TTAS lock using a C++11 atomic variable.

* TTAS 
* TAS

In most cases, TTAS is faster than TAS.


#### [Scheduler](./cpp/DelayedScheduler)

A multimap <Time, Task\> is maintained to keep track of the task queues.

A threadpool is used execute the tasks.

#### [MPMCQueue](./cpp/MPMCQueue)

Lock-free Multiple-producer multiple-consumer queue.

* Use atomic variables as the ticket-lock for fairness.
* Use padding to prevent false sharing.

#### [ReaderWriterLock](./cpp/ReaderWriterLock)

C++14 supports shared_timed_mutex directly. This is a demo.

#### [Singleton](./cpp/Singleton)

A very simple C++11 simple singleton.

#### [SingletonLog](./cpp/SingletonLog)

Thread-safe customizable multi-level singleton log

Build status for cpp/SingletonLog:

 [![Build Status](https://travis-ci.com/yuan-feng/multithreading.svg?branch=master)](https://travis-ci.com/yuan-feng/multithreading)


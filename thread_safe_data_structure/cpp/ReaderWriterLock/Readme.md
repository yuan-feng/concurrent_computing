## Reader-Writer-Locks

Reader-Writer-Lock is a classical problem in multithreading.

* Only one writer can modify the data.
* Multiple readers can read the data.

One traditional solution is to use a semaphore to control the number of readers.

Now with C++ 14 shared_timed_mutex, a cleaner solution is available.

[Why it is called shared_timed_mutex instead of shared_mutex?](https://stackoverflow.com/questions/40207171/why-shared-timed-mutex-is-defined-in-c14-but-shared-mutex-in-c17)



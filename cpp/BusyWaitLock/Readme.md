### TAS lock and TTAS lock


This is an implementation of 

* Test-And-Set lock
* Test-Test-And-Set lock

using a C++11 atomic variable.

In most cases, TTAS is faster than TAS because TTAS "wait until lock looks free".

Specifically, TTAS is faster because TTAS:

* Spin on lock cache
* No bus use while lock is busy


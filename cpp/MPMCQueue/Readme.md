## Multiple-producer multiple-consumer queue.

This implementation is to learn/practice multithreading from the excellent code of [rigtorp](https://github.com/rigtorp/MPMCQueue).

It is Lock-free, or actually lower-level Bus-lock on the atomic variables.

#### Some Implementation Details

* Use atomic variables as the ticket-lock for fairness.
* For each element, use padding to avoid the data on the same cache line, which prevents false sharing.
* Similarly, for the head and tail, use even/odd number padding to prevent false sharing.
* Bounded blocking queue. It blocks when pop on empty or push to full.



#### Example
File MPMCQueue_demo.cpp

```
#include "MPMCQueue.hpp"
#include <iostream>
#include <thread>

using namespace PACKAGE_NAME;

int main(int argc, char *argv[]) {

  MPMCQueue<int> q(42);
  
  auto threadA = std::thread([&] {
    int v;
    q.pop(v);
    std::cout << "threadA " << v << std::endl ;
  });

  auto threadB = std::thread([&] {
    int v;
    q.pop(v);
    std::cout << "threadB " << v << std::endl ;
  });
  
  q.push(1);
  q.push(2);
  
  threadA.join();
  threadB.join();

  return 0;
}

```



#### Tutorial


Reference : Ticket Lock
[https://www.youtube.com/watch?v=r8eNGLY26T0](https://www.youtube.com/watch?v=r8eNGLY26T0)

Reference : False Sharing
[https://www.youtube.com/watch?v=dznxqe1Uk3E](https://www.youtube.com/watch?v=dznxqe1Uk3E)

Reference : MPMCQueue.h
[https://github.com/rigtorp/MPMCQueue](https://github.com/rigtorp/MPMCQueue)




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


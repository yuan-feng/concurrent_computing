#include <future>
#include <iostream>
#include <cassert>
#include "TasSpinLock.hpp"
#include "TTasSpinLock.hpp"

constexpr int nOps = 1'000'000 ; 

volatile int value = 0;

PACKAGE_NAME:: TasSpinLock tas ;
PACKAGE_NAME::TTasSpinLock ttas ;

int tasLoop(bool inc, int limit) {
  for (int i = 0; i < limit; ++i) {
    tas.lock();
    inc ? ++value : --value ; 
    tas.unlock();
  }
  return 0;
}

int ttasLoop(bool inc, int limit) {
  for (int i = 0; i < limit; ++i) {
    ttas.lock();
    inc ? ++value : --value ; 
    ttas.unlock();
  }
  return 0;
}



std::chrono::duration<double> getTasTime(){
  auto begin = std::chrono::system_clock::now();
  auto f1 = std::async(std::launch::async, std::bind(tasLoop, true, nOps));
  auto f2 = std::async(std::launch::async, std::bind(tasLoop, false, nOps));
  auto f3 = std::async(std::launch::async, std::bind(tasLoop, true, nOps));
  auto f4 = std::async(std::launch::async, std::bind(tasLoop, false, nOps));
  f1.wait();
  f2.wait();
  f3.wait();
  f4.wait();
  return std::chrono::system_clock::now() - begin ; 
}


std::chrono::duration<double> getTtasTime(){
  auto begin = std::chrono::system_clock::now();
  auto f1 = std::async(std::launch::async, std::bind(ttasLoop, true, nOps));
  auto f2 = std::async(std::launch::async, std::bind(ttasLoop, false, nOps));
  auto f3 = std::async(std::launch::async, std::bind(ttasLoop, true, nOps));
  auto f4 = std::async(std::launch::async, std::bind(ttasLoop, false, nOps));
  f1.wait();
  f2.wait();
  f3.wait();
  f4.wait();
  return std::chrono::system_clock::now() - begin ; 
}




int main() {

  auto tasTime = getTasTime();
  assert(value == 0 ) ; 

  auto ttasTime = getTtasTime();
  assert(value == 0 ) ; 

  std::cout << "TAS time = " << tasTime.count()  << std::endl;
  std::cout << "TTAS time = " << ttasTime.count()  << std::endl;
}


CXX = clang++ 

CXXFLAGS = -pthread -Wall -std=c++14 -pedantic

Files = InterruptableSleep.hpp Scheduler.hpp ThreadPool.h


demo: $(Files) Scheduler_demo.cpp
	$(CXX) -o scheduler_demo.exe Scheduler_demo.cpp $(CXXFLAGS) -O3 

run_demo:
	./scheduler_demo.exe

clean:
	rm -f scheduler_demo.exe

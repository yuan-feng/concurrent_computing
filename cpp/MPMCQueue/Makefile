
CXX = clang++ 

CXXFLAGS = -pthread -Wall -std=c++14

Files = MPMCQueue.hpp 


demo: $(Files) MPMCQueue_demo.cpp
	$(CXX) -o MPMCQueue_demo.exe MPMCQueue_demo.cpp $(CXXFLAGS) -O2 

test: $(Files)
	$(CXX) -o MPMCQueue_test.exe MPMCQueue_test.cpp $(CXXFLAGS) -O2 

	


run_demo:
	./MPMCQueue_demo.exe

run_test:
	./MPMCQueue_test.exe

clean:
	rm -f MPMCQueue_demo.exe
	rm -f MPMCQueue_test.exe

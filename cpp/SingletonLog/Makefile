
CXX = clang++ 

CXXFLAGS = -pthread -Wall -std=c++14

Files = Log_test.cpp Log.hpp LogLevels.hpp


demo: $(Files) Log_demo.cpp
	$(CXX) -o Log_demo.exe Log_demo.cpp $(CXXFLAGS) -O3 

test: $(Files)
	$(CXX) -o Log_test.exe Log_test.cpp $(CXXFLAGS) -O3 

	


run_demo:
	./Log_demo.exe

run_test:
	./Log_test.exe

clean:
	rm -f Log_demo.exe
	rm -f Log_test.exe

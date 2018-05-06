
CXX = clang++ 

CXXFLAGS = -pthread -Wall -std=c++14 -pedantic

Files = TTasSpinLock.hpp TasSpinLock.hpp

example: $(Files) 
	$(CXX) -o example.exe example.cpp $(CXXFLAGS) -O3 



run_example:
	./example.exe



clean:
	rm -f example.exe

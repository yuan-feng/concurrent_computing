
CXX = clang++ 

CXXFLAGS = -pthread -Wall -std=c++14

Files = Singleton_test.cpp Singleton.cpp 

O3: $(Files)
	$(CXX) -o testSingleton.exe Singleton_test.cpp Singleton.cpp $(CXXFLAGS) -O3 

O0: $(Files)
	$(CXX) -o testSingleton.exe Singleton_test.cpp Singleton.cpp $(CXXFLAGS) -O0 
	


run:
	./testSingleton.exe

clean:
	rm -f testSingleton.exe

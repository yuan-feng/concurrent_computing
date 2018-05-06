
CXX = clang++ 

CXXFLAGS = -pthread -Wall -std=c++14 -pedantic

Files = readerWriterLock.cpp 


demo: $(Files) 
	$(CXX) -o readerWriterLock.exe readerWriterLock.cpp $(CXXFLAGS) -O3 


	


run_demo:
	./readerWriterLock.exe


clean:
	rm -f readerWriterLock.exe

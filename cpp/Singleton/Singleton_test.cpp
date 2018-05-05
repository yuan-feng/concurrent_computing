#include <chrono>
#include <iostream>
#include <future>
#include <vector>

#include "Singleton.cpp" 

constexpr auto oneBillion = 1'000'000'000 ; 

std::chrono::duration<double> getTime(){
	auto begin = std::chrono::system_clock::now();
	for (size_t i = 0; i < oneBillion; ++i)	{
		Singleton::getInstance();
	}
	return std::chrono::system_clock::now() - begin ; 
}



int main(int argc, char const *argv[])
{
	auto future1 = std::async(std::launch::async, getTime);
	auto future2 = std::async(std::launch::async, getTime);
	auto future3 = std::async(std::launch::async, getTime);
	auto future4 = std::async(std::launch::async, getTime);

	auto total = future1.get() + 
				 future2.get() + 
				 future3.get() + 
				 future4.get() ; 

	std::cout << total.count() << std::endl; 

	return 0;
}
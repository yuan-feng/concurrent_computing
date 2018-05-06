
#include "MPMCQueue.hpp"

#include <vector>
#include <iostream>

#include <cassert>
// #include <chrono>
// #include <set>
#include <thread>

using PACKAGE_NAME::MPMCQueue ;
using std::vector ;
using std::cout ;
using std::endl ;
using std::cerr ;

int main(int argc, char const *argv[])
{
	cerr << " Starting test case... " << endl;
	{
		MPMCQueue<float> q(1.f) ; 
		float val = 0.f ;
		assert( q.try_push(1.f) == true );
		assert( q.try_push(2.f) == false );
		assert( q.try_pop(val) == true && val == 1 ) ;
		assert( q.try_pop(val) == false && val == 1 ) ;
	}
	cerr << " Passed the basic test case. " << endl;


	// {
	// 	MPMCQueue< vector<float> > q(32) ; 
	// 	vector<float> vec{1.f,2.f} ;
	// 	// lvalue
	// 	q.emplace(vec);
	// 	q.try_emplace(vec);
	// 	q.push(vec);
	// 	q.try_push(vec);
	// 	// xvalue 
	// 	q.push({});
	// 	q.try_push({});
	// }
	// cerr << " Passed the vector case. " << endl;



	{
		MPMCQueue< std::unique_ptr<int> > q(32) ; 
		q.emplace(std::unique_ptr<int>(new int(1)));
		q.try_emplace(std::unique_ptr<int>(new int(1)));
		q.push(std::unique_ptr<int>(new int(1)));
		q.try_push(std::unique_ptr<int>(new int(1)));
	}
	cerr << " Passed the movable element case. " << endl;

	{
		bool throws = false; 
		try{
			MPMCQueue<int> q(0);
		} catch (std::exception & e){
			throws = true ;
		}
		assert(throws == true );
	}
	cerr << " Passed the capacity-check case. " << endl;	

	{
		const uint64_t numOps = 1000 ;
		const int numThreads = 10 ; 
		MPMCQueue<int> q(numThreads);
		std::atomic<bool> flag(false);
		std::vector<std::thread> threads ;
		std::atomic<uint64_t> sum(0) ; 
		for (int i = 0; i < numThreads; ++i){
			threads.push_back(std::thread([&,i]{
				while (!flag )
					;
				for (auto j = i ; j < numOps ; j+= numThreads)
				{
					q.push(j);
				}
			}));
		}

		for (int i = 0; i < numThreads; ++i){
			threads.push_back(std::thread([&, i]{
				while( !flag )
					;
				uint64_t threadSum = 0;  
				for (auto j = i ; j < numOps; j+= numThreads){
					int v ; 
					q.pop(v);
					threadSum += v; 
				}
				sum += threadSum ;
			}));
		}

		flag = true ; 
		for (auto & thread : threads ){
			thread.join();
		}
		assert( sum == numOps * (numOps -1 ) / 2 ) ;
	}
	cerr << " Passed the multithread test case. " << endl;	
	cerr << " Passed all test cases! " << endl;	
	return 0;
}



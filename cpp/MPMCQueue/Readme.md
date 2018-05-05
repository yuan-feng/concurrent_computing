### Thread-safe Singleton Log 



This is a thread-safe Singleton Log in C++ .

Supports 

* Thread-safety.
* Customizable log-level.
* Red Color for Log::Error().

You can modify the PACKAGE_NAME to your own namespace and plug into your project quickly.

#### Example
File Log_demo.cpp

```
#include "Log.hpp" 

void allLog(){
	std::cout << "All Logging... " << std::endl;
	PACKAGE_NAME::Log::All()     << " All     " << std::endl;
	PACKAGE_NAME::Log::Debug()   << " Debug   " << std::endl;
	PACKAGE_NAME::Log::Verbose() << " Verbose " << std::endl;
	PACKAGE_NAME::Log::Info()    << " Info    " << std::endl;
	PACKAGE_NAME::Log::Warning() << " Warning " << std::endl;
	PACKAGE_NAME::Log::Error()   << " Error   " << std::endl;
	std::cout << "All Logging... Done! " << std::endl;
}

int main(int argc, char const *argv[])
{

	std::cout << "\nDefault Log level is All" << std::endl ;
	allLog();


	PACKAGE_NAME::Log::Config::get().setLevel( "Info" ) ;
	std::cout << "\nReduced Log Level to Info" << std::endl ;
	allLog();

	
	PACKAGE_NAME::Log::Config::get().setLevel( "Nothing" ) ;
	std::cout << "\nReduced Log Level to Nothing" << std::endl ;
	allLog();

	return 0 ;
}
```

Output is 
```
Default Log level is All
All Logging... 
[A]  All 
[D]  Debug 
[V]  Verbose 
[I]  Info 
[W]  Warning 
[E]  Error 
All Logging... Done! 

Reduced Log Level to Info
All Logging... 
[I]  Info 
[W]  Warning 
[E]  Error 
All Logging... Done! 

Reduced Log Level to Nothing
All Logging... 
All Logging... Done!
```


#### Tutorial



Reference : Ticket Lock
https://www.youtube.com/watch?v=r8eNGLY26T0

Reference : False Sharing
https://www.youtube.com/watch?v=dznxqe1Uk3E

Reference : MPMCQueue.h
https://github.com/rigtorp/MPMCQueue



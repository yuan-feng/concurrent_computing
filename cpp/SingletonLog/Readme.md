### Thread-safe Singleton Log 



This is a thread-safe Singleton Log in C++ .

Supports 

* Thread-safety.
* Customizable log-level.
* Red Color for Log::Error().

You can modify the PACKAGE_NAME to your own namespace and plug into your project quickly.

#### Examples
```
#include "Log.hpp" 

void allLog(){
	PACKAGE_NAME::Log::All() << " All " << std::endl;
	PACKAGE_NAME::Log::Debug() << " Debug " << std::endl;
	PACKAGE_NAME::Log::Verbose() << " Verbose " << std::endl;
	PACKAGE_NAME::Log::Info() << " Info " << std::endl;
	PACKAGE_NAME::Log::Warning() << " Warning " << std::endl;
	PACKAGE_NAME::Log::Error() << " Error " << std::endl;
}

int main(int argc, char const *argv[])
{

	allLog();

  	PACKAGE_NAME::Log::Config::get().setLevel( "Info" ) ;
  	PACKAGE_NAME::Log::Info() << std::endl << " Reduced Log Level to " << "Info" << std::endl ;
  	

	allLog();
	
	return 0 ;
}

```

Output is 
```
[A]  All 
[D]  Debug 
[V]  Verbose 
[I]  Info 
[W]  Warning 
[E]  Error 
[I] 
[I]  Reduced Log Level to Info
[I]  Info 
[W]  Warning 
[E]  Error 
```


<!-- [D]  Debug 
[V]  Verbose 
[I]  Info 
[W]  Warning 
<span style="color:red">[E]</span>  Error 
[I] 
[I]  Reduced Log Level to Info
[I]  Info 
[W]  Warning 
<span style="color:red">[E]</span>  Error  -->

#### Tutorial


###### Usage 1:
Customize the log-level with the following command:
```
PACKAGE_NAME::Log::Config::get().setLevel( "Info" ) ;
```

###### Usage 2:
Each Log must be ending with 
```
std::endl ;
```

###### Usage 3:
Modify PACKAGE_NAME to your own namespace.




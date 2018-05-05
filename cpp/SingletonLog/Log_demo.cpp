#include "Log.hpp" 

using namespace PACKAGE_NAME;

void allLog(){
	std::cout << "All Logging... " << std::endl;
	Log::All()     << " All     " << std::endl;
	Log::Debug()   << " Debug   " << std::endl;
	Log::Verbose() << " Verbose " << std::endl;
	Log::Info()    << " Info    " << std::endl;
	Log::Warning() << " Warning " << std::endl;
	Log::Error()   << " Error   " << std::endl;
	std::cout << "All Logging... Done! " << std::endl;
}

int main(int argc, char const *argv[])
{

	std::cout << "\nDefault Log level is All" << std::endl ;
	allLog();


	Log::Config::get().setLevel( "Info" ) ;
	std::cout << "\nReduced Log Level to Info" << std::endl ;
	allLog();

	
	Log::Config::get().setLevel( "Nothing" ) ;
	std::cout << "\nReduced Log Level to Nothing" << std::endl ;
	allLog();

	return 0 ;
}
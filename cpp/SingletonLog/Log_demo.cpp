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
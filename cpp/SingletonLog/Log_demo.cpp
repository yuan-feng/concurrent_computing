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

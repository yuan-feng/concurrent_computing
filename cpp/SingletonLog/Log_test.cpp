#include "Log.hpp" 
#include <thread>
#include <future>
#include <vector>
#include <functional>


size_t work() {

  std::ostringstream s; 
  s << "hi my name is: " << std::this_thread::get_id() ;
  
  for(size_t i  = 0; i < 3; ++i) {
    PACKAGE_NAME::Log::All() << s.str() << std::endl  ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    PACKAGE_NAME::Log::Debug() <<  s.str() << std::endl ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    PACKAGE_NAME::Log::Verbose() <<  s.str() << std::endl ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    PACKAGE_NAME::Log::Info() <<  s.str() << std::endl ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    PACKAGE_NAME::Log::Warning() <<  s.str() << std::endl ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    PACKAGE_NAME::Log::Error() << s.str() << std::endl; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

  }

  return 1;
}


int main(int argc, char const *argv[])
{

  // Basic Test
  PACKAGE_NAME::Log::All() << " The Default Log level is L_All " << std::endl;

  PACKAGE_NAME::Log::All() << " Testing the basic function ! " << std::endl;

  PACKAGE_NAME::Log::All() << " All " << std::endl;
  PACKAGE_NAME::Log::Debug() << " Debug " << std::endl;
  PACKAGE_NAME::Log::Verbose() << " Verbose " << std::endl;
  PACKAGE_NAME::Log::Info() << " Info " << std::endl;
  PACKAGE_NAME::Log::Warning() << " Warning " << std::endl;
  PACKAGE_NAME::Log::Error() << " Error " << std::endl;

  PACKAGE_NAME::Log::All() << " Testing the basic function is passed ! " << std::endl;



  std::string custimzedLogLevel = "Info" ; 
  // ******************************
  // Set the Log level
  // ******************************
  PACKAGE_NAME::Log::Config::get().setLevel( custimzedLogLevel ) ;
  // ******************************



  PACKAGE_NAME::Log::Info() << " The Log Level is set to " << custimzedLogLevel << std::endl;




  // ******************************
  // Multi-threading Test
  PACKAGE_NAME::Log::Error() << " Testing the Multi-threading function " << std::endl;
  
  std::vector<std::future<size_t> > results;
  for(size_t i = 0; i < 3; ++i) {
    results.emplace_back(std::async(std::launch::async, work) );
  }
  

  int exit_code = 0;
  for(auto& result : results) {
    try {
      size_t count = result.get();
      PACKAGE_NAME::Log::Info() << " count = " << count << std::endl;
    }
    catch(std::exception& e) {
      std::cout << e.what();
      exit_code++;
    }
  }

  PACKAGE_NAME::Log::Info() << " Testing the Multi-threading function is passed ! " << std::endl;
  
  PACKAGE_NAME::Log::Info() << " ************************************************** " << std::endl;
  PACKAGE_NAME::Log::Info() << " Usage Reminder: the log must end with \"std::endl\" ! " << std::endl;
  PACKAGE_NAME::Log::Info() << " ************************************************** " << std::endl;

  PACKAGE_NAME::Log::Info() << " All Testing functions are passed ! " << std::endl;

  return exit_code;
}





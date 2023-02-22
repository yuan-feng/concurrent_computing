#include "Log.hpp" 
#include <thread>
#include <future>
#include <vector>
#include <functional>

using namespace PACKAGE_NAME;

size_t work() {

  std::ostringstream s; 
  s << "hi my name is: " << std::this_thread::get_id() ;
  
  for(size_t i  = 0; i < 3; ++i) {
    Log::All() << s.str() << std::endl  ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    Log::Debug() <<  s.str() << std::endl ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    Log::Verbose() <<  s.str() << std::endl ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    Log::Info() <<  s.str() << std::endl ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    Log::Warning() <<  s.str() << std::endl ; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    Log::Error() << s.str() << std::endl; 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

  }

  return 1;
}


int main(int argc, char const *argv[])
{

  // Basic Test
  Log::All() << " The Default Log level is L_All " << std::endl;

  Log::All() << " Testing the basic function ! " << std::endl;

  Log::All() << " All " << std::endl;
  Log::Debug() << " Debug " << std::endl;
  Log::Verbose() << " Verbose " << std::endl;
  Log::Info() << " Info " << std::endl;
  Log::Warning() << " Warning " << std::endl;
  Log::Error() << " Error " << std::endl;

  Log::All() << " Testing the basic function is passed ! " << std::endl;



  std::string custimzedLogLevel = "Info" ; 
  // ******************************
  // Set the Log level
  // ******************************
  Log::Config::get().setLevel( custimzedLogLevel ) ;
  // ******************************



  Log::Info() << " The Log Level is set to " << custimzedLogLevel << std::endl;




  // ******************************
  // Multi-threading Test
  Log::Error() << " Testing the Multi-threading function " << std::endl;
  
  std::vector<std::future<size_t> > results;
  for(size_t i = 0; i < 3; ++i) {
    results.emplace_back(std::async(std::launch::async, work) );
  }
  

  int exit_code = 0;
  for(auto& result : results) {
    try {
      size_t count = result.get();
      Log::Info() << " count = " << count << std::endl;
    }
    catch(std::exception& e) {
      std::cout << e.what();
      exit_code++;
    }
  }

  Log::Info() << " Testing the Multi-threading function is passed ! " << std::endl;
  
  Log::Info() << " ************************************************** " << std::endl;
  Log::Info() << " Usage Reminder: the Log must end with \"std::endl\" ! " << std::endl;
  Log::Info() << " ************************************************** " << std::endl;

  Log::Info() << " All Testing functions are passed ! " << std::endl;

  return exit_code;
}





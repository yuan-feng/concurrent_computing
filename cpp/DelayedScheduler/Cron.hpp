#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>



namespace PACKAGE_NAME{
	using Clock = std::chrono::system_clock;

	inline void add ( std::tm & tm , Clock::duration time ){
		auto tp = Clock::from_time_t(std::mktime(&tm));
		auto tp_adjusted = tp + time ; 
		auto tm_adjusted = Clock::to_time_t(tp_adjusted) ;
		tm = *std::localtime(&tm_adjusted);
	}

	class BadCronExpression: public std::exception
	{
	public:
		explicit BadCronExpression(std::string msg )\
		: _msg(std::move(msg))
		{}

		const char *what() const noexcept override {
			return (_msg.c_str());
		}
	private:
		std::string _msg ;
	};

	inline void 
	verify_and_set(
		const std::string &token, 
		const std::string &expression, 
		int &field, 
		const int lower_bound,
        const int upper_bound, 
        const bool adjust = false)
	{
		if ( token == "*" ){
			field = -1 ; 
		}else{
			try{
				field = std::stoi(token);
			}catch(){
				
			}
		}
	}

} // namespace PACKAGE_NAME
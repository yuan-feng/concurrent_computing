#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include <iostream>
using std::cerr ;
using std::endl ; 

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
			}catch( const std::invalid_argument & ){
				throw BadCronExpression("malformed cron string (`" + token 
					+ "` not an integer or *): " + expression);
			}catch( const std::out_of_range & ){
				throw BadCronExpression("malformed cron string (`" + token 
					+ "` not convertible to int): " + expression );
			}

			if ( field < lower_bound || field > upper_bound ){
				std::ostringstream oss ; 
				oss << "malformed cron string ('" << token << "' must be <= " 
					<< upper_bound << " and >= " << lower_bound << "): " << expression ; 
				throw BadCronExpression(oss.str());
			}

			if (adjust){
				field--;
			}
		}
	} // function verify_and_set





	class Cron
	{
	public:
		explicit Cron( const std::string & expression ){
			std::istringstream iss(expression) ; 
			std::vector<std::string> tokens{
				std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>{}
			};

			if ( tokens.size() != 5 ){
				throw BadCronExpression(
					"malformed cron string (must be 5 fields): " + expression 
					);

				verify_and_set(tokens[0], expression, _minute, 0, 59 );
				verify_and_set(tokens[1], expression, _hour, 0, 23 );
				verify_and_set(tokens[2], expression, _day, 1, 31 );
				verify_and_set(tokens[3], expression, _month, 1, 12, true );
				verify_and_set(tokens[4], expression, _day_of_week, 0, 6 );
			}


		}
		
		Clock::time_point cron_to_next( const Clock::time_point from = Clock::now() ) const{
			auto now = Clock::to_time_t(from) ;
			// cerr << " cron_to_next debug 1 " << endl;
			std::tm next(*std::localtime(&now));
			// cerr << " cron_to_next debug 2 " << endl;

			next.tm_sec = 0 ;
			// cerr << " cron_to_next debug 3 " << endl;
			add( next, std::chrono::minutes(1) );
			// cerr << " cron_to_next debug 4 " << endl;
			while( true ){
				if ( _month != -1 && next.tm_mon != _month ){
					// cerr << " cron_to_next debug 5 " << endl;
					if ( next.tm_mon + 1 > 11 ){
						next.tm_mon = 0 ; 
						next.tm_year++ ; 
					}else{
						next.tm_mon++;
					}
					next.tm_mday = 1 ; 
					next.tm_hour = 0 ;
					next.tm_min = 0 ; 
					continue;
				}

				if ( _day != -1 && next.tm_mday != _day ){
					// cerr << " cron_to_next debug 6 " << endl;
					add( next, std::chrono::hours(24) );
					next.tm_hour = 0 ; 
					next.tm_min = 0 ; 
					continue;
				}

				if ( _day_of_week != -1 && next.tm_wday != _day_of_week ){
					// cerr << " cron_to_next debug 7 " << endl;
					add(next, std::chrono::hours(24)) ; 
					next.tm_hour = 0 ; 
					next.tm_min = 0 ; 
					continue;
				}

				if ( _hour != -1 && next.tm_hour != _hour) {
					// cerr << " cron_to_next debug 8 " << endl;
					add(next, std::chrono::hours(1) );
					next.tm_min = 0 ;
					continue;
				}

				if ( _minute != -1 && next.tm_min != _minute ){
					// cerr << " cron_to_next debug 9 " << endl;
					add(next, std::chrono::minutes(1) );
					continue;
				}

				break;
			}

			next.tm_isdst = -1 ; 
			return Clock::from_time_t(std::mktime(&next));
			// cerr << " cron_to_next debug 10 " << endl;
		}

		int _minute, _hour, _day, _month, _day_of_week ;
		
	}; // class Cron



} // namespace PACKAGE_NAME
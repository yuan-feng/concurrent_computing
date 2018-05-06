
#ifndef PACKAGE_NAME_UTILS_GUARD_INTERRUPTABLESLEEP_H_
#define PACKAGE_NAME_UTILS_GUARD_INTERRUPTABLESLEEP_H_


namespace PACKAGE_NAME{

	class InterruptableSleep
	{
		using Clock = std::chrono::system_clock ; 
	public:
		InterruptableSleep()
		: _interrupted(false)
		{}

		InterruptableSleep(InterruptableSleep const& ) = delete ;
		InterruptableSleep(InterruptableSleep && ) = delete ;

		InterruptableSleep(InterruptableSleep const& ) noexcept = delete ;
		InterruptableSleep(InterruptableSleep && ) noexcept = delete ;

		~InterruptableSleep() noexcept = default ;

		void sleep_for( Clock::duration duration ){
			std::unique_lock<std::mutex> lock(_lock) ;
			_cv.wait_for( lock , duration, [this]{ return _interrupted; } ) ; 
			_interrupted = false ; 
		}

		void sleep_until( Clock::time_point time ){
			std::unique_lock<std::mutex> lock(_lock) ; 
			_cv.wait_until( lock, time, [this]{ return _interrupted; }) ; 
			_interrupted = false ; 
		}

		void sleep(){
			std::unique_lock<std::mutex> lock(_lock) ;
			_cv.wait( lock, [this]{ return _interrupted ; }) ; 
			_interrupted = false ;
		}

		void interrupt(){
			std::lock_guard<std::mutex> lock(_lock);
			interrupted = true ;
			cv.notify_one();
		}

	private:
		bool _interrupted ; 
		std::mutex _m; 
		std::condition_variable _cv ; 
	}; // InterruptableSleep




}






#endif // PACKAGE_NAME_UTILS_GUARD_INTERRUPTABLESLEEP_H_

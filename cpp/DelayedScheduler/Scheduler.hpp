
#ifndef PACKAGE_NAME_UTILS_GUARD_SCHEDULER_H_
#define PACKAGE_NAME_UTILS_GUARD_SCHEDULER_H_

#include <iomanip>
#include <map>
#include "ctpl_stl.h"

#include "InterruptableSleep.h"
#include "Cron.h"

#define DEFAULT_MAX_N_TASKS 4

namespace PACKAGE_NAME{
	using Clock = std::chrono::system_clock;

	struct Task {
		explicit Task(std::function<void()> && f, 
			bool recur = false, 
			bool interval = false)
		: _f(std::move(f))
		, _recur(recur)
		, _interval(interval)
		{}

		virtual Clock::time_point get_new_time() const = 0 ; 

		std::function<void()> _f;
		bool _recur ; 
		bool _interval ; 
	};


	struct InTask : public Task{
		explicit InTask( std::function<void()> && f)
		: Task( std::move(f))
		{}

		// dummy time_point because it is not used
		Clock::time_point get_new_time() const override {
			return Clock::time_point(Clock::duration(0));
		}
	};



	struct EveryTask: public Task{
		EveryTask(Clock::duration time, 
			std::function<void()> && f,
			bool interval = false
			)
		: Task(std::move(f), true, interval)
		, _time(time)
		{}

		Clock::time_point get_new_time() const override{
			return Clock::time_point(Clock::duration(0)) ;
		}
	};

	struct CronTask: public Task {
		CronTask(const std::string & expression, 
			std::function<void()> && f)
		: Task(std::move(f), true)
		, _cron(expression)
		{}

		Clock::time_point get_new_time() const override{
			return cron.cron_to_next();
		};
		Cron _cron ;
	};

	inline bool try_parse( std::tm & tm, 
					const std::string& expression,
					const std::string& format)
	{
		std::stringstream ss(expression) ; 
		return !(ss >> std::get_time(&tm, format.c_str())).fail() ;
	}






	struct Schedular
	{
		explicit Schedular(unsigned int max_n_tasks = DEFAULT_MAX_N_TASKS )
		: _done(false)
		, _threads(max_n_tasks+1)
		{
			_threads.push([this](int){
				while( !_done ){
					if ( _tasks.empty() ){
						sleeper.sleep(); 
					} else {
						auto time_of_first_task = (*_tasks.begin()).first ;
						_sleeper.sleep_until(time_of_first_task);
					}
					manage_tasks();
				}
			});
		}

		Schedular( const Schedular & ) = delete ; 
		Schedular &operator=( const Schedular & )  = delete ; 

		Schedular( Schedular && ) noexcept = delete ; 
		Schedular &operator=( Schedular && ) noexcept = delete ; 

		~Schedular(){
			_done = true ; 
			_sleeper.interrupt() ; 
		}

		template< typename __Callable, typename ... __Args>
		void in( const Clock::time_point time, 
				__Callable && f,
				__Args && ... args )
		{
			std::shared_ptr<Task> t = std::make_shared<InTask>(
					std::bind( std::forward<__Callable>(f), std::forward<__Args>(args)... )
				);

			this->add_task(time, std::move(t));
		}

		template< typename __Callable, typename ... __Args>
		void in( const Clock::duration time, __Callable && f, __Args && ... args){
			in( Clock::now() + time, 
				std::forward<__Callable>(f), 
				std::forward<__Args>(args)...
			);
		}



		template<typename __Callable, typename ... __Args>
		void at( const std::string &time , __Callable, __Args && ... args){
			auto time_now = Clock::to_time_t(Clock::now());
			std::tm tm = *std::localtime(&time_now);

			Clock::time_point tp ; 

			if (try_parse(tm, time, "%H:%M:%S")){
				tp = Clock::from_time_t(std::mktime(&tm));

				// if we've already passed this time, the user means next day, so add a day.
				if (Clock::now() >= tp){
					tp += std::chrono::hours(24);
				}
			}else if ( try_parse(tm, time, "%Y-%m-%d %H:%M:%S") ){
				tp = Clock::from_time_t(std::mktime(&tm));
			}else if ( try_parse(tm, time, "%Y/%m/%d %H:%M:%S") ){
				tp = Clock::from_time_t(std::mktime(&tm));
			}else{
				throw std::runtime_error( "cannot parse time string " + time );
			}

			in( tp, std::forward<__Callable>(f), std::forward<__Args>(args)... );
		}




		template<typename __Callable, typename ... __Args>
		void every( const Clock::duration time, 
					__Callable && f, 
					__Args && ... args )
		{
			std::shared_ptr<Task> t = std::make_shared<EveryTask>(
					time, 
					std::bind(
						std::forward<__Callable>(f),
						std::forward<__Args>(args)...
						);
				);

			auto next_time = t->get_new_time() ;
			add_task(next_time, std::move(t) ) ; 
		}






// expression format:
// from https://en.wikipedia.org/wiki/Cron#Overview
//    ┌───────────── minute (0 - 59)
//    │ ┌───────────── hour (0 - 23)
//    │ │ ┌───────────── day of month (1 - 31)
//    │ │ │ ┌───────────── month (1 - 12)
//    │ │ │ │ ┌───────────── day of week (0 - 6) (Sunday to Saturday)
//    │ │ │ │ │
//    │ │ │ │ │
//    * * * * *
		template<typename __Callable, typename ... __Args>
		void cron( const std::string & expression,
				__Callable && f, 
				__Args && ... args)
		{
			std::shared_ptr<Task> t = std::make_shared<CronTask>(
					expression,
					std::bind(
							std::forward<__Callable>(f),
							std::forward<__Args>(args)...
						)
				);

			auto next_time = t->get_new_time();
			add_task( next_time, std::move(t) ); 
		}



		template<typename __Callable, typename ... __Args>
		void interval( const Clock::duration time, 
					__Callable && f, 
					__Args &&... args
				)
		{
			std::shared_ptr<Task> t = std::make_shared<EveryTask>(
					time, 
					std::bind(
							std::forward<__Callable>(f),
							std::forward<__Args>(args)...
						),
					true
				);

			add_task( Clock::now(), std::move(t) ) ;
		}


	private:

		std::atomic<bool> _done ; 

		PACKAGE_NAME::InterruptableSleep _sleeper ;

		std::multimap<Clock::time_point, std::shared_ptr<Task>> _tasks ; 

		std::mutex _lock ; 

		ctpl::thread_pool _threads ; 

		void add_task( const Clock::time_point time, 
				std::shared_ptr<Task> t
			)
		{
			std::lock_guard<std::mutex> l(lock);
			_tasks.emplace(time, std::move(t));
			_sleeper.interrupt();
		}


		void manage_tasks(){

			std::lock_guard<std::mutex> l(lock);

			auto end_of_tasks_to_run = _tasks.upper_bound(Clock::now()) ; 

			if ( end_of_tasks_to_run != _tasks.begin() )
			{
				decltype( _tasks ) recurred_tasks ; 

				for (auto i = _tasks.begin() ; i != end_of_tasks_to_run; ++i ){
					auto & task = (*i).second ; 

					if ( task->interval ) {
						_threads.push([this, task](){
							task->f();
							add_task(task->get_new_time, task);
						});
					} else {
						_threads.push([task](int){
							task->f();
						});

						if ( task->recur ){
							recurred_tasks.emplace(
									task->get_new_time(),
									std::move(task)
								);
						}
					}

					if ( task->recur ){
						recurred_tasks.emplace(task->get_new_time(), std::move(task));
					}
				} 


				_tasks.erase( _tasks.begin(), end_of_tasks_to_run ) ; 

				for (auto & task : recurred_tasks ){
					_tasks.emplace( task.first, std::move(task.second ) ) ; 
				}
			}

		 } // function manage_tasks



	}; // class Schedular
	

	
} // namespace PACKAGE_NAME

#endif // PACKAGE_NAME_UTILS_GUARD_SCHEDULER_H_


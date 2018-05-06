
#ifndef PACKAGE_NAME_UTILS_GUARD_TTASSPINLOCK_H_
#define PACKAGE_NAME_UTILS_GUARD_TTASSPINLOCK_H_


namespace PACKAGE_NAME {

class TTasSpinLock{

	std::atomic<int> _state{0} ; 

public:
	TTasSpinLock() = default ; 
	~TTasSpinLock() = default ; 

	TTasSpinLock( const TTasSpinLock& ) = delete ; 
	TTasSpinLock& operator=( const TTasSpinLock& ) = delete ; 

	void lock(){
		int expected = 0 ;
		do{
			while( _state.load( std::memory_order_relaxed) != 0 )
				;
			expected = 0 ; 
		}while(  not _state.compare_exchange_weak(expected, 1, std::memory_order_acquire ) ) ; 
	}

	bool try_lock(){
		int expected = 0 ;
		return _state.compare_exchange_weak(expected, 1, std::memory_order_acquire ) ;
	}

	void unlock(){
		_state.store(0, std::memory_order_release);
	}

}; // class TTasSpinLock


}




#endif //PACKAGE_NAME_UTILS_GUARD_TTASSPINLOCK_H_


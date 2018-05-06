
#ifndef PACKAGE_NAME_UTILS_GUARD_TASSPINLOCK_H_
#define PACKAGE_NAME_UTILS_GUARD_TASSPINLOCK_H_


namespace PACKAGE_NAME {

class TasSpinLock{

	std::atomic<int> _state{0} ; 

public:
	TasSpinLock() = default ; 
	~TasSpinLock() = default ; 

	TasSpinLock( const TasSpinLock& ) = delete ; 
	TasSpinLock& operator=( const TasSpinLock& ) = delete ; 

	void lock(){
		int expected = 0 ;

		while( not _state.compare_exchange_weak(expected, 1 ) ){
			expected = 0 ; 
		}
	}

	bool try_lock(){
		int expected = 0 ;
		return _state.compare_exchange_weak(expected, 1) ;
	}

	void unlock(){
		_state.store(0);
	}

}; // class TasSpinLock


}




#endif //PACKAGE_NAME_UTILS_GUARD_TASSPINLOCK_H_


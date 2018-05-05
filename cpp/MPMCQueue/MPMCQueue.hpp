

#ifndef PACKAGE_NAME_UTILS_GUARD_MPMCQueue_H_
#define PACKAGE_NAME_UTILS_GUARD_MPMCQueue_H_

#include <atomic>
#include <cassert>
#include <limits>
#include <memory>
#include <stdexcept>

namespace PACKAGE_NAME{

template <typename T> 
class MPMCQueue{

	static_assert(std::is_nothrow_copy_assignable<T>::value ||
		std::is_nothrow_move_assignable<T>::value, 
		"The element type in PACKAGE_NAME::MPMCQueue must be nothrow or move assignable");

	static_assert(std::is_nothrow_destructible<T>::value,
		"The element type in PACKAGE_NAME::MPMCQueue must be nothrow destructible");

	static constexpr size_t kCacheLineSize = 128;

public:
	explicit MPMCQueue(const size_t capacity)
	: _capacity(capacity)
	, _head(0)
	, _tail(0)
	{
		if( capacity < 1 ){
			throw std::invalid_argument("ERROR in PACKAGE_NAME::MPMCQueue ==> capacity < 1 ");
		}
		size_t space = capacity * sizeof(Slot) + kCacheLineSize - 1 ; 
		_buf = malloc(space) ; 
		if ( _buf == nullptr ){
			throw std::bad_alloc();
		}
		void *buf = _buf ; 
		_slots = reinterpret_cast<Slot *>(
			std::align(kCacheLineSize, capacity * sizeof(Slot), buf, space ));

		if ( _slots == nullptr )
		{
			free(_buf);
			throw std::bad_alloc();
		}

		for (size_t i = 0; i < _capacity; ++i)
		{
			new( &_slots[i] ) Slot() ;
		}

		static_assert( sizeof(MPMCQueue<T>) % kCacheLineSize == 0 ,
			"ERROR in PACKAGE_NAME::MPMCQueue "
			"queue_size \% CacheLineSize should equal 0"
			"to avoid false sharing. "
			);
		static_assert( sizeof(Slot) % kCacheLineSize == 0 ,
			"ERROR in PACKAGE_NAME::MPMCQueue "
			"slot_size \% CacheLineSize should equal 0"
			"to avoid false sharing. "
			);

		assert(reinterpret_cast<size_t>(_slots) % kCacheLineSize == 0 &&
			"ERROR in PACKAGE_NAME::MPMCQueue "
			" _slots array should be aligned "
			"to avoid false sharing. "
			);
		assert(reinterpret_cast<char *>(&_tail) - 
			reinterpret_cast<char *>(&_head) >= kCacheLineSize &&
			"ERROR in PACKAGE_NAME::MPMCQueue "
			"_head and _tail should be a cache line apart to prevent false sharing"
			);
	} // constructor MPMCQueue

	~MPMCQueue() noexcept{
		for (size_t i = 0; i < _capacity; ++i)
		{
			_slots[i].~Slot();
		}
		free(_buf);
	}

	MPMCQueue( const MPMCQueue & ) = delete; 
	MPMCQueue( MPMCQueue && ) = delete; 
	MPMCQueue &operator=( const MPMCQueue & ) = delete; 
	MPMCQueue &operator=( MPMCQueue && ) = delete; 

	template <typename ... Args>
	void emplace( Args &&... args ) noexcept {
		static_assert( std::is_nothrow_constructible<T, Args && ...>::value,
			"The element type in PACKAGE_NAME::MPMCQueue must be "
			"nothrow constructible with Args && ..."
			);

		auto const head = _head.fetch_add(1) ;
		auto &slot = _slots[idx(head)] ; 
		while( turn(head) * 2 == slot.turn.load(std::memory_order_acquire) )
			;
		slot.construct( std::forward<Args>(args)... ) ; 
		slot.turn.store( turn(head) * 2 + 1 , std::memory_order_release ) ; 
	}

	template <typename ... Args> 
	bool try_emplace( Args && ... args ) noexcept {
		static_assert(std::is_nothrow_constructible<T, Args &&...>::value,
			"The element type in PACKAGE_NAME::MPMCQueue must be "
			"nothrow constructible with Args && ..."
			);
		auto head = _head.load(std::memory_order_acquire) ; 
		for (;;){
			auto &slot = _slots[idx(head)] ; 
			if ( turn(head) * 2 == slot.turn.load(std::memory_order_acquire) ){
				if ( _head.compare_exchange_strong(head, head + 1 ) ) {
					slot.construct( std::forward<Args>(args)... ) ; 
					slot.turn.store( turn(head) * 2 + 1 , std::memory_order_release ) ; 
					return true ; 
				}
			}else{
				auto const prevHead = head ; 
				head = _head.load(std::memory_order_acquire) ; 
				if ( head == prevHead ){
					return false;
				}
			}
		}
	}

	void push( const T& v ) noexcept {
		static_assert(std::is_nothrow_copy_constructible<T>::value, 
			"The element type in PACKAGE_NAME::MPMCQueue must be nothrow copy constructible"
			);
		emplace(v);
	}

	template < typename P,
		typename = typename std::enable_if<
			std::is_nothrow_constructible<T, P &&>::value>::type>
	void push( P && v ) noexcept {
		emplace(std::forward<P>(v));
	}

	bool try_push( const T& v ) noexcept {
		static_assert( std::is_nothrow_copy_constructible<T>::value, 
			"The element type in PACKAGE_NAME::MPMCQueue must be nothrow copy constructible"
			);
		return try_emplace(v) ;
	}

	template < typename P,
		typename = typename std::enable_if<
			std::is_nothrow_constructible<T, P &&>::value>::type>
	bool try_push( P && v) noexcept {
		return try_emplace(std::forward<P>(v));
	}



	void pop( T & v ) noexcept{
		auto const tail = _tail.fetch_add(1) ; 
		auto &slot = _slots[idx(tail)] ; 
		while( turn(tail) * 2 + 1 != slot.turn.load(std::memory_order_acquire) )
			;
		v = slot.move() ; 
		slot.destory();
		slot.turn.store( turn(tail)*2 + 2, std::memory_order_release ) ; 
	}

	bool try_pop( T & v ) noexcept {
		auto tail = _tail.load( std::memory_order_acquire ) ;
		for (;;){
			auto & slot = _slots[idx(tail)];
			if( turn(tail) * 2 + 1 == slot.turn.load(std::memory_order_acquire)){
				if ( _tail.compare_exchange_strong(tail, tail+1)){
					v = slot.move() ; 
					slot.destory();
					slot.turn.store(turn(tail) * 2 + 2 , std::memory_order_release ) ;
					return true;
				}
			} else {
				auto const prevTail = tail ; 
				tail = _tail.load( std::memory_order_acquire ) ; 
				if ( tail == prevTail) {
					return false ; 
				}
			}
		}
	}

private:
	constexpr size_t idx(size_t i) const noexcept { 
		return i % _capacity ; 
	}
	constexpr size_t turn(size_t i ) const noexcept{
		return i / _capacity ; 
	}

	struct Slot{
		~Slot() noexcept {
			if ( turn & 1 ){
				destory();
			}
		}

		template <typename ... Args> 
		void construct( Args && ... args ) noexcept {
			static_assert(std::is_nothrow_constructible<T, Args &&...>::value,
				"The element type in PACKAGE_NAME::MPMCQueue must be nothrow constructible with construct(Args&& ..."
				);
			new (&storage) T( std::forward<Args>(args)... );
		}

		void destory() noexcept {
			static_assert( std::is_nothrow_destructible<T>::value, 
				"The element type in PACKAGE_NAME::MPMCQueue must be nothrow destructible"
				);
			reinterpret_cast<T *>( &storage) -> ~T();
		}

		T &&move() noexcept {
			return reinterpret_cast<T &&>( storage ) ; 
		}

		alignas(kCacheLineSize) std::atomic<size_t> turn = {0} ; 
		typename std::aligned_storage<sizeof(T), alignof(T)>::type storage ;
	}; // struct Slot




private:
	
	const size_t _capacity ;
	Slot *_slots ; 
	void *_buf ; 

	alignas(kCacheLineSize) std::atomic<size_t> _head ; 
	alignas(kCacheLineSize) std::atomic<size_t> _tail ; 

}; // class MPMCQueue
} // namespace PACKAGE_NAME







#endif // PACKAGE_NAME_UTILS_GUARD_MPMCQueue_H_


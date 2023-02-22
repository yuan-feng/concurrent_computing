// Thread-safe Log

// ### Usage 1:
// Customize the log-level with the following command:
// PACKAGE_NAME::Log::Config::get().setLevel( argv[i] ) ;

// ### Usage 2:
// Each Log must be ending with std::endl ;



#ifndef PACKAGE_NAME_UTILS_GUARD_LOG_H_
#define PACKAGE_NAME_UTILS_GUARD_LOG_H_

#define PACKAGE_NAME_stringify(s) #s

#define DEFAULT_LOG_LEVEL L_All


#include "LogLevels.hpp"
#include <mutex>
#include <iostream>
#include <sstream>





namespace PACKAGE_NAME{
	namespace Log{
		struct Config{
			std::ostream* out ; 
			std::ostream* err ;

			Level level; 

			bool no_prefix; 

			static Config& get(){
				static Config s_config ; 
				return s_config ; 
			}

			void setLevel( std::string const& levelStr ){
				#define PACKAGE_NAME_LOG_LEVEL_( l ) \
				  if( levelStr == PACKAGE_NAME_stringify( l ) ){ level = L_##l ; }

				  PACKAGE_NAME_LOG_LEVELS

				  if( levelStr == "Nothing" ){ level = L_Nothing ; }

				#undef PACKAGE_NAME_LOG_LEVEL_

			}

		private:
			Config()
			: out(nullptr)
			, err(nullptr)
			, level(DEFAULT_LOG_LEVEL)
			, no_prefix(false)
			{}

		}; // config


		// declaration of log_impl
		namespace log_impl{
			template < Level L >
			struct Log ; 

			template < Level L >
			struct Stream {
				template< typename T>
				Log<L>& operator<<( const T& o ) const ;

				Log<L>& operator<<( std::ostream& modifier(std::ostream& ) ) const ; 

				static std::ostream& as_std_ostream() ; 

				static std::ostream& unsafe() ; 

			private:
				static Log<L>& impl() ; 

			}; // stream 
		} // log_impl


		// typedef the name for each log level
		#define PACKAGE_NAME_LOG_LEVEL_( level ) \
			typedef log_impl::Stream<L_##level> level ;
			
			PACKAGE_NAME_LOG_LEVELS

			typedef log_impl::Stream< L_Nothing > Nothing ; 
		#undef PACKAGE_NAME_LOG_LEVEL_


		// full and abbr names
		template< Level L > 
		struct LevelNames{
			static const char* full() { return "Nothing" ; }
			static char abbrev() { return 'N'; }
		} ;
		// for each level 
		#define PACKAGE_NAME_LOG_LEVEL_(level) \
			template<> struct \
			LevelNames< L_##level > { \
				static const char* full() { return #level ; } \
				static char abbrev() { return #level[0] ; } \
			};

			PACKAGE_NAME_LOG_LEVELS

			;
		#undef PACKAGE_NAME_LOG_LEVEL_




		// implementation of log_impl
		namespace log_impl{

			struct LogBase
			{
				explicit LogBase( std::stringbuf *buf )
				: stream(buf)
				{
					Config & c = Config::get() ; 
					if ( !c.out )
					{
						c.out = &std::cout ; 
					}

					if ( !c.err )
					{
						c.err = &std::cerr ; 
					}
				}

				template< Level l >
				static const char * prefix() {
					static const char s_prefix[5] = {
							'[', LevelNames< l > ::abbrev(), ']', ' ', '\0'
						};

					return ( Config::get().no_prefix ) ? "" : s_prefix ;
				}


				static std::recursive_mutex& global_mutex() {
					static std::recursive_mutex s_global ; 
					return s_global ; 
				}

				std::ostream stream ; 
			}; // LogBase

			// Special for Error with red color.
			template<>
			const char * LogBase::prefix<L_Error>() {
				static const char s_prefix[14] = {
							'\x1b', '[', '3', '1', 'm', '[', 'E', ']', '\x1b', '[', '0', 'm', ' ', '\0'
						}; // "\x1b[31m[E]\x1b[0m "
				return ( Config::get().no_prefix ) ? "" : s_prefix ;
			}


			template < Level L > 
			struct Buf : public std::stringbuf {
				std::ostream* &stream ; 

				std::recursive_mutex mutex ; 
				volatile bool locked ;

				Buf( std::ostream* & stream_ptr )
				:stream( stream_ptr )
				{}

				// Called each time when the stream is flushed (e.g. for each std::endl )
				virtual int sync ()
				{
					{
						// Output prefix + buffered string
						std::lock_guard<std::recursive_mutex> lock( LogBase::global_mutex() );
						*stream << LogBase::prefix<L>() << str() ; 
						stream->flush();
					}

					str("");

					// Release mutex
					if ( locked )
					{
						locked = false; 
						mutex.unlock();
					}

					return 0 ; 
				}

				virtual std::streamsize xsputn ( const char* s, std::streamsize n )
				{
					acquire_mutex();
					return std::stringbuf::xsputn( s, n ) ;
				}

				void acquire_mutex()
				{
					mutex.lock() ;
					if ( locked )
					{
						mutex.unlock() ; 
					}

					locked = true ; 
				}

			}; // Buf





			template < Level L > 
			struct Log: public LogBase{
				Buf< L > buffer ;

				Log() 
				: LogBase( &buffer )
				, buffer ( Config::get().out )
				{}

				Log& thread_safe() { 
					buffer.acquire_mutex() ; 
					return *this; 
				}

				std::ostream& unsafe() {
					return *buffer.stream << LogBase::prefix< L_Error > () ; 
				}
			}; // Log 

			// special for Level Error
			template <>
			struct Log< L_Error > : public LogBase{
				Buf< L_Error > buffer ;

				Log() 
				: LogBase( &buffer )
				, buffer( Config::get().err )
				{}

				Log& thread_safe() {
					buffer.acquire_mutex();
					return *this; 
				}

				std::ostream& unsafe(){
					return *buffer.stream << LogBase::prefix<L_Error>() ; 
				}
			}; // Log special for level error 



			template < Level L, typename T > 
			Log< L > & operator << ( Log<L> & s , const T& o )
			{
				if( L < Config::get().level ){
					return s ; 
				}

				s.thread_safe().stream << o ; 
				return s ; 
			}

			template < Level L > 
			Log< L > & operator << ( Log<L> & s, std::ostream& modifier( std::ostream& ) )
			{
				if ( L < Config::get().level )
				{
					return s ; 
				}

				s.thread_safe().stream << modifier ; 

				return s ;
			}

			// special for level Nothing 
			template < >
			struct Stream < L_Nothing >
			{
				static std::ostream& as_std_ostream() {
					return impl() ; 
				}
				static std::ostream& unsafe() {
					return as_std_ostream() ; 
				}

			private:
				struct Black_hole_buf: public std::streambuf{
					std::streamsize xsputn( const char * , std::streamsize n ) override {
						return n ;
					}
					int overflow(int ) override {
						return 1; 
					}
				};

				static std::ostream& impl(){
					static Black_hole_buf s_buf ;
					static std::ostream s_stream ( &s_buf ) ;
					return s_stream ; 
				}
			}; // Stream <Nothing >


			// filtered, thread-safe operator << 
			template< Level L > 
			template< typename T > 
			Log< L >& Stream< L >::operator << ( const T& o ) const
			{
				return ( impl() << o ) ; 
			}

			// filtered, thread-safe operator << for stream manipulators
			template< Level L > 
			Log< L >& Stream< L >::operator << ( std::ostream& modifier( std::ostream&) ) const
			{
				return (impl() << modifier ) ; 
			}

			template< Level L > 
			std::ostream& Stream< L >::as_std_ostream()
			{
				if ( L < Config::get().level )
				{
					return Nothing::as_std_ostream() ; 
				}
				return impl().thread_safe().stream ;
			}

			template< Level L > 
			std::ostream& Stream< L >::unsafe()
			{
				if ( L < Config::get().level )
				{
					return Nothing::as_std_ostream() ; 
				}
				return impl().unsafe() ;
			}

			template< Level L > 
			Log<L>& Stream< L >::impl(){
				static Log< L > s_impl ;
				return s_impl;
			}
			

		} // namespace implementation of log_impl
	} // namespace Log
} // namespace PACKAGE_NAME



#endif // PACKAGE_NAME_UTILS_GUARD_LOG_H_


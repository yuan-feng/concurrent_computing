#ifndef PACKAGE_NAME_UTILS_GUARD_LOGLEVELS_H_
#define PACKAGE_NAME_UTILS_GUARD_LOGLEVELS_H_


#define PACKAGE_NAME_LOG_LEVELS \
	PACKAGE_NAME_LOG_LEVEL_( All ) \
	PACKAGE_NAME_LOG_LEVEL_( Debug ) \
	PACKAGE_NAME_LOG_LEVEL_( Verbose ) \
	PACKAGE_NAME_LOG_LEVEL_( Info ) \
	PACKAGE_NAME_LOG_LEVEL_( Warning ) \
	PACKAGE_NAME_LOG_LEVEL_( Error )

	namespace PACKAGE_NAME{
		namespace Log{

			enum Level{
				#define PACKAGE_NAME_LOG_LEVEL_( level ) L_##level,
					PACKAGE_NAME_LOG_LEVELS
				#undef PACKAGE_NAME_LOG_LEVEL_

				L_Nothing,
			};

		} // Log
	} // PACKAGE_NAME



#endif
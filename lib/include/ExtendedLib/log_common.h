/**
* @file
* @brief      Header for the log and error management common definitions
* @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
* @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
* @license    BSD License (See LICENSE.txt)
*/

#ifndef EXTENDEDLIB_log_common_h__
#define EXTENDEDLIB_log_common_h__

///@defgroup log Logging
///@{

#ifndef LOG_CATEGORY
/**
 * Name of the category for the messages that will be logged.
 *
 * By default, logged messages will be recorded using the default (empty) category.
 *
 * To set a custom category just define the macro with the name of the category
 * before including <ExtendedLib/log.h>.
 *
 * @par Example
 * @code{.cpp}
 * #define LOG_CATEGORY "MyCategoryName"
 * #include <ExtendedLib/log.h>
 * @endcode
 */
#define LOG_CATEGORY ((const char*)0)
#endif

/**
 * @def LOG_PRIORITY_MAX
 *
 * Sets at compile-time the lowest priority limit for logging messages.
 *
 * Messages with priority lower (higher "value") than LOG_PRIORITY_MAX priority will be discarded
 * at compile-time.
 *
 * By default, the compile-time lowest priority will be LOG_PRIORITY_INFO for "Release"
 * configurations and LOG_PRIORITY_TRACE for "Debug" configurations.
 *
 * To set a custom lowest priority, define the macro with the numeric value corresponding to the
 * custom priority before including <ExtendedLib/log.h> or in compiler command-line parameters
 * (e.g. @c -DLOG_PRIORITY_MAX=n).
 *
 * @remark
 * At runtime, calling log::set_priority_limit() can further limit the messages that are actually
 * logged. However, the macro LOG_PRIORITY_MAX sets the lowest priority limit at compile-time, therefore
 * setting the log priority at runtime lower than LOG_PRIORITY_MAX will behave just as
 * setting it to LOG_PRIORITY_MAX.
 *
 * @see log::set_priority_limit()
 */

///@name log Priority Values (lower values have higher priority)
///@{
#define LOG_PRIORITY_ERROR            1 ///< %log priority @c ERROR numeric value
#define LOG_PRIORITY_WARN            2 ///< %log priority @c WARN numeric value
#define LOG_PRIORITY_INFO            3 ///< %log priority @c INFO numeric value
#define LOG_PRIORITY_DEBUG            4 ///< %log priority @c TRACE numeric value
#define LOG_PRIORITY_TRACE          5 ///< %log priority @c DEBUG numeric value
#define LOG_PRIORITY_DEBUG_EXTRA    6 ///< %log priority @c DEBUG_EXTRA numeric value
#define LOG_PRIORITY_ALLOC            7 ///< %log priority @c ALLOC numeric value
///@}

#ifndef LOG_PRIORITY_MAX
#ifdef _DEBUG_
#define LOG_PRIORITY_MAX            LOG_PRIORITY_TRACE
#else
#define LOG_PRIORITY_MAX            LOG_PRIORITY_INFO
#endif
#endif

///@cond
#ifdef __GNUC__
#define __FUNCTION_INFO__     __PRETTY_FUNCTION__
#else
#define __FUNCTION_INFO__     __FUNCTION__
#endif
///@endcond

///@}

#endif // header guard

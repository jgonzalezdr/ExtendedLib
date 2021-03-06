/**
 * @file
 * @brief      Header for the log management related classes and macros
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_log_hpp_
#define Extended_log_hpp_

///@defgroup log Logging
///@{

#include <memory>

#include "extended_config.hpp"
#include "log_common.hpp"

namespace ext
{

/**
 * The log_handler abstract class is the base class for implementations of log handlers
 * (to process the log messages generated by the application).
 */
class Extended_API log_handler
{
public:
    virtual ~log_handler()
    {}

    /**
     * This method is called by the log management system when a log message is generated by the application.
     *
     * @param[in] prio Priority of the message
     * @param[in] category Category of the message
     * @param[in] function Name of the function or method where the message was generated
     * @param[in] msg Message text
     * @retval true if the message must be also logged to console (if verbose mode is activated)
     * @retval false otherwise
     */
    virtual bool process( int prio, const char* category, const char* function, const char* msg ) = 0;
};

/**
 * The log singleton class provides access to the log management functionalities.
 */
class Extended_API log
{
public:
    ///@cond INTERNAL
    /**
     * Logs a message.
     *
     * @param[in] prio Priority of the message (one of LOG_PRIORITY_xxx)
     * @param[in] category Category of the message (may be NULL)
     * @param[in] function Name of the function or method where the message was generated
     * @param[in] msg Message format string (using printf format)
     * @param[in] ... Variable parameters for the format string
     */
    static void log_message( int prio, const char* category, const char* function, const char* format, ... );
    ///@endcond

    /**
     * Gets the current priority limit for logging messages.
     */
    static int get_priority_limit() noexcept;

    /**
     * Sets the runtime priority limit for logging messages.
     *
     * Messages with priority lower (higher "value") than the given priority will be ignored.
     *
     * @remark
     * The macro LOG_PRIORITY_MAX sets the lowest priority limit at compile-time. Therefore,
     * setting the log priority at runtime lower than LOG_PRIORITY_MAX will behave just as
     * setting it to LOG_PRIORITY_MAX.
     *
     * @see LOG_PRIORITY_MAX
     *
     * @param[in] logPriorityLimit Priority limit to be set
     */
    static void set_priority_limit( int logPriorityLimit ) noexcept;

    /**
     * Gets the currently installed log handler.
     */
    static const std::shared_ptr<log_handler>& get_log_handler() noexcept;

    /**
     * Sets a new log handler.
     *
     * If a log handler was set previously, the new log handler replaces the old one.
     *
     * @param[in] userLogHandler New log handler
     */
    static void set_log_handler( const std::shared_ptr<log_handler>& userLogHandler ) noexcept;

private:
    log() {}; // Make it non-instantiable
};

} // namespace

template class Extended_API std::shared_ptr<ext::log_handler>;

///@name Message Logging Macros
///@{

///@cond INTERNAL
/**
 * Logs a formatted message with the given priority.
 */
#define LOG(prio,str,...) ext::log::log_message( prio, LOG_CATEGORY, __FUNCTION_INFO__, str, ##__VA_ARGS__ )
///@endcond

/**
 * \def LOG_ERROR
 * Logs a message with @c ERROR priority.
 * @param[in] str Message format string (using printf format)
 * @param[in] ... Variable parameters for the format string
 */
#if LOG_PRIORITY_MAX >= LOG_PRIORITY_ERROR
#define LOG_ERROR(str,...) LOG( LOG_PRIORITY_ERROR, str, ##__VA_ARGS__ )
#else
#define LOG_ERROR(str,...)
#endif

/**
 * \def LOG_WARN
 * Logs a message with @c WARN priority.
 * @param[in] str Message format string (using printf format)
 * @param[in] ... Variable parameters for the format string
 */
#if LOG_PRIORITY_MAX >= LOG_PRIORITY_WARN
#define LOG_WARN(str,...) LOG( LOG_PRIORITY_WARN, str, ##__VA_ARGS__ )
#else
#define LOG_WARN(str,...)
#endif

/**
 * \def LOG_INFO
 * Logs a message with @c INFO priority.
 * @param[in] str Message format string (using printf format)
 * @param[in] ... Variable parameters for the format string
 */
#if LOG_PRIORITY_MAX >= LOG_PRIORITY_INFO
#define LOG_INFO(str,...) LOG( LOG_PRIORITY_INFO, str, ##__VA_ARGS__ )
#else
#define LOG_INFO(str,...)
#endif

/**
 * \def LOG_DEBUG
 * Logs a message with @c DEBUG priority.
 * @param[in] str Message format string (using printf format)
 * @param[in] ... Variable parameters for the format string
 */
#if LOG_PRIORITY_MAX >= LOG_PRIORITY_DEBUG
#define LOG_DEBUG(str,...) LOG( LOG_PRIORITY_DEBUG, str, ##__VA_ARGS__ )
#else
#define LOG_DEBUG(str,...)
#endif

 /**
 * \def LOG_TRACE
 * Logs a message with @c TRACE priority.
 * @param[in] str Message format string (using printf format)
 * @param[in] ... Variable parameters for the format string
 */
#if LOG_PRIORITY_MAX >= LOG_PRIORITY_TRACE
#define LOG_TRACE(str,...) LOG( LOG_PRIORITY_TRACE, str, ##__VA_ARGS__ )
#else
#define LOG_TRACE(str,...)
#endif

 /**
 * \def LOG_DEBUG_EXTRA
 * Logs a message with @c DEBUG_EXTRA priority.
 * @param[in] str Message format string (using printf format)
 * @param[in] ... Variable parameters for the format string
 */
#if LOG_PRIORITY_MAX >= LOG_PRIORITY_DEBUG_EXTRA
#define LOG_DEBUG_EXTRA(str,...) LOG( LOG_PRIORITY_DEBUG_EXTRA, str, ##__VA_ARGS__ )
#else
#define LOG_DEBUG_EXTRA(str,...)
#endif

/**
 * \def LOG_ALLOC
 * Logs a message with @c ALLOC priority.
 * @param[in] str Message format string (using printf format)
 * @param[in] ... Variable parameters for the format string
 */
#if LOG_PRIORITY_MAX >= LOG_PRIORITY_ALLOC
#define LOG_ALLOC(str,...)    LOG( LOG_PRIORITY_ALLOC, str, ##__VA_ARGS__ )
#else
#define LOG_ALLOC(str,...)
#endif

///@}

///@name Utility Macros
///@{

/**
 * Useful macro to trace function/method entries.
 *
 * @par Example
 * @code{.cpp}
 * void function( int p )
 * {
 *     TRACE_FUNCTION_ENTRY
 *
 *     // ... Do something ...
 *
 *     TRACE_FUNCTION_EXIT
 * }
 * @endcode
 */
#define TRACE_FUNCTION_ENTRY  LOG_TRACE("Entry");

 /**
 * Useful macro to trace function/method exits.
 *
 * @par Example
 * @code{.cpp}
 * void function( int p )
 * {
 *     TRACE_FUNCTION_ENTRY
 *
 *     // ... Do something ...
 *
 *     TRACE_FUNCTION_EXIT
 * }
 * @endcode
 */
#define TRACE_FUNCTION_EXIT   LOG_TRACE("Exit");

///@}

///@}

#endif // header guard

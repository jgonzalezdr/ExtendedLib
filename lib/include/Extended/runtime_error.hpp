/**
 * @file
 * @brief      Header for the error management related classes and macros
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_runtime_error_hpp_
#define Extended_runtime_error_hpp_

#include "extended_config.hpp"
#include <stdexcept>
#include "log_common.hpp"
#include <memory>

///@defgroup error Runtime Errors
///@{

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )
#endif

namespace ext
{

/**
 * The runtime_error class represents errors that are detected while a program
 * executes.
 *
 * It's an extension of the STL \p runtime_error class, with addition functionalities
 * to take advantage of the ExtendedLib logging subsystem.
 *
 * To throw runtime errors it's recommended to use the utility macros
 * #THROW_ERROR / #THROW_ERROR_LOG.
 *
 * @par Example
 * @code{.cpp}
 * try
 * {
 *    int errorCode = someFunction();
 *
 *    if( errorCode != 0 )
 *    {
 *        THROW_ERROR( "someFuntion() returned the error code '%d'", errorCode );
 *    }
 * }
 * catch( ext::runtime_error &e )
 * {
 *     e.log();
 * }
 *
 * @endcode
 *
 * @see log
 */
class Extended_API runtime_error : public std::exception
{
public:
    /**
     * Constructor for a error message text.
     *
     * If @a log_on_throw is @c true, the error is logged immediately. Otherwise, the
     * error will only be logged by explicitly calling @ref log() or if the
     * exception is not caught.
     *
     * @remarks
     * To throw runtime errors it's recommended to use the utility macros
     * #THROW_ERROR / #THROW_ERROR_LOG rather than constructing "manually" one of these
     * objects.
     *
     * @param[in] category Category of the error message (may be NULL)
     * @param[in] function Name of the function or method where the error is thrown
     * @param[in] log_on_throw If @c true, the error is logged immediately
     * @param[in] msg Textual description of the error
     */
    runtime_error( const char *category, const char *function, bool log_on_throw, const std::string &msg );

    /**
     * Constructor for a error message constructed from a format and a variable number of
     * arguments (printf-like).
     *
     * If @a log_on_throw is @c true, the error is logged immediately. Otherwise, the
     * error will only be logged by explicitly calling @ref log() or if the
     * exception is not caught.
     *
     * @remarks
     * To throw runtime errors it's recommended to use the utility macros
     * #THROW_ERROR / #THROW_ERROR_LOG rather than constructing "manually" one of these
     * objects.
     *
     * @param[in] category Category of the error message (may be NULL)
     * @param[in] function Name of the function or method where the error is thrown
     * @param[in] log_on_throw If @c true, the error is logged immediately
     * @param[in] format Format string for the error message
     * @param[in] ... Variable parameters for the format string
     */
    runtime_error( const char *category, const char *function, bool log_on_throw, const char *format, ... );

    /**
     * Destructor.
     */
    virtual ~runtime_error() noexcept
    {}

    /**
     * Logs the error.
     *
     * Errors can only be logged once; calling this method more than once or
     * if the message was logged when thrown is just ignored.
     */
    void log() const;

    /**
     * Returns the category of the error message.
     */
    const std::string& get_category() const noexcept
    {
        return m_category;
    }

    /**
     * Returns the name of the function or method where the error was thrown.
     */
    const std::string& get_function() const noexcept
    {
        return m_function;
    }

    /**
     * Returns the textual description of the error.
     */
    const std::string& get_message() const noexcept
    {
        return m_message;
    }

    /**
     * Returns the textual description of the error.
     */
    virtual const char* what() const noexcept
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
    std::string m_category;
    std::string m_function;
    mutable bool m_logged;
};

} // namespace

#ifdef _MSC_VER
#pragma warning( pop )
#endif

///@name Exception Throwing Macros
///@{

///@cond INTERNAL
#define THROW_EXCEPTION( exception, str, ... ) \
    throw exception( LOG_CATEGORY, __FUNCTION_INFO__, false, str, ##__VA_ARGS__ )

#if LOG_PRIORITY_MAX >= LOG_PRIORITY_ERROR

#define THROW_EXCEPTION_LOG( exception, str, ... ) \
    throw exception( LOG_CATEGORY, __FUNCTION_INFO__, true, str, ##__VA_ARGS__ )

#else

#define THROW_EXCEPTION_LOG( exception, str, ... ) \
    THROW_EXCEPTION( exception, str, ##__VA_ARGS__ )

#endif
///@endcond INTERNAL

/**
* Throws a runtime_error exception with a custom error message.
*
* The error message won't be logged unless the exception is not caught or
* if it's explicitly logged (calling runtime_error::log()).
*
* @param[in] str Message format string (using printf format)
* @param[in] ... Variable parameters for the format string
*/
#define THROW_ERROR( str, ... ) \
    THROW_EXCEPTION( ext::runtime_error, str, ##__VA_ARGS__ )

/**
* Throws a runtime_error exception with a custom error message, which is immediately
* logged.
*
* @param[in] str Message format string (using printf format)
* @param[in] ... Variable parameters for the format string
*/
#define THROW_ERROR_LOG( str, ... ) \
    THROW_EXCEPTION_LOG( ext::runtime_error, str, ##__VA_ARGS__ )

///@}

///@}

#endif // header guard

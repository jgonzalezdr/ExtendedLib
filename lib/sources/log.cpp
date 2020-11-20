/**
 * @file
 * @brief      Implementation of the log management related classes
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#include "Extended/log.hpp"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <exception>
#include <errno.h>

#include "Extended/string.hpp"
#include "Extended/runtime_error.hpp"

using namespace ext;

#if defined(__GNUC__) && !defined(WIN32)
    #define STACKTRACE_SUPPORTED
    #include <execinfo.h>
    #include <cxxabi.h>
#endif

static std::string program_name;

#if defined(WIN32)
    #include <Windows.h>

    #ifndef UNIT_TEST
    #define __OutputDebugString OutputDebugString
    #else
    extern "C" {
    void WINAPI OutputDebugStringUT( _In_opt_ LPCTSTR lpOutputString );
    }
    #define __OutputDebugString OutputDebugStringUT
    #endif

#elif !defined(__GNUC__)
    #error "Unsupported system"
#endif

#ifdef WIN32
    #define COLOR_RESET ""
    #define COLOR_MAGENTA ""
    #define COLOR_RED ""
    #define COLOR_YELLOW ""
    #define COLOR_BLUE ""
    #define COLOR_GRAY ""
#else
    #define COLOR_RESET "\x1b[0m"
    #define COLOR_MAGENTA "\x1b[35m"
    #define COLOR_RED "\x1b[31;1m"
    #define COLOR_YELLOW "\x1b[33;1m"
    #define COLOR_BLUE "\x1b[34m"
    #define COLOR_GRAY "\x1b[30;1m"
#endif

static const char* get_prio_header( int prio )
{
    switch( prio )
    {
    case LOG_PRIORITY_ERROR:
        return COLOR_RED "[ERROR]" COLOR_RESET;
    case LOG_PRIORITY_WARN:
        return COLOR_YELLOW "[WARN]" COLOR_RESET;
    case LOG_PRIORITY_INFO:
        return COLOR_BLUE "[INFO]" COLOR_RESET;
    case LOG_PRIORITY_TRACE:
        return COLOR_GRAY "[TRACE]";
    case LOG_PRIORITY_DEBUG:
        return COLOR_MAGENTA "[DEBUG]" COLOR_GRAY;
    case LOG_PRIORITY_DEBUG_EXTRA:
        return COLOR_MAGENTA "[XTDBG]" COLOR_GRAY;
    case LOG_PRIORITY_ALLOC:
        return "[ALLOC]";
    default:
        return "[UNKNOWN]"; // LCOV_EXCL_LINE
    }
}

static const char* get_prio_end( int prio )
{
    switch( prio )
    {
    case LOG_PRIORITY_TRACE:
    case LOG_PRIORITY_DEBUG:
    case LOG_PRIORITY_DEBUG_EXTRA:
        return COLOR_RESET;
    default:
        return "";
    }
}

static int g_priorityLimit = LOG_PRIORITY_ALLOC;
static log_handler::XPtr g_logHandler = NULL;

#ifdef __GNUC__

static std::string simplify_function(const char* function)
{
    std::string func(function);
    unsigned int epos = func.rfind('(');
    if( epos == std::string::npos )
    {
        return func;
    }

    unsigned int bpos = func.rfind(' ', epos-1);
    if( bpos == std::string::npos )
    {
        bpos = 0;
    }
    else
    {
        bpos++;
    }

    return func.substr( bpos, epos-bpos );
}

#else
#define simplify_function(x) x
#endif

void do_log_msg( int prio, const char* category, const char* function, const char* msg )
{
    if( prio > log::get_priority_limit() )
    {
        return; // LCOV_EXCL_LINE
    }

    std::string sfunc = simplify_function(function);

    bool log_to_console = true;
    if( log::get_log_handler() )
    {
        log_to_console = log::get_log_handler()->process( prio, category, sfunc.c_str(), msg );
    }

    if( log_to_console )
    {
        std::string logText;
        if( category == NULL )
        {
            logText = format( "%s {%s} <%s> %s%s\n", get_prio_header(prio), program_name.c_str(), sfunc.c_str(), msg, get_prio_end(prio) );
        }
        else
        {
            logText = format( "%s {%s:%s} <%s> %s%s\n", get_prio_header(prio), program_name.c_str(), category, sfunc.c_str(), msg, get_prio_end(prio) );
        }
#ifdef WIN32
        __OutputDebugString( logText.c_str() );
#else
        puts( logText.c_str() );
#endif
    }
}

void log::log_message( int prio, const char* category, const char* function, const char* format, ... )
{
    if( prio > g_priorityLimit )
    {
        return;
    }

    va_list args;
    va_start( args, format );
    std::string msg = vformat( format, args );
    va_end( args );

    do_log_msg( prio, category, function, msg.c_str() );
}

int ext::log::get_priority_limit() noexcept
{
    return g_priorityLimit;
}

void ext::log::set_priority_limit( int logPriorityLimit ) noexcept
{
    g_priorityLimit = logPriorityLimit;
}

const log_handler::XPtr & ext::log::get_log_handler() noexcept
{
    return g_logHandler;
}

void ext::log::set_log_handler( const log_handler::XPtr & userLogHandler ) noexcept
{
    g_logHandler = userLogHandler;
}

#ifndef UTIL_LOG_NO_TERMINATE_OVERRIDE

#ifndef UTIL_LOG_BACKTRACE_SIZE
#define UTIL_LOG_BACKTRACE_SIZE 50
#endif

#ifdef STACKTRACE_SUPPORTED
static std::string demangle(const char* line)
{
    std::string buf(line);
    int fPos = buf.rfind('(');
    int e2Pos = buf.rfind(')');
    int ePos = buf.rfind('+', e2Pos);

    size_t len = ePos-fPos-1;
    int status;
    std::string symbol = buf.substr(fPos+1, len);
    char* demangled = abi::__cxa_demangle( symbol.c_str(), NULL, NULL, &status );
    if( demangled != NULL && status == 0 )
    {
        return buf.substr(0, fPos+1) + demangled + buf.substr(ePos, std::string::npos);
    }
    else
    {
        return line;
    }
}
#endif

// LCOV_EXCL_START
static void _ex_terminate()
{
    static bool already_tried = false;

    log::set_log_handler(NULL);

    try
    {
        // try once to re-throw currently active exception
        if( !already_tried )
        {
            already_tried = true;
            throw;
        }
    }
    catch( const runtime_error &e )
    {
        e.log();
    }
    catch (const std::exception &e)
    {
        do_log_msg( LOG_PRIORITY_ERROR, NULL, typeid(e).name(), e.what() );
    }
    catch (...) {
        do_log_msg( LOG_PRIORITY_ERROR, NULL, "Unknown", "Unknown exception" );
    }

    fprintf( stderr, "Unhandled exception, program terminated\n" );

#if defined(STACKTRACE_SUPPORTED) && !defined(LOG_NO_BACKTRACE)

    fprintf( stderr, "Backtrace:\n" );

    void* fnAddr[UTIL_LOG_BACKTRACE_SIZE];
    int size = backtrace(fnAddr, UTIL_LOG_BACKTRACE_SIZE);

    char** fnSyms = backtrace_symbols(fnAddr, size);

    if( fnSyms != NULL )
    {
        // We skip the first call in the stack, which is the call to this
        // function
        for (int i=1; i < size; i++)
        {
            fprintf( stderr,"[%d]: %s\n", i, demangle(fnSyms[i]).c_str() );
        }

        free(fnSyms);
    }

#endif

#ifdef __GNUC__
    abort();
#else
    exit(-1);
#endif
}
// LCOV_EXCL_STOP

class _REInitializer
{
public:
    _REInitializer()
    {
        std::set_terminate( _ex_terminate );

#ifdef WIN32
        TCHAR module_name_cstr[MAX_PATH];

        GetModuleFileName( NULL, module_name_cstr, sizeof(module_name_cstr) );

        std::string module_name( module_name_cstr );

        size_t path_sep_pos = module_name.find_last_of( '\\' );

        if( path_sep_pos == std::string::npos )
        {
            program_name = module_name; // LCOV_EXCL_LINE
        }
        else
        {
            program_name = module_name.substr( path_sep_pos + 1 );
        }
#elif defined(__GNUC__)
        program_name = program_invocation_short_name;
#endif
    }
};

static _REInitializer reInit;

#endif // UTIL_LOG_NO_TERMINATE_OVERRIDE

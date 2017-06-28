/**
 * @file
 * @brief      Implementation of the error management related classes
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#include "ExtendedLib/runtime_error.h"
#include "ExtendedLib/format_string.h"

using namespace ext;

extern void do_log_msg( int prio, const char* category, const char* function, const char* msg );

runtime_error::runtime_error( const char* category, const char* function, bool log_on_throw, const std::string &msg )
: m_message( msg ), m_category( category ? category : "" ), m_function( function ), m_logged( log_on_throw )
{
    if( log_on_throw )
    {
        do_log_msg( LOG_PRIORITY_ERROR, category, function, what() );
    }
}

runtime_error::runtime_error( const char *category, const char *function, bool log_on_throw, const char *format, ... )
: m_category( category ? category : "" ), m_function( function ), m_logged( log_on_throw )
{
    va_list args;
    va_start( args, format );
    m_message = ext::vformat( format, args );
    va_end( args );

    if( log_on_throw )
    {
        do_log_msg( LOG_PRIORITY_ERROR, category, function, what() );
    }
}

void runtime_error::log() const
{
    if( !m_logged )
    {
        do_log_msg( LOG_PRIORITY_ERROR, m_category.empty() ? NULL : m_category.c_str(), m_function.c_str(), what() );
        m_logged = true;
    }
}

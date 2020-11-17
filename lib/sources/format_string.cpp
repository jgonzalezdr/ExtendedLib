/**
 * @file
 * @brief      Implementation of the format family of functions
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#include "Extended/format_string.hpp"

#include <stdio.h>
#include <stdarg.h>

#include "local_log.hpp"

#define _INITIAL_LENGTH    1024

std::string ext::vformat( const char *fmt, va_list ap )
{
    if ( !fmt ) return "";

    char *buffer = new char[_INITIAL_LENGTH];

    int n = vsnprintf( buffer, _INITIAL_LENGTH, fmt, ap );

    if( n >= _INITIAL_LENGTH )
    {
        // Didn't get enough space
        delete [] buffer;
        buffer = new char [n + 1];

        n = vsnprintf( buffer, n + 1, fmt, ap );
    }

    if( n < 0 ) return "";

    std::string ret( buffer );
    delete [] buffer;
    return ret;
}

std::string ext::format( const char *fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    std::string ret = ext::vformat( fmt, args );
    va_end( args );
    return ret;
}

std::string ext::format_hex( const std::vector<uint8_t>& data,
                             const std::string &indent,
                             const std::string &separator,
                             unsigned int bytes_per_line )
{
    std::string out;
    unsigned int i;

    for( i = 0; i < data.size(); i++ )
    {
        if( (i % bytes_per_line) == 0 )
        {
            if( i > 0 )
            {
                out += "\n";
            }
            out += indent;
        }
        else
        {
            out += separator;
        }
        out += ext::format( "%02X", (unsigned char)data[i] );
    }

    return out;
}

std::string ext::format_hex( const std::vector<uint8_t>& data,
                             unsigned int indent,
                             unsigned int separator,
                             unsigned int bytes_per_line )
{
    std::string sep_str( separator, ' '  );
    std::string idt_str( indent, ' ' );

    return ext::format_hex( data, idt_str, sep_str, bytes_per_line );
}

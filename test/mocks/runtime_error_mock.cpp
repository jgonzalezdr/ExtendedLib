/**
 * @file
 * @brief      Mocks for the error management related classes
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2017 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#include "ExtendedLib/runtime_error.h"

#include <CppUTestExt/MockSupport.h>

ext::runtime_error::runtime_error( const char *category, const char *function, bool log_on_throw, const char *format, ... )
: m_category( category ? category : "" ), m_function( function ), m_logged( log_on_throw )
{
    mock().actualCall("ext::runtime_error::runtime_error").withParameter("category", category).withParameter("function", function)
          .withParameter("log_on_throw", log_on_throw).withParameter("format", format);
}

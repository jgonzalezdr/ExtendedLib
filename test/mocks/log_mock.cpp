/**
 * @file
 * @brief      Mocks for the log management related classes and macros
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2017 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#include "Extended/log.hpp"

#include <CppUTestExt/MockSupport.h>

void ext::log::log_message( int prio, const char* category, const char* function, const char* format, ... )
{
    mock().actualCall("ext::log::log_message").withParameter("prio", prio).withParameter("category", category)
          .withParameter("function", function).withParameter("format", format);
}

/**
 * @file
 * @brief      Implementation for MSW of the 'thread' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#include "ExtendedLib/thread.h"
#include <windows.h>
#include "../local_log.h"
#include "ExtendedLib/runtime_error.h"
#include "Helpers.h"

using namespace ext;

static int GetMSWPriority( thread::ThreadPriority priority )
{
    switch( priority )
    {
        case thread::PRIORITY_LOWEST:
            return THREAD_PRIORITY_IDLE;
        case thread::PRIORITY_LOWER:
            return THREAD_PRIORITY_LOWEST;
        case thread::PRIORITY_LOW:
            return THREAD_PRIORITY_BELOW_NORMAL;
        case thread::PRIORITY_HIGH:
            return THREAD_PRIORITY_ABOVE_NORMAL;
        case thread::PRIORITY_HIGHER:
            return THREAD_PRIORITY_HIGHEST;
        case thread::PRIORITY_HIGHEST:
            return THREAD_PRIORITY_TIME_CRITICAL;
        default:
            return THREAD_PRIORITY_NORMAL;
    }
}

void thread::set_priority( ThreadPriority priority )
{
    if( !SetThreadPriority( native_handle(), GetMSWPriority( priority ) ) )
    {
        THROW_ERROR( "Couldn't set thread priority (Error = %s)", GetLastErrorAsString().c_str() );
    }
}

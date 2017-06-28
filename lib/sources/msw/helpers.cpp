/**
 * @file
 * @brief      Implementation of MSW helper functions
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#include "helpers.h"
#include <Windows.h>

std::string GetLastErrorAsString()
{
    DWORD lastErrorNum = ::GetLastError();
    if( lastErrorNum == 0 )
    {
        return std::string(); //No error message has been recorded
    }

    LPSTR lastErrorBuffer = NULL;
    size_t lastErrorSize = ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                            NULL, lastErrorNum, 0, (LPSTR) &lastErrorBuffer, 0, NULL );

    std::string message( lastErrorBuffer, lastErrorSize );

    LocalFree( lastErrorBuffer );

    return message;
}

/**
 * @file
 * @brief      Mocks for the win32 OS calls
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2017 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#include <Windows.h>

#include <CppUTestExt/MockSupport.h>

extern "C" {

void WINAPI OutputDebugStringUT( _In_opt_ LPCTSTR lpOutputString )
{
    mock().actualCall( "::OutputDebugString" ).withParameter( "lpOutputString", lpOutputString );
}

}

/**
 * @file
 * @brief      Platform selection header for the 'callback_dispatcher' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#ifndef EXTENDEDLIB_callback_dispatcher_h__
#define EXTENDEDLIB_callback_dispatcher_h__

#ifdef WIN32
#include "ExtendedLib/msw/callback_dispatcher.h"
#else
#error "Platform unknown or not supported"
#endif

#endif // header guard

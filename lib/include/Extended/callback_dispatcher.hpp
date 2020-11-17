/**
 * @file
 * @brief      Platform selection header for the 'callback_dispatcher' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_callback_dispatcher_hpp_
#define Extended_callback_dispatcher_hpp_

#ifdef WIN32
#include "msw/callback_dispatcher.hpp"
#else
#error "Platform unknown or not supported"
#endif

#endif // header guard

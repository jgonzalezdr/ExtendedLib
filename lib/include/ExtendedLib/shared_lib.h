/**
 * @file
 * @brief      Shared library exportation definitions
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#ifndef EXTENDEDLIB_shared_lib_h__
#define EXTENDEDLIB_shared_lib_h__

#if defined (WIN32) && defined(EXTENDEDLIB_BUILD_SHARED_LIB)
#define EXTENDEDLIB_EXPORT __declspec(dllexport)
#elif defined (WIN32) && defined (EXTENDEDLIB_USE_SHARED_LIB)
#define EXTENDEDLIB_EXPORT __declspec(dllimport)
#else
#define EXTENDEDLIB_EXPORT
#endif

#endif // header guard

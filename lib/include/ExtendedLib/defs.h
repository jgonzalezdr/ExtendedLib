/**
 * @file
 * @brief      Useful macros
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#ifndef EXTENDEDLIB_extended_defs_h__
#define EXTENDEDLIB_extended_defs_h__

/**
 * Calculates the number of elements of an array.
 *
 * @param[in] x Array to calculate size
 */
#define SIZEOF_ARRAY(x) ( sizeof(x) / sizeof(x[0]) )

#endif // header guard

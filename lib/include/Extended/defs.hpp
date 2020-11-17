/**
 * @file
 * @brief      Useful macros
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_defs_hpp_
#define Extended_defs_hpp_

/**
 * Calculates the number of elements of an array.
 *
 * @param[in] x Array to calculate size
 */
#define SIZEOF_ARRAY(x) ( sizeof(x) / sizeof(x[0]) )

#endif // header guard

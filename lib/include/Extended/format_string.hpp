/**
 * @file
 * @brief      Header for the format family of functions
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_FormastString_hpp_
#define Extended_FormastString_hpp_

#include "extended_config.hpp"
#include <stdarg.h>
#include <string>
#include <vector>
#include <stdint.h>

///@cond INTERNAL
#ifdef __GNUC__
#define ATTR_PRINTF_1_0 __attribute__((format(printf, 1, 0)))
#define ATTR_PRINTF_1_2 __attribute__((format(printf, 1, 2)))
#else
#define ATTR_PRINTF_1_0
#define ATTR_PRINTF_1_2
#endif
///@endcond

namespace ext
{

///@defgroup format_string Format String
///@{

/**
 * Returns a std::string formatted according to @p fmt using the variable arguments list @p va.
 *
 * @param[in] fmt Format string (using printf format)
 * @param[in] ap Variable arguments list
 */
Extended_API std::string vformat( const char *fmt, va_list ap ) ATTR_PRINTF_1_0;

/**
 * Returns a std::string formatted according to @p fmt using the variable arguments passed to the function.
 *
 * @param[in] fmt Format string (using printf format)
 * @param[in] ... Variable parameters for the format string
 */
Extended_API std::string format( const char *fmt, ... ) ATTR_PRINTF_1_2;

/**
 * Returns a std::string with the printable hexadecimal representation of byte array contained in @p data.
 *
 * The output string is splitted in different lines according to @p bytes_per_line by inserting line feed ('\n') characters.
 *
 * @param[in] data Array of bytes to be formatted
 * @param[in] indent String to insert at the beginning of lines
 * @param[in] separator String to insert between each byte hexadecimal representation
 * @param[in] bytes_per_line Maximum number of bytes to be printed per line
 */
Extended_API std::string format_hex( const std::vector<uint8_t>& data, const std::string &indent, const std::string &separator,
                                           unsigned int bytes_per_line = 16 );

/**
 * Returns a std::string with the printable hexadecimal representation of byte array contained in @p data.
 *
 * The output string is splitted in different lines according to @p bytes_per_line by inserting line feed ('\n') characters.
 *
 * @param[in] data Array of bytes to be formatted
 * @param[in] indent Number of spaces to insert at the beginning of lines
 * @param[in] separator Number of spaces to insert between each byte hexadecimal representation
 * @param[in] bytes_per_line Maximum number of bytes to be printed per line
 * @return
 */
Extended_API std::string format_hex( const std::vector<uint8_t>& data, unsigned int indent = 0, unsigned int separator = 1,
                                           unsigned int bytes_per_line = 16 );

///@}

} // namespace

#endif // header guard

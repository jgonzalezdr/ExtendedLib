/**
 * @file
 * @brief      Header for the 'byte_vector' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_byte_vector_hpp_
#define Extended_byte_vector_hpp_

#include <vector>
#include <stdint.h>

#ifndef byte
/**
 * Type that represents an 8-bit number.
 */
typedef uint8_t byte;
///@cond INTERNAL
#define byte byte ///< Identity macro to avoid redefining byte
///@endcond
#endif

namespace ext
{

/**
 * Specialization of std::vector that manages a sequence of bytes.
 */
class byte_vector : public std::vector<byte>
{
public:
    using std::vector<byte>::vector;

    /**
     * Constructs an empty container, with no elements.
     */
    byte_vector() : vector() {}

    /**
     * Constructs a container with @a dataLen elements, which are copied from @a data.
     */
    byte_vector( const byte* data, size_type dataLen )
        : vector( data, data + dataLen ) {}
};

} // namespace

#endif // header guard

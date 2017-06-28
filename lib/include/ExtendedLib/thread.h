/**
 * @file
 * @brief      Header for the 'thread' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#ifndef EXTENDEDLIB_thread_h__
#define EXTENDEDLIB_thread_h__

#include "ExtendedLib/shared_lib.h"
#include <thread>

namespace ext
{

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4275 )
#endif

/**
 * Class that adds extended features to std::thread.
 */
class EXTENDEDLIB_EXPORT thread : public std::thread
{
public:
    using std::thread::thread;

    thread(const thread&) = delete;
    thread& operator=(const thread&) = delete;

    /**
     * Priority of the thread.
     */
    enum ThreadPriority
    {
        PRIORITY_LOWEST, //!< Lowest priority
        PRIORITY_LOWER,  //!< Lower priority
        PRIORITY_LOW,    //!< Low priority
        PRIORITY_NORMAL, //!< Normal priority
        PRIORITY_HIGH,   //!< High priority
        PRIORITY_HIGHER, //!< Higher priority
        PRIORITY_HIGHEST //!< Highest priority
    };

    /**
     * Sets the priority of the thread.
     *
     * @param[in] priority Priority of the thread
     */
    void set_priority( ThreadPriority priority );
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

} // namespace

#endif // header guard

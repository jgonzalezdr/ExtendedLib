/**
 * @file
 * @brief      Header for the 'callback' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#ifndef EXTENDEDLIB_callback_h__
#define EXTENDEDLIB_callback_h__

#include <string>
#include <utility>
#include "ExtendedLib/shared_lib.h"
#include "ExtendedLib/memory.h"

namespace ext
{

/**
 * The callback template class is used to encapsulate a function or method that
 * can be later invoked (called).
 *
 * @tparam ParamType Type of the parameter accepted by the callback
 */
template <typename ParamType>
class callback
{
public:
    DECLARE_SHAREDPTR( callback )

    /* Destructor */
    virtual ~callback()
    {}

    /**
     * Invokes the callback with the given parameter.
     *
     * @param[in] param Parameter to the callback
     */
    void operator()( ParamType param ) const
    {
        invoke( param );
    }

    /**
     * Invokes the callback with the given parameter.
     *
     * @param[in] param Parameter to the callback
     */
    virtual void invoke( ParamType param ) const = 0;
};

/**
 * The method_callback template class is a specialization of the callback class that encapsulates an instance method that
 * can be later invoked (called) on a specific object.
 *
 * @tparam ObjectClass Class of the object which method will be invoked
 * @tparam ParamType Type of the parameter accepted by the callback
 */
template <typename ObjectClass, typename ParamType>
class method_callback : public callback<ParamType>
{
public:
    /**
     * Creates a new callback that will invoke the given instance @a method on the given @a object.
     *
     * @param[in] object Object on which the method will be invoked
     * @param[in] method Instance method to be invoked on the object
     */
    method_callback( ObjectClass *object, void ( ObjectClass::*method )( ParamType ) ) noexcept
        : m_object( object ), m_method( method )
    {}

    /* Destructor */
    virtual ~method_callback()
    {}

    virtual void invoke( ParamType param ) const override
    {
        ( m_object->*m_method )( param );
    }

private:
    ObjectClass *m_object;
    void ( ObjectClass::*m_method )( ParamType );
};

///@defgroup callbacks Callbacks
///@{

/**
 * Creates a new callback that will invoke the given instance @a method on the given @a object.
 *
 * @tparam ObjectClass Class of the object which method will be invoked
 * @tparam ParamType Type of the parameter accepted by the callback
 * @param[in] object Object on which the method will be invoked
 * @param[in] method Instance method to be invoked on the object
 * @return A new callback instance
 */
template <typename ObjectClass, typename ParamType>
shared_ptr<callback<ParamType>> new_callback( ObjectClass *object, void ( ObjectClass::*method )( ParamType ) ) noexcept
{
    return new method_callback<ObjectClass, ParamType>( object, method );
}

///@}

} // namespace

#endif // header guard

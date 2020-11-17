/**
 * @file
 * @brief      Header for the 'dispatched_method_callback' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_dispatched_callback_hpp_
#define Extended_dispatched_callback_hpp_

#include "callback_dispatcher.hpp"
#include "callback.hpp"

namespace ext
{

/**
 * The dispatched_method_callback template class is a specialization of the callback class that encapsulates an instance method that
 * can be later invoked (called) on a specific object via a dispatched message.
 *
 * Dispatched callbacks are useful to execute the callback in the GUI thread instead of the invoker's thread.
 *
 * @note Invocation of a dispatched callback returns immediately and the callback will be executed later asynchronously in the GUI thread.
 *
 * @note Dispatched callbacks have the restriction that the callback parameter must be a shared_ptr reference in order to guarantee that
 * the passed object is still valid when the callback execution occurs.
 *
 * @tparam ObjectClass Class of the object which method will be invoked
 * @tparam ParamType Type of the parameter accepted by the callback
 *
 * @see message_dispatcher
 */
template <typename ObjectClass, typename ParamType>
class dispatched_method_callback : public callback<const ext::shared_ptr<ParamType>&>
{
public:
    /**
     * Creates a new dispatched callback that will invoke the given instance @a method on the given @a object.
     *
     * @param[in] object Object on which the method will be invoked
     * @param[in] method Instance method to be invoked on the object
     * @param[in] dispatcher Message dispatcher to be used when invoking the callback
     */
    dispatched_method_callback( ObjectClass *object, void ( ObjectClass::*method )( const ext::shared_ptr<ParamType>& ),
                                const callback_dispatcher& dispatcher = callback_dispatcher::get_instance() ) noexcept
        : m_finalCallback( new_callback( object, method ) ), m_dispatcher( dispatcher )
    {}

    virtual ~dispatched_method_callback()
    {}

    virtual void invoke( const ext::shared_ptr<ParamType> &param ) const override
    {
        m_dispatcher.dispatch( m_finalCallback, param );
    }

private:
    typename callback< const ext::shared_ptr<ParamType>& >::XPtrConst m_finalCallback;
    const callback_dispatcher& m_dispatcher;
};

///@defgroup callbacks Callbacks
///@{

/**
 * Creates a new dispatched callback that will invoke the given instance @a method on the given @a object.
 *
 * @tparam ObjectClass Class of the object which method will be invoked
 * @tparam ParamType Type of the parameter accepted by the callback
 * @param[in] object Object on which the method will be invoked
 * @param[in] method Instance method to be invoked on the object
 * @param[in] dispatcher Message dispatcher to be used when invoking the callback
 * @return A new dispatched callback instance
 */
template <typename ObjectClass, typename ParamType>
shared_ptr<callback< const ext::shared_ptr<ParamType>& >>
new_dispatched_callback( ObjectClass *object, void ( ObjectClass::*method )( const ext::shared_ptr<ParamType>& ),
                        const callback_dispatcher& dispatcher = callback_dispatcher::get_instance() ) noexcept
{
    return new dispatched_method_callback<ObjectClass, ParamType>( object, method, dispatcher );
}

///@}

} // namespace

#endif // header guard

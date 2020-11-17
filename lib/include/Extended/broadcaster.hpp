/**
 * @file
 * @brief      Header for the 'broadcaster' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_broadcaster_hpp_
#define Extended_broadcaster_hpp_

#include "extended_config.hpp"
#include "callback.hpp"
#include <map>
#include <mutex>

namespace ext
{

/**
 * The broadcaster template class is used to broadcast event messages to any listener that has registered to receive them.
 *
 * An event message can be an object of any type, even primitive types (char, int, etc.).
 *
 * Listeners have to be callbacks that will be called when an event message is broadcast.
 *
 * @tparam MessageType Type of event messages
 */
template<typename MessageType>
class broadcaster
{
public:
    /**
     * Type of the listeners.
     */
    typedef callback<MessageType> broadcast_handler_t;

    /**
     * Broadcasts the @a message to any registered listeners.
     *
     * @param[in] message Contents of the message to be broadcast
     */
    void broadcast( MessageType message ) const
    {
        LockGuard lockGuard( m_mutex );
        typename listeners_list_t::const_iterator it;
        for( it = m_listeners.begin(); it != m_listeners.end(); )
        {
            const ext::shared_ptr<const broadcast_handler_t> &callback = it->second;
            // Increment is done here to avoid problems if the callback associated to this iterator is
            // erased inside the callback
            it++;
            callback->invoke( message );
        }
    }

    /**
     * Registers a new @a listener for broadcast messages.
     *
     * The object address of the @a listener callback (contained in the smart pointer) will be used to uniquely identify the listener,
     * and therefore the same callback instance will have to be passed to remove_listener() to unregister it from the broadcast.
     *
     * @param[in] listener Listener to be registered
     * @retval true if the listener was registered (i.e. it wasn't previously registered)
     * @retval false otherwise
     */
    bool add_listener( const ext::shared_ptr<const broadcast_handler_t> &listener )
    {
        return add_listener( listener, (void*) listener.get() );
    }

    /**
     * Unregisters a new @a listener for broadcast messages.
     *
     * The @a listener callback instance (contained in the smart pointer) shall be the same that was passed to add_listener().
     *
     * @param[in] listener Listener to be unregistered
     * @retval true if the listener was unregistered (i.e. it was previously registered)
     * @retval false otherwise
     */
    bool remove_listener( const ext::shared_ptr<const broadcast_handler_t> &listener )
    {
        return remove_listener( (void*) listener.get() );
    }

    /**
     * Registers a new @a listener for broadcast messages.
     *
     * The @a idKey identifier shall uniquely identify the listener, and will have to be passed to remove_listener() to
     * unregister the listener from the broadcast.
     *
     * @param[in] listener Listener to be registered
     * @param[in] idKey Identifier for the listener
     * @retval true if the listener was registered (i.e. it wasn't previously registered with the same identifier)
     * @retval false otherwise
     */
    bool add_listener( const ext::shared_ptr<const broadcast_handler_t> &listener, void* idKey )
    {
        LockGuard lockGuard( m_mutex );
        std::pair<typename listeners_list_t::iterator, bool> ret = m_listeners.insert( typename listeners_list_t::value_type( idKey, listener ) );
        return ret.second;
    }

    /**
     * Unregisters a new @a listener for broadcast messages.
     *
     * The @a idKey identifier shall be the same idenfier passed to add_listener().
     *
     * @param[in] idKey Identifier for the listener
     * @retval true if the listener was unregistered (i.e. it was previously registered)
     * @retval false otherwise
     */
    bool remove_listener( void* idKey )
    {
        LockGuard lockGuard( m_mutex );
        typename listeners_list_t::size_type ret = m_listeners.erase( idKey );
        return ( ret > 0 );
    }

    /**
     * Indicates whether listeners are registered.
     *
     * @retval true if listeners are registered
     * @retval false otherwise
     */
    bool has_listeners() const noexcept
    {
        return !m_listeners.empty();
    }

    /**
     * Returns the number of listener that are registered.
     */
    unsigned int listeners_count() const noexcept
    {
        return m_listeners.size();
    }

private:
    typedef std::map<void*, ext::shared_ptr<const broadcast_handler_t>> listeners_list_t;
    listeners_list_t m_listeners;

    mutable std::recursive_mutex m_mutex;
    typedef std::lock_guard<std::recursive_mutex> LockGuard;
};

} // namespace

#endif // header guard

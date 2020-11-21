/**
 * @file
 * @brief      Header for the 'callback_dispatcher' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2003-2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_MSW_callback_dispatcher_hpp_
#define Extended_MSW_callback_dispatcher_hpp_

#include "extended_config.hpp"
#include "../callback.hpp"
#include <windows.h>

/**
 * Default message identifier used by ext::callback_dispatcher to post to its associated window.
 */
#define Extended_DEFAULT_WM_EVENT ( WM_USER + 13500 )

namespace ext
{

/**
 * The callback_dispatcher class handles invoking a callback in a thread but asynchronously executing
 * it later in the GUI thread.
 */
class Extended_API callback_dispatcher
{
public:
    /**
     * Creates a new callback dispatcher.
     *
     * @note The shared callback dispatcher returned by get_instance() will be useful in most cases and
     * a specific callback dispatcher will not be necessary.
     */
    callback_dispatcher() noexcept;

    /**
     * Returns the shared callback dispatcher instance.
     */
    static callback_dispatcher& get_instance() noexcept;

    /**
     * Returns the handle for the window associated with the callback dispatcher.
     */
    HANDLE get_window_handle() noexcept;

    /**
     * Associates a window to the callback dispatcher.
     *
     * The messages used to execute the callback will be posted to the associated window.
     *
     * Before dispatching any callback, a window shall be associated to the callback dispatcher.
     *
     * @note Normally the application's top level window must be associated to the callback dispatcher. The association
     * shall normally be performed during the main/event loop initialization or during the main window initialization.
     *
     * @param[in] hwnd Handle to the window to associate
     */
    void set_window_handle( HWND hwnd ) noexcept;

    /**
     * Returns the message identifier used to post to the associated window.
     */
    UINT get_message_id() noexcept;

    /**
     * Sets the message identifier used to post to the associated window.
     *
     * @note The message identifier may have to be changed only when the default identifier is already in use for other purposes.
     *
     * @param[in] messageId Message identifier
     */
    void set_message_id( UINT messageId ) noexcept;

    ///@cond INTERNAL

    typedef callback<const std::shared_ptr<int> &> generic_callback_t;

    /**
     * Invokes the asynchronous execution of the given callback with the given parameter on the GUI thread.
     *
     * @note This method is not intended to be used directly by users. Users shall use its homonymous template method.
     *
     * @param[in] callback Callback to be invoked
     * @param[in] param Parameter to be passed to the callback when invoking it
     */
    void dispatch( const std::shared_ptr<const generic_callback_t> &callback,
                   const std::shared_ptr<int> &param ) const;

    ///@endcond INTERNAL

    /**
     * Invokes the asynchronous execution of the given callback with the given parameter on the GUI thread.
     *
     * @param[in] callback Callback to be invoked
     * @param[in] param Parameter to be passed to the callback when invoking it
     * @tparam ParamType Type of the callback parameter
     */
    template<typename ParamType>
    void dispatch( const std::shared_ptr<const callback<const std::shared_ptr<ParamType> &>> &callback,
                   const std::shared_ptr<ParamType> &param ) const
    {
        dispatch( reinterpret_cast<const std::shared_ptr<const generic_callback_t> &>( callback ),
                  reinterpret_cast<const std::shared_ptr<int> &>( param ) );
    }

    /**
     * Processes a Win32 message.
     *
     * @note This method must be invoked from the application's main/event loop for each message received by the
     * associated window.
     *
     * @param[in] nMsg Message identifier
     * @param[in] wParam Message 1st parameter
     * @param[in] lParam Message 2nd parameter
     */
    void process_platform_message( UINT nMsg, WPARAM wParam, LPARAM lParam ) const;

private:
    HWND m_windowHandle;
    UINT m_messageId;
};

} // namespace

#endif // header guard

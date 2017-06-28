/**
 * @file
 * @brief      Implementation for MSW of the 'callback_dispatcher' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    BSD License (See LICENSE.txt)
 */

#include "ExtendedLib/msw/callback_dispatcher.h"
#include "../local_log.h"
#include "ExtendedLib/runtime_error.h"
#include "Helpers.h"

using namespace ext;

#ifndef UNIT_TEST
#define __PostMessage PostMessage
#else
extern "C" {
WINBOOL WINAPI PostMessageUT( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
}
#define __PostMessage PostMessageUT
#endif

callback_dispatcher::callback_dispatcher() noexcept
    : m_windowHandle( NULL ), m_messageId( EXTENDEDLIB_DEFAULT_WM_EVENT )
{
}

callback_dispatcher& callback_dispatcher::get_instance() noexcept
{
    static callback_dispatcher instance;
    return instance;
}

HANDLE callback_dispatcher::get_window_handle() noexcept
{
    return m_windowHandle;
}

void callback_dispatcher::set_window_handle( HWND windowHandle ) noexcept
{
    m_windowHandle = windowHandle;
}

UINT callback_dispatcher::get_message_id() noexcept
{
    return m_messageId;
}

void callback_dispatcher::set_message_id( UINT messageId ) noexcept
{
    m_messageId = messageId;
}

void callback_dispatcher::dispatch( const ext::shared_ptr<const generic_callback_t> &callback,
                                   const ext::shared_ptr<int> &param ) const
{
    // A new weak pointer to the callback is created to allow it being destroyed while the windows message is being delivered,
    // however a new shared pointer to the parameter is created to avoid it being deleted in that circumstance
    std::weak_ptr< const generic_callback_t > *wk_callback = new std::weak_ptr< const generic_callback_t >( callback );
    ext::shared_ptr<int> *sh_param = new ext::shared_ptr<int>( param );
    if( __PostMessage( m_windowHandle, m_messageId, (WPARAM) wk_callback, (LPARAM) sh_param ) == 0 )
    {
        delete wk_callback;
        delete sh_param;
        THROW_ERROR( "Couldn't post message %u to window %p (Error = %s)", m_messageId, m_windowHandle, GetLastErrorAsString().c_str() );
    }
}

void callback_dispatcher::process_platform_message( UINT nMsg, WPARAM wParam, LPARAM lParam ) const
{
    if( nMsg == m_messageId )
    {
        std::weak_ptr< const generic_callback_t > *wk_callback = (std::weak_ptr< const generic_callback_t > *) wParam;
        ext::shared_ptr<int> *param = (ext::shared_ptr<int>*) lParam;
        // If the weak pointer that was created when the message was dispatched is not valid then it's assumed that the callback
        // is not able to handle events any more.
        ext::shared_ptr< const generic_callback_t > sh_callback = wk_callback->lock();
        if( sh_callback )
        {
            sh_callback->invoke( *param );
        }
        delete param;
        delete wk_callback;
    }
}

/**
 * @file
 * @brief      unit tests for the "callback_dispatcher" class
 * @project    ExtendedLib
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) Jesus Gonzalez. All rights reserved.
 * @license    See LICENSE.txt
 */

/*===========================================================================
 *                              INCLUDES
 *===========================================================================*/

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <ExtendedLib/msw/callback_dispatcher.h>
#include <ExtendedLib/runtime_error.h>

/*===========================================================================
 *                      COMMON TEST DEFINES & MACROS
 *===========================================================================*/

class TestClass
{
public:
    void TestCallback( const ext::shared_ptr<int> &p )
    {
        mock().actualCall( "TestClass::TestCallback" ).onObject( this ).withParameter( "p", *p );
    }
};

extern "C" {

WPARAM PostMessageUT_wParam;
LPARAM PostMessageUT_lParam;

BOOL WINAPI PostMessageUT( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
   PostMessageUT_wParam = wParam;
   PostMessageUT_lParam = lParam;
    return mock().actualCall( "::PostMessage" ).withParameter( "hWnd", hWnd ).withParameter( "Msg", Msg ).returnBoolValue();
}

}

/*===========================================================================
 *                          TEST GROUP DEFINITION
 *===========================================================================*/

TEST_GROUP( callback_dispatcher_msw )
{
};

/*===========================================================================
 *                    TEST CASES IMPLEMENTATION
 *===========================================================================*/

/*
 * Check that the callback_dispatcher is created properly.
 */
TEST( callback_dispatcher_msw, Creation )
{
    // Prepare

    // Exercise
    ext::callback_dispatcher callback_dispatcher;

    // Verify
    CHECK_EQUAL( (HANDLE)NULL, callback_dispatcher.get_window_handle() );
    CHECK_EQUAL( EXTENDEDLIB_DEFAULT_WM_EVENT, callback_dispatcher.get_message_id() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that the shared callback_dispatcher behaves properly.
 */
TEST( callback_dispatcher_msw, SharedInstance )
{
    // Prepare

    // Exercise
    ext::callback_dispatcher& callback_dispatcher = ext::callback_dispatcher::get_instance();

    // Verify
    CHECK_EQUAL( (HANDLE)NULL, callback_dispatcher.get_window_handle() );
    CHECK_EQUAL( EXTENDEDLIB_DEFAULT_WM_EVENT, callback_dispatcher.get_message_id() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that the window handle is set properly.
 */
TEST( callback_dispatcher_msw, SetWindowHandle )
{
    // Prepare
    ext::callback_dispatcher callback_dispatcher;
    const HWND hwnd = (HWND) 1234;

    // Exercise
    callback_dispatcher.set_window_handle( hwnd );

    // Verify
    CHECK_EQUAL( (HANDLE) hwnd, callback_dispatcher.get_window_handle() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that the message id is set properly.
 */
TEST( callback_dispatcher_msw, MessageId )
{
    // Prepare
    ext::callback_dispatcher callback_dispatcher;
    const UINT msgId = 8454;

    // Exercise
    callback_dispatcher.set_message_id( msgId );

    // Verify
    CHECK_EQUAL( msgId, callback_dispatcher.get_message_id() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that invocation works properly (both phases).
 */
TEST( callback_dispatcher_msw, Invocation_OK )
{
    // Prepare
    const HWND hwnd = (HWND) 1234;
    const UINT msgId = 8454;
    TestClass obj;
    int *paramValue = new int( 9876543 );

    ext::callback_dispatcher callback_dispatcher;
    callback_dispatcher.set_window_handle( hwnd );
    callback_dispatcher.set_message_id( msgId );

    ext::callback<const ext::shared_ptr<int> &>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );
    std::shared_ptr<int> paramSharedPtr( paramValue );

    mock().expectOneCall( "::PostMessage" ).withParameter( "hWnd", hwnd ).withParameter( "Msg", msgId ).andReturnValue( true );

    // Exercise
    callback_dispatcher.dispatch( callback, paramSharedPtr );

    // Verify
    mock().checkExpectations();

    // Prepare
    mock().expectOneCall("TestClass::TestCallback").onObject( &obj ).withParameter( "p", *paramValue );

    // Exercise
    callback_dispatcher.process_platform_message( msgId, PostMessageUT_wParam, PostMessageUT_lParam );

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check failure to post message during invocation.
 */
TEST( callback_dispatcher_msw, Invocation_ErrorPosting )
{
    // Prepare
    const HWND hwnd = (HWND) 1234;
    const UINT msgId = 8454;
    TestClass obj;
    int *paramValue = new int( 9876543 );

    ext::callback_dispatcher callback_dispatcher;
    callback_dispatcher.set_window_handle( hwnd );
    callback_dispatcher.set_message_id( msgId );

    ext::callback<const ext::shared_ptr<int> &>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );
    std::shared_ptr<int> paramSharedPtr( paramValue );

    mock().expectOneCall( "::PostMessage" ).withParameter( "hWnd", hwnd ).withParameter( "Msg", msgId ).andReturnValue( false );
    mock().expectOneCall("ext::runtime_error::runtime_error").ignoreOtherParameters();

    // Exercise
    CHECK_THROWS( ext::runtime_error, callback_dispatcher.dispatch( callback, paramSharedPtr ) );

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that the messages for other MsgIds are ignored.
 */
TEST( callback_dispatcher_msw, MessageProcessing_OtherMsgId )
{
    // Prepare
    const HWND hwnd = (HWND) 1234;
    const UINT msgId = 7669;

    ext::callback_dispatcher callback_dispatcher;
    callback_dispatcher.set_window_handle( hwnd );
    callback_dispatcher.set_message_id( msgId );

    // Exercise
    callback_dispatcher.process_platform_message( msgId+1, 0, 0 );

    // Verify
    mock().checkExpectations();

    // Prepare

    // Exercise
    callback_dispatcher.process_platform_message( msgId-1, 0, 0 );

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that the callback is not invoked if it's not valid any more.
 */
TEST( callback_dispatcher_msw, MessageProcessing_CallbackNotValid )
{
    // Prepare
    const HWND hwnd = (HWND) 1234;
    const UINT msgId = 7669;
    TestClass obj;
    int *paramValue = new int( 9876543 );

    ext::callback_dispatcher callback_dispatcher;
    callback_dispatcher.set_window_handle( hwnd );
    callback_dispatcher.set_message_id( msgId );

    ext::callback<const ext::shared_ptr<int> &>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );
    std::shared_ptr<int> paramSharedPtr( paramValue );

    // The following variables
    std::weak_ptr< const ext::callback_dispatcher::generic_callback_t > *wk_callback = new std::weak_ptr< const ext::callback_dispatcher::generic_callback_t >( callback );
    ext::shared_ptr<int> *sh_param = new ext::shared_ptr<int>( paramSharedPtr );

    callback.reset();

    // Exercise
    callback_dispatcher.process_platform_message( msgId, (WPARAM) wk_callback, (LPARAM) sh_param );

    // Verify
    mock().checkExpectations();

    // Cleanup
}


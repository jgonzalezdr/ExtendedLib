/**
 * @file
 * @brief      unit tests for the "dispatched_callback" class
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

#include "Extended/dispatched_callback.hpp"

/*===========================================================================
 *                      COMMON TEST DEFINES & MACROS
 *===========================================================================*/

class TestClass
{
public:
    void TestCallback( const ext::shared_ptr< int > &p )
    {
        mock().actualCall( "TestClass::TestCallback" ).onObject( this ).withParameter( "p", *p );
    }
};

// message_dispatcher& ext::message_dispatcher::get_instance() noexcept
// {
//     *( (message_dispatcher*) mock().actualCall("ext::message_dispatcher::get_instance").returnPointerValue() );
// }

 ext::callback_dispatcher::callback_dispatcher() noexcept
 {
 }

 std::weak_ptr<const ext::callback_dispatcher::generic_callback_t> g_dispatch_callback;

 void ext::callback_dispatcher::dispatch( const ext::shared_ptr<const generic_callback_t> &callback,
                                        const ext::shared_ptr<int> &param ) const
 {
    mock().actualCall("ext::message_dispatcher::dispatch").onObject(this).withParameter("param", *param);
    g_dispatch_callback = callback;
 }

 ext::callback_dispatcher g_msg_dispatcher;

/*===========================================================================
 *                          TEST GROUP DEFINITION
 *===========================================================================*/

TEST_GROUP( method_dispatched_callback )
{
};

/*===========================================================================
 *                    TEST CASES IMPLEMENTATION
 *===========================================================================*/

/*
 * Check that the callback is created properly and the target method is not invoked.
 */
TEST( method_dispatched_callback, Creation )
{
    // Prepare
    TestClass obj;

    // Exercise
    ext::callback<const ext::shared_ptr<int> &>::XPtr callback = ext::new_dispatched_callback( &obj, &TestClass::TestCallback, g_msg_dispatcher );

    // Verify
    CHECK_TRUE( callback );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check proper invocation of the target method
 */
TEST( method_dispatched_callback, Invocation )
{
    // Prepare
    const int value = 98976;
    g_dispatch_callback.reset();
    ext::shared_ptr<int> param( new int(value) );
    TestClass obj;
    ext::callback<const ext::shared_ptr<int> &>::XPtr callback = ext::new_dispatched_callback( &obj, &TestClass::TestCallback, g_msg_dispatcher );
    mock().expectOneCall("ext::message_dispatcher::dispatch").onObject( &g_msg_dispatcher ).withParameter( "param", value );

    // Exercise
    (*callback)( param );

    // Verify
    mock().checkExpectations();
    CHECK_FALSE( g_dispatch_callback.expired() );

    // Prepare
    mock().expectOneCall("TestClass::TestCallback").onObject(&obj).withParameter( "p", value );

    // Exercise
    (*g_dispatch_callback.lock())( param );

    // Verify
    mock().checkExpectations();

    // Cleanup
    g_dispatch_callback.reset();
}

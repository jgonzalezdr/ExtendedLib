/**
 * @file
 * @brief      unit tests for the "broadcaster" class
 * @project    ExtendedLib
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) Jesus Gonzalez. All rights reserved.
 * @license    See LICENSE.txt
 */

/*===========================================================================
 *                              INCLUDES
 *===========================================================================*/

#include "ExtendedLib/broadcaster.h"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*===========================================================================
 *                      COMMON TEST DEFINES & MACROS
 *===========================================================================*/

class TestClass
{
public:
    void TestCallback( int p )
    {
        mock().actualCall( "TestClass::TestCallback" ).onObject( this ).withParameter( "p", p );
    }
};

/*===========================================================================
 *                          TEST GROUP DEFINITION
 *===========================================================================*/

TEST_GROUP( broadcaster )
{
};

/*===========================================================================
 *                    TEST CASES IMPLEMENTATION
 *===========================================================================*/

/*
 * Check that the broadcaster is created properly without listeners.
 */
TEST( broadcaster, Creation )
{
    // Prepare

    // Exercise
    ext::broadcaster<int> broadcaster;

    // Verify
    CHECK_FALSE( broadcaster.has_listeners() );
    CHECK_EQUAL( 0, broadcaster.listeners_count() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that a listener identified by its own address is added properly.
 */
TEST( broadcaster, AddListener_SelfId )
{
    // Prepare
    ext::broadcaster<int> broadcaster;
    TestClass obj;
    ext::callback<int>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );

    // Exercise
    broadcaster.add_listener( callback );

    // Verify
    CHECK_TRUE( broadcaster.has_listeners() );
    CHECK_EQUAL( 1, broadcaster.listeners_count() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that a listener identified by its own address is removed properly.
 */
TEST( broadcaster, RemoveListener_SelfId_Proper )
{
    // Prepare
    ext::broadcaster<int> broadcaster;
    TestClass obj;
    ext::callback<int>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );
    broadcaster.add_listener( callback );

    // Exercise
    broadcaster.remove_listener( callback );

    // Verify
    CHECK_FALSE( broadcaster.has_listeners() );
    CHECK_EQUAL( 0, broadcaster.listeners_count() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that a listener identified by its own address is not removed if not added previously.
 */
TEST( broadcaster, RemoveListener_SelfId_Other )
{
    // Prepare
    ext::broadcaster<int> broadcaster;
    TestClass obj;
    ext::callback<int>::XPtr callback1 = ext::new_callback( &obj, &TestClass::TestCallback );
    ext::callback<int>::XPtr callback2 = ext::new_callback( &obj, &TestClass::TestCallback );
    broadcaster.add_listener( callback1 );

    // Exercise
    broadcaster.remove_listener( callback2 );

    // Verify
    CHECK_TRUE( broadcaster.has_listeners() );
    CHECK_EQUAL( 1, broadcaster.listeners_count() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that a listener identified by key is added properly.
 */
TEST( broadcaster, AddListener_KeyId )
{
    // Prepare
    ext::broadcaster<int> broadcaster;
    TestClass obj;
    ext::callback<int>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );
    void* key = (void*) 238445;

    // Exercise
    broadcaster.add_listener( callback, key );

    // Verify
    CHECK_TRUE( broadcaster.has_listeners() );
    CHECK_EQUAL( 1, broadcaster.listeners_count() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that a listener identified by key is removed properly.
 */
TEST( broadcaster, RemoveListener_KeyId_Proper )
{
    // Prepare
    ext::broadcaster<int> broadcaster;
    TestClass obj;
    ext::callback<int>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );
    void* key = (void*) 238445;
    broadcaster.add_listener( callback, key );

    // Exercise
    broadcaster.remove_listener( key );

    // Verify
    CHECK_FALSE( broadcaster.has_listeners() );
    CHECK_EQUAL( 0, broadcaster.listeners_count() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that a listener identified by key is not removed if not added previously.
 */
TEST( broadcaster, RemoveListenerKeyId_OtherKey )
{
    // Prepare
    ext::broadcaster<int> broadcaster;
    TestClass obj;
    ext::callback<int>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );
    void* key1 = (void*) 238445;
    void* key2 = (void*) 34644;
    broadcaster.add_listener( callback, key1 );

    // Exercise
    broadcaster.remove_listener( key2 );

    // Verify
    CHECK_TRUE( broadcaster.has_listeners() );
    CHECK_EQUAL( 1, broadcaster.listeners_count() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that a listener identified by key is not removed if tried to remove by address.
 */
TEST( broadcaster, RemoveListener_KeyId_SelfId )
{
    // Prepare
    ext::broadcaster<int> broadcaster;
    TestClass obj;
    ext::callback<int>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );
    void* key = (void*) 3444;
    broadcaster.add_listener( callback, key );

    // Exercise
    broadcaster.remove_listener( callback );

    // Verify
    CHECK_TRUE( broadcaster.has_listeners() );
    CHECK_EQUAL( 1, broadcaster.listeners_count() );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that listeners are properly noticed of a broadcast.
 */
TEST( broadcaster, Broadcast_OneListener )
{
    // Prepare
    const int value = 123;
    ext::broadcaster<int> broadcaster;
    TestClass obj1;
    TestClass obj2;
    ext::callback<int>::XPtr callback1 = ext::new_callback( &obj1, &TestClass::TestCallback );
    ext::callback<int>::XPtr callback2 = ext::new_callback( &obj2, &TestClass::TestCallback );
    broadcaster.add_listener( callback1 );
    mock().expectOneCall("TestClass::TestCallback").onObject( &obj1 ).withParameter( "p", value );

    // Exercise
    broadcaster.broadcast( value );

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that listeners are properly noticed of a broadcast.
 */
TEST( broadcaster, Broadcast_MultipleListeners )
{
    // Prepare
    const int value = 365444;
    ext::broadcaster<int> broadcaster;
    TestClass obj1;
    TestClass obj2;
    TestClass obj3;
    ext::callback<int>::XPtr callback1 = ext::new_callback( &obj1, &TestClass::TestCallback );
    ext::callback<int>::XPtr callback2 = ext::new_callback( &obj2, &TestClass::TestCallback );
    ext::callback<int>::XPtr callback3 = ext::new_callback( &obj3, &TestClass::TestCallback );
    broadcaster.add_listener( callback1 );
    broadcaster.add_listener( callback3 );
    mock().expectOneCall("TestClass::TestCallback").onObject( &obj1 ).withParameter( "p", value );
    mock().expectOneCall("TestClass::TestCallback").onObject( &obj3 ).withParameter( "p", value );

    // Exercise
    broadcaster.broadcast( value );

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that listeners are properly noticed of a broadcast.
 */
TEST( broadcaster, Broadcast_AfterRemoval )
{
    // Prepare
    const int value = 365444;
    ext::broadcaster<int> broadcaster;
    TestClass obj1;
    TestClass obj2;
    TestClass obj3;
    ext::callback<int>::XPtr callback1 = ext::new_callback( &obj1, &TestClass::TestCallback );
    ext::callback<int>::XPtr callback2 = ext::new_callback( &obj2, &TestClass::TestCallback );
    ext::callback<int>::XPtr callback3 = ext::new_callback( &obj3, &TestClass::TestCallback );
    broadcaster.add_listener( callback1 );
    broadcaster.add_listener( callback2 );
    broadcaster.add_listener( callback3 );
    broadcaster.remove_listener( callback2 );
    mock().expectOneCall("TestClass::TestCallback").onObject( &obj1 ).withParameter( "p", value );
    mock().expectOneCall("TestClass::TestCallback").onObject( &obj3 ).withParameter( "p", value );

    // Exercise
    broadcaster.broadcast( value );

    // Verify
    mock().checkExpectations();

    // Cleanup
}

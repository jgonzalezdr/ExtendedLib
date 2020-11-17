/**
 * @file
 * @brief      Header for the 'shared_ptr' class
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) 2016 Jesus Gonzalez
 * @license    See LICENSE.txt
 */

#ifndef Extended_shared_ptr_hpp_
#define Extended_shared_ptr_hpp_

#include <memory>

namespace ext
{

/**
 * Extended smart pointer based on std::shared_ptr.
 *
 * @tparam Type Type of the pointed object
 */
template<typename Type>
class shared_ptr : public std::shared_ptr<Type>
{
public:
    using std::shared_ptr<Type>::shared_ptr;

    /**
     *  Constructs an empty shared_ptr of type @c Type.
     */
    constexpr shared_ptr() noexcept : std::shared_ptr<Type>() {}

    /**
     * Constructs a shared_ptr of type @c Type that shares an object with @a sp
     * which is of another type @c AnotherType (that should be convertible to type @c Type).
     * 
     * @tparam AnotherType Type of the object pointed by @a p
     * @param[in] sp Shared pointer for an @c AnotherType object
     */
    template<typename AnotherType>
    shared_ptr( const std::shared_ptr<AnotherType> &sp ) noexcept : std::shared_ptr<Type>( sp ) {}

    /**
     * Move-constructs a shared_ptr of type @c Type from @a sp
     * which stores objects of another type @c AnotherType (that should be convertible to type @c Type).
     * 
     * @tparam AnotherType Type of the object pointed by @a p
     * @param[in] sp Shared pointer for an @c AnotherType object
     */
    template<typename AnotherType>
    shared_ptr( const std::shared_ptr<AnotherType> &&sp ) noexcept : std::shared_ptr<Type>( std::move(sp) ) {}

    /**
     * Constructs a shared_ptr of type @c Type that owns the object pointed by @a p
     * which is of another type @c AnotherType (that should be convertible to type @c Type).
     *
     * @tparam AnotherType Type of the object pointed by @a p
     * @param[in] p Pointer to an @c AnotherType object
     */
    template<typename AnotherType>
    shared_ptr( AnotherType *p ) noexcept : std::shared_ptr<Type>( p ) {}

    //operator shared_ptr<const Type>&() const
    //{
    //    return ConstRef();
    //}

    //operator shared_ptr<const Type>() const
    //{
    //    return shared_ptr<const Type>( this );
    //}

    //shared_ptr<const Type>& ConstRef() const
    //{
    //    return (shared_ptr<const Type>&) *this;
    //}

    /**
     * Creates a new shared_ptr of type @c Type from a shared_ptr of a different type @c AnotherType,
     * by performing a dynamic (runtime) cast of the pointed instance.
     *
     * @tparam AnotherType Type of the object pointer by @a sp
     */
    template<typename AnotherType>
    static shared_ptr<Type> cast_dynamic( const std::shared_ptr<AnotherType>& sp )
    {
        return std::dynamic_pointer_cast<Type, AnotherType>( sp );
    }

    /**
     * Creates a new shared_ptr of type @c Type from a shared_ptr of a different type @c AnotherType,
     * by performing a static (compile-time) cast of the pointed instance.
     *
     * @tparam AnotherType Type of the object pointer by @a sp
     */
    template<typename AnotherType>
    static shared_ptr<Type> cast_static( const std::shared_ptr<AnotherType>& sp )
    {
        return std::static_pointer_cast<Type, AnotherType>( sp );
    }
};

} // namespace

/**
 * Smart pointer to class declaration.
 *
 * This macro is used inside the declaration of a class
 * to define \c Ptr and \c ConstPtr as typedefs of smart pointers to that class instances.
 *
 * @param[in] cls Class name
 */
#define DECLARE_SHAREDPTR(cls) \
/** Type definition for a shared_ptr to non-constant instances of this class */ \
typedef ::ext::shared_ptr< cls > XPtr; \
/** Type definition for a shared_ptr to constant instances of this class */ \
typedef ::ext::shared_ptr< const cls > XPtrConst; \
/** Type definition for a weak_ptr to non-constant instances of this class */ \
typedef ::std::weak_ptr< cls > WeakPtr; \
/** Type definition for a shared_ptr to constant instances of this class */ \
typedef ::std::weak_ptr< const cls > WeakPtrConst;

/**
 * Smart pointer to class declaration and default constructor declaration.
 *
 * This macro is used inside the declaration of a class
 * to define \c Ptr and \c ConstPtr as typedefs of smart pointers to that class instances.
 *
 * Additionally, a \c New() constructor method is defined that creates a default instance
 * of the class and returns a smart pointer to it.
 *
 * @param[in] cls Class name
 */
#define DECLARE_SHAREDPTR_NEWCTOR(cls) \
DECLARE_SHAREDPTR(cls) \
/** Creates a new instance of this class and returns a shared_ptr to it */ \
static ::ext::shared_ptr< cls > New() { return new cls(); }

#endif // header guard

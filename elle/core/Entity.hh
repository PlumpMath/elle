//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit (c)
//
// file          /home/mycure/infinit/elle/core/Entity.hh
//
// created       julien quintard   [sun feb 22 19:43:33 2009]
// updated       julien quintard   [wed mar 10 20:25:57 2010]
//

#ifndef ELLE_CORE_ENTITY_HH
#define ELLE_CORE_ENTITY_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/core/Boolean.hh>

#include <elle/misc/Status.hh>
#include <elle/misc/Report.hh>
#include <elle/misc/Maid.hh>

#include <elle/idiom/Open.hh>

namespace elle
{
  using namespace misc;

  namespace core
  {

//
// ---------- macro functions -------------------------------------------------
//

///
/// this macro function makes it easy for classes to derive from Entity
/// as only a copy constructor and a == operator must be manually written.
/// everything else, assignment, != operator, sizeof are automatically
/// generated by the following.
///
#define EntityEmbed(_type_)						\
  _type_&		operator=(const _type_&		element)	\
  {									\
    enter();								\
									\
    if (this == &element)						\
      return (*this);							\
									\
    if (this->Recycle<_type_>(&element) == StatusError)			\
      yield("unable to recycle the object", *this);			\
									\
    return (*this);							\
  }									\
									\
  Boolean		operator!=(const _type_&	element) const	\
  {									\
    return (!(*this == element));					\
  }									\
									\
  Status		Imprint(Natural32&		size) const	\
  {									\
    enter();								\
									\
    size = sizeof(_type_);						\
									\
    leave();								\
  }

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this interface must be implemented by every 'entity' ... in the
    /// elle library terms.
    ///
    class Entity
    {
    public:
      //
      // constructors and destructors
      //

      ///
      /// every class inheriting directly or indirectly the Entity interface
      /// must define a default constructor.
      ///
      /// note that nothing---especially no allocation---should be performed
      /// in this method but initializing the attributes to default values.
      ///
      Entity()
      {
      }

      ///
      /// the copy constructor is responsible for duplicating an entity
      /// and should therefore be provided for every class also providing
      /// the = operator.
      ///
      Entity(const Entity&)
      {
      }

      ///
      /// this destructor should release the allocated resources but must
      /// not re-set attributes to default values.
      ///
      virtual ~Entity()
      {
      }

      //
      // methods
      //

      ///
      /// this method recycles an entity by deallocating resources and
      /// re-setting them to their default values.
      ///
      template <typename T>
      Status		Recycle(const T*			entity = NULL)
      {
	enter();

	// release the resources.
	this->~Entity();

	if (entity == NULL)
	  {
	    // initialize the object with default values.
	    new (this) T;
	  }
	else
	  {
	    // initialize the object with defined values.
	    new (this) T(*entity);
	  }

	leave();
      }

      ///
      /// this method returns the memory footprint of the actual object.
      ///
      /// this method has been introduced because there is no way to known
      /// the size of an object when its base class is manipulated.
      ///
      /// note that this method can be automatically generating by using
      /// the Embed(Entity, T) macro function.
      ///
      virtual Status	Imprint(Natural32&			size) const
      {
	enter();

	escape("this method should never have been called");
      }

      //
      // operators
      //

      ///
      /// this operator copies an entity.
      ///
      /// this method (i) starts by checking if the given entity
      /// is not the current one before (ii) calling Recycle().
      ///
      /// note that the Embed(Entity, T) macro function can be used
      /// to automatically embed the source code of this operator.
      ///
      virtual Entity&	operator=(const Entity&)
      {
	fail("this method should never have been called");
      }

      ///
      /// this operator compares two entitys.
      ///
      virtual Boolean	operator==(const Entity&) const
      {
	enter();

	flee("this method should never have been called");
      }

      ///
      /// this operator compares two entitys. the source code of this
      /// function can be automatically generated through the Embed(Entity, T)
      /// macro function.
      ///
      virtual Boolean	operator!=(const Entity&) const
      {
	enter();

	flee("this method should never have been called");
      }
    };

  }
}

#endif

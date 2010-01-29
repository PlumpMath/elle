//
// ---------- header ----------------------------------------------------------
//
// project       etoile
//
// license       infinit
//
// file          /home/mycure/infinit/etoile/depot/Depot.hh
//
// created       julien quintard   [tue sep  1 01:08:05 2009]
// updated       julien quintard   [thu jan 28 14:14:15 2010]
//

#ifndef ETOILE_DEPOT_DEPOT_HH
#define ETOILE_DEPOT_DEPOT_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <etoile/hole/Hole.hh>
#include <etoile/core/Core.hh>

#include <etoile/depot/Repository.hh>
#include <etoile/depot/Record.hh>
#include <etoile/depot/Cell.hh>
#include <etoile/depot/Unit.hh>

#include <map>
#include <list>

namespace etoile
{
  namespace depot
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class abstracts the storage layer into three units: the cache
    /// which keeps blocks in main memory, the reserve which keeps block
    /// on a stable but slower storage device such as on a hard disk and
    /// the hole which represents the online storage space.
    ///
    /// note that blocks are passed as pointers so that no allocation nor
    /// copies are required, leading to better performance.
    ///
    /// furthermore, the depot---hence the underlying cache, reserve and
    /// hole---are expecting to take blocks allocated through the new
    /// operator. likewise, the Get() method returns an already
    /// new-operator-allocated block.
    ///
    class Depot
    {
    public:
      //
      // static methods
      //
      static Status	Initialize();
      static Status	Clean();

      //
      // templates
      //

      ///
      /// this method has been introduced because the C++ typing system
      /// seems unable to implicitly cast an core::Object* for instance
      /// to a hole::Block*& or to const-equivalents.
      ///
      template <typename T>
      static Status	Put(const hole::Address&		address,
			    T*					block)
      {
	// just forward the call to the appropriate method with the correct
	// types.
	return (Depot::Put(address, (hole::Block*)block));
      }

      template <typename T>
      static Status	Get(const hole::Address&		address,
			    T*&					block)
      {
	// just forward the call to the appropriate method with the correct
	// types.
	return (Depot::Get(address, (hole::Block*&)block));
      }

    private:
      //
      // static methods
      //
      static Status	Put(const hole::Address&,
			    hole::Block*);
      static Status	Get(const hole::Address&,
			    hole::Block*&);
    };

  }
}

#endif

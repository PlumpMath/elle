//
// ---------- header ----------------------------------------------------------
//
// project       etoile
//
// license       infinit (c)
//
// file          /home/mycure/infinit/etoile/core/Data.hh
//
// created       julien quintard   [tue aug  4 06:54:28 2009]
// updated       julien quintard   [thu jan 28 22:07:18 2010]
//

#ifndef ETOILE_CORE_DATA_HH
#define ETOILE_CORE_DATA_HH

//
// ---------- includes --------------------------------------------------------
//

#include <etoile/core/Contents.hh>

namespace etoile
{
  namespace core
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class represents file data.
    ///
    class Data:
      public Contents
    {
    public:
      //
      // identifier
      //
      static const String	Identifier;

      //
      // constructors & destructors
      //
      Data();

      //
      // methods
      //
      Status		Read(const Natural64,
			     Byte*,
			     Natural64&) const;
      Status		Write(const Natural64,
			      const Byte*,
			      const Natural64);
      Status		Adjust(const Natural64);

      Status		Size(Natural64&) const;

      //
      // interfaces
      //

      // entity
      Embed(Entity, Data);
      // XXX operator==

      // dumpable
      Status		Dump(const Natural32 = 0) const;

      // archivable
      Status		Serialize(Archive&) const;
      Status		Extract(Archive&);

      //
      // attributes
      //
      Region		region;
    };

  }
}

#endif

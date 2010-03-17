//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit (c)
//
// file          /home/mycure/infinit/elle/test/network/door/Test.hh
//
// created       julien quintard   [wed jan 28 11:28:16 2009]
// updated       julien quintard   [wed mar 17 10:36:44 2010]
//

#ifndef ELLE_TEST_NETWORK_TEST_HH
#define ELLE_TEST_NETWORK_TEST_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <elle/test/network/door/Server.hh>
#include <elle/test/network/door/Client.hh>

#include <elle/idiom/Close.hh>
# include <QCoreApplication>
# include <iostream>
#include <elle/idiom/Open.hh>

namespace elle
{
  using namespace core;

  namespace test
  {

//
// ---------- classes ---------------------------------------------------------
//

    class Test
    {
    public:
      //
      // methods
      //
      static Status	Main(int, char*[]);
    };

  }
}

#endif


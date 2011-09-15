//
// ---------- header ----------------------------------------------------------
//
// project       infinit
//
// license       infinit
//
// author        julien quintard   [tue may  4 22:00:43 2010]
//

#ifndef INFINIT_HH
#define INFINIT_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>
#include <lune/Lune.hh>

//
// ---------- classes ---------------------------------------------------------
//

class Infinit
{
public:
  //
  // constants
  //
  static const elle::String		Key;
  static const elle::String		Version;
  static const elle::String		Copyright;

  //
  // globals
  //
  static lune::Authority		Authority;
  static lune::Configuration		Configuration;

  static elle::Parser*			Parser;

  //
  // methods
  //
  static elle::Status		Initialize();
  static elle::Status		Clean();
};

#endif

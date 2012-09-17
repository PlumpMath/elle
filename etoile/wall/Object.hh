#ifndef ETOILE_WALL_OBJECT_HH
# define ETOILE_WALL_OBJECT_HH

# include <elle/types.hh>

# include <etoile/path/fwd.hh>
# include <etoile/gear/fwd.hh>
# include <etoile/abstract/fwd.hh>

namespace etoile
{
  namespace wall
  {

    ///
    /// this class provides general-purpose methods for manipulating objects.
    ///
    /// these are very useful when the caller does not know the genre of
    /// the object i.e file, directory or link.
    ///
    class Object
    {
    public:
      /// Loads the objec identified by \a chemin into a new scope and
      /// return the scope's identifier.
      static
      gear::Identifier
      Load(path::Chemin const& chemin);
      /// Lock the object.
      ///
      /// \return whether the lock has been acquired.
      static
      bool
      Lock(gear::Identifier const&);
      /// Release a previously locked object.
      static
      void
      Release(gear::Identifier const&);
      /// The general abstract regarding the identified object.
      static
      abstract::Object
      Information(gear::Identifier const&);
      /// Discard the scope, potentially ignoring some
      /// modifications.
      static
      void
      Discard(gear::Identifier const&);
      /// Commit the pending modifications by placing the
      /// scope in the journal.
      static
      void
      Store(const gear::Identifier&);
      /// Destroy an object.
      ///
      /// Use with great care since, not knowing the object's genre, the
      /// data blocks will not be removed. therefore, the genre-specific
      /// Destroy() method should always be preferred.
      static
      void
      Destroy(gear::Identifier const&);
      /// Remove all the blocks of all the versions associated with an
      /// object.
      ///
      /// Use with great care since, not knowing the object's genre, the
      /// data blocks will not be removed. therefore, the genre-specific
      /// Destroy() method should always be preferred.
      static
      void
      Purge(gear::Identifier const&);
    };

  }
}

#endif

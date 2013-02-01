#ifndef ETOILE_SHRUB_RIFFLE_HH
# define ETOILE_SHRUB_RIFFLE_HH

# include <elle/types.hh>
# include <elle/utility/Time.hh>

# include <nucleus/proton/Location.hh>

# include <etoile/path/Slab.hh>

# include <boost/noncopyable.hpp>

# include <map>
# include <utility>

namespace etoile
{
  namespace shrub
  {

    ///
    /// this class represents a riffle i.e a component of the hierarchical
    /// file system structure.
    ///
    /// riffles store the slab and corresponding location for a path's element
    /// but also a pointer to the parent directory.
    ///
    /// although it would have been easier to rely on class inheritance
    /// to prevent riffles from storing a pointer to a useless hierarchy
    /// variable, it would have been too much burden for no benefit.
    ///
    class Riffle:
      private boost::noncopyable
    {
    public:
      //
      // types
      //
      typedef std::pair<path::Slab, Riffle*>    Value;
      typedef std::map<path::Slab, Riffle*>     Container;
      typedef Container::iterator               Iterator;
      typedef Container::const_iterator         Scoutor;

      //
      // constructors & destructors
      //
      Riffle();

      //
      // methods
      //
      elle::Status              Create(const path::Slab&,
                                       const nucleus::proton::Location&,
                                       Riffle* = nullptr);

      elle::Status              Resolve(const path::Slab&,
                                        Riffle*&);
      elle::Status              Update(const path::Slab&,
                                       const nucleus::proton::Location&);
      elle::Status              Destroy(const path::Slab&);

      elle::Status              Flush();

      //
      // interfaces
      //

      // dumpable
      elle::Status              Dump(const elle::Natural32 = 0);

      //
      // attributes
      //
      path::Slab                slab;
      nucleus::proton::Location location;

      elle::utility::Time                timestamp;

      Riffle*                   parent;
      Container                 children;
    };

  }
}

#endif

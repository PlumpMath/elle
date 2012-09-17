#include <nucleus/proton/MutableBlock.hh>
#include <nucleus/proton/History.hh>
#include <nucleus/proton/Version.hh>
#include <nucleus/proton/Address.hh>
#include <nucleus/proton/Network.hh>
#include <nucleus/Nucleus.hh>

#include <elle/log.hh>
#include <elle/io/File.hh>
#include <elle/io/Piece.hh>

#include <lune/Lune.hh>

#include <hole/Hole.hh>

#include <Infinit.hh>

#include <elle/idiom/Close.hh>
# include <boost/lexical_cast.hpp>
#include <elle/idiom/Open.hh>

ELLE_LOG_COMPONENT("infinit.nucleus.proton.MutableBlock");

namespace nucleus
{
  namespace proton
  {

//
// ---------- constructs & destructors ----------------------------------------
//

    ///
    /// default constructor.
    ///
    MutableBlock::MutableBlock():
      Block(FamilyUnknown, neutron::ComponentUnknown),

      version(Version::First)
    {
    }

    ///
    /// specific constructor.
    ///
    MutableBlock::MutableBlock(const Family                     family,
                               const neutron::Component         component):
      Block(family, component),

      version(Version::First)
    {
    }

//
// ---------- methods ---------------------------------------------------------
//

    elle::Boolean
    MutableBlock::derives(MutableBlock const& other) const
    {
      return (this->version > other.version);
    }

    elle::io::Path
    MutableBlock::_path(Network const& network,
                        Address const& address)
    {
      return (elle::io::Path(lune::Lune::Network::Shelter::MutableBlock,
                             elle::io::Piece("%NETWORK%", network.name),
                             elle::io::Piece("%ADDRESS%", address.unique())));
    }

    elle::io::Path
    MutableBlock::_path(Network const& network,
                        Address const& address,
                        elle::String const& version)
    {
      return (elle::io::Path(lune::Lune::Network::Shelter::MutableBlock,
                             elle::io::Piece("%NETWORK%", network.name),
                             elle::io::Piece("%ADDRESS%", address.unique()),
                             elle::io::Piece("%VERSION%", version)));
    }

//
// ---------- object ----------------------------------------------------------
//

    ///
    /// this macro-function call generates the object.
    ///
    embed(MutableBlock, _());

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps the block's internals.
    ///
    elle::Status        MutableBlock::Dump(const elle::Natural32 margin) const
    {
      elle::String      alignment(margin, ' ');

      std::cout << alignment << "[MutableBlock]" << std::endl;

      // dump the parent.
      if (Block::Dump(margin + 2) == elle::Status::Error)
        escape("unable to dump the parent");

      // dump the version.
      if (this->version.Dump(margin + 2) == elle::Status::Error)
        escape("unable to dump the version");

      return elle::Status::Ok;
    }

//
// ---------- fileable --------------------------------------------------------
//

    void
    MutableBlock::load(Network const& network,
                       Address const& address,
                       Version const& version)
    {
      ELLE_TRACE_SCOPE("%s: load(%s, %s, %s)", *this, network, address, version);

      // operate depending on the network's support of history.
      if (hole::Hole::Descriptor.meta().history() == false)
        {
          this->load(elle::io::Path(MutableBlock::_path(network, address, "@")));
        }
      else
        {
          /* XXX
          elle::standalone::Region region;
          elle::String number;
          elle::io::Unique unique(address.unique());

          // if the requested version is the latest...
          if (version == Version::Last)
            {
              elle::io::Path link(path);
              elle::standalone::Region r;

              // complete the link path.
              if (link.Complete(elle::io::Piece("%NETWORK%", network.name),
                                elle::io::Piece("%ADDRESS%", unique),
                                elle::io::Piece("%VERSION%", "@")) ==
                  elle::Status::Error)
                throw std::runtime_error("unable to complete the path");

              // read the file's content.
              if (elle::io::File::Read(link, r) == elle::Status::Error)
                throw std::runtime_error("unable to read the file's content");

              // set the number of the latest version.
              number = elle::String(reinterpret_cast<const char*>(r.contents),
                                    r.size);
            }
          else
            {
              number = boost::lexical_cast<std::string>(version.number);
            }

          // complete the path with the network name.
          if (path.Complete(elle::io::Piece("%NETWORK%", network.name),
                            elle::io::Piece("%ADDRESS%", unique),
                            elle::io::Piece("%VERSION%", number)) ==
              elle::Status::Error)
            throw std::runtime_error("unable to complete the path");

          this->load(path);
          */
        }
    }

    void
    MutableBlock::store(Network const& network,
                        Address const& address) const
    {
      ELLE_TRACE_SCOPE("%s: store(%s, %s)", *this, network, address);

      // operate depending on the network's support of history.
      if (hole::Hole::Descriptor.meta().history() == false)
        {
          //
          // if the history is not supported, store the mutable block
          // in a file without version number extension.
          //

          this->store(
            elle::io::Path(MutableBlock::_path(network, address, "@")));
        }
      else
        {
          /* XXX
          //
          // otherwise, store the block in a file with a name of the
          // form [identifier]#[version number].blk. besides, a special
          // file of the form [identifier]#@.blk contains the number
          // of the latest version of the mutable block.
          //

          elle::String number;
          elle::io::Path link(file);
          elle::standalone::Region region;
          nucleus::proton::History history;

          number = boost::lexical_cast<std::string>(this->version.number);

          // complete the file path.
          if (file.Complete(elle::io::Piece("%VERSION%", number)) ==
              elle::Status::Error)
            throw std::runtime_error("unable to complete the path");

          this->store(file);

          // complete the link path.
          if (link.Complete(elle::io::Piece("%VERSION%", "@")) ==
              elle::Status::Error)
            throw std::runtime_error("unable to complete the path");

          // if there is a link, remove it.
          if (elle::io::File::Exist(link) == true)
            {
              // delete the file which references the latest version since a
              // new version has been created.
              if (elle::io::File::Erase(link) == elle::Status::Error)
                throw std::runtime_error("unable to erase the block link");
            }

          // finally, create the block link.
          if (elle::io::File::Write(
                link,
                elle::standalone::Region(
                  reinterpret_cast<const elle::Byte*>(number.c_str()),
                  number.length())) == elle::Status::Error)
            throw std::runtime_error("unable to create the block link");

          // XXX here the history should be loaded before the file is
          // XXX being stored. then the file should be stored only if the
          // XXX file does not already exist. finally, the @ link should be
          // XXX updated (if the file has been stored) with the latest version
          // XXX number (which is not necessarily the one given here; indeed
          // XXX this method could be called for version 4 though a version
          // XXX 32 already exists). finally, the history should be re-worked
          // XXX so as to used a sorted data structure such as a map which
          // XXX would then be used to retrieve the latest version number.
          throw std::runtime_error("XXX");

          // if there is a history, load it.
          if (history.exists(network, address) == true)
            {
              // load the history.
              history.load(network, address);
            }

          // register the new version.
          if (history.Register(version) == elle::Status::Error)
            throw std::runtime_error("unable to register the new version");

          // store the history.
          history.store(network, address);
          */
        }
    }

    void
    MutableBlock::erase(Network const& network,
                        Address const& address)
    {
      ELLE_TRACE_SCOPE("erase(%s, %s)", network, address);

      // operate depending on the network's support of history.
      if (hole::Hole::Descriptor.meta().history() == false)
        {
          elle::concept::Fileable<>::erase(
            MutableBlock::_path(network, address, "@"));
        }
      else
        {
          /* XXX
          elle::io::Unique unique(address.unique);
          History       history;
          Version::Type size;
          Version::Type i;

          // complete the path with the network name.
          if (path.Complete(elle::io::Piece("%NETWORK%", network.name),
                            elle::io::Piece("%ADDRESS%", unique)) ==
              elle::Status::Error)
            throw std::runtime_error("unable to complete the path");

          // load the history.
          if (history.Load(network, address) == elle::Status::Error)
            throw std::runtime_error("unable to load the history");

          // retrieve the number of versions.
          if (history.Size(size) == elle::Status::Error)
            throw std::runtime_error("unable to retrieve the size of the history");

          // go through the versions.
          for (i = 0; i < size; i++)
            {
              nucleus::proton::Version version;
              elle::String      number;
              elle::io::Path        file;

              // select a particular version.
              if (history.Select(i, version) == elle::Status::Error)
                throw std::runtime_error("unable to select a particular version");

              try
                {
                  number = boost::lexical_cast<std::string>(version.number);
                }
              catch (std::exception const&)
                {
                  throw std::runtime_error("unable to transform the version number into a string");
                }

              // duplicate the generic path.
              file = path;

              // complete the path with the version number.
              if (file.Complete(elle::io::Piece("%VERSION%",
                                            number)) == elle::Status::Error)
                throw std::runtime_error("unable to complete the path");

              // is the file present...
              if (elle::io::File::Exist(file) == elle::Status::True)
                {
                  // erase the file.
                  if (elle::io::File::Erase(file) == elle::Status::Error)
                    throw std::runtime_error("unable to erase the file");
                }
            }

          // complete the path with the last version pointer.
          if (path.Complete(elle::io::Piece("%VERSION%", "@")) ==
              elle::Status::Error)
            throw std::runtime_error("unable to complete the path");

          // is the file present...
          if (elle::io::File::Exist(path) == elle::Status::True)
            {
              // delete the link which references the latest version.
              if (elle::io::File::Erase(path) == elle::Status::Error)
                throw std::runtime_error("unable to erase the block link");
            }

          // erase the history.
          if (history.Erase(network, address) == elle::Status::Error)
            throw std::runtime_error("unable to erase the history");
          */
        }
    }

    elle::Boolean
    MutableBlock::exists(Network const& network,
                         Address const& address,
                         Version const& version)
    {
      ELLE_TRACE_SCOPE("exists(%s, %s)", network, address);

      // operate depending on the network's support of history.
      if (hole::Hole::Descriptor.meta().history() == false)
        {
          if (version == Version::Last)
            {
              return (elle::concept::Fileable<>::exists(
                MutableBlock::_path(network, address, "@")));
            }
          else
            {
              elle::String number(
                boost::lexical_cast<elle::String>(version.number));

              return (elle::concept::Fileable<>::exists(
                MutableBlock::_path(network, address, number)));
            }
        }
      else
        {
          /* XXX
           elle::String unique(address.unique);
           elle::String  number;

          // if the requested version is the latest...
          if (version == Version::Last)
            {
              elle::io::Path        link;
              elle::standalone::Region      r;

              // copy the path.
              link = path;

              // complete the link path.
              if (link.Complete(elle::io::Piece("%NETWORK%", network.name),
                                elle::io::Piece("%ADDRESS%", unique),
                                elle::io::Piece("%VERSION%", "@")) ==
                  elle::Status::Error)
                flee("unable to complete the path");

              // read the file's content.
              if (elle::io::File::Read(link, r) == elle::Status::Error)
                flee("unable to read the file's content");

              // set the number of the latest version.
              number = elle::String(reinterpret_cast<const char*>(r.contents),
                                    r.size);
            }
          else
            {
              try
                {
                  number = boost::lexical_cast<std::string>(version.number);
                }
              catch (std::exception const&)
                {
                  flee("unable to convert the version number into a string");
                }
            }

          // complete the path with the network name.
          if (path.Complete(elle::io::Piece("%NETWORK%", network.name),
                            elle::io::Piece("%ADDRESS%", unique),
                            elle::io::Piece("%VERSION%", number)) ==
              elle::Status::Error)
            flee("unable to complete the path");

          // test the file.
          if (elle::io::File::Exist(path) == elle::Status::True)
            return elle::Status::True;
          */
        }

      return (false);
    }

  }
}

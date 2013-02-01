#include <nucleus/neutron/Reference.hh>
#include <nucleus/proton/Nest.hh>
#include <nucleus/proton/Contents.hh>

namespace nucleus
{
  namespace neutron
  {

//
// ---------- definitions -----------------------------------------------------
//

    proton::Nature const Reference::Constants::seam =
      proton::Nature::reference_seam;
    proton::Nature const Reference::Constants::quill =
      proton::Nature::reference_quill;
    proton::Nature const Reference::Constants::value =
      proton::Nature::reference_value;
    proton::Nature const Reference::Constants::nature =
      Reference::Constants::value;

//
// ---------- constructor & destructors ---------------------------------------
//

    Reference::Reference():
      Value::Value()
    {
      this->state(proton::State::dirty); // XXX[est-ce necessaire?]
      this->footprint(0); // XXX[initial catalog footprint]
    }

    ELLE_SERIALIZE_CONSTRUCT_DEFINE(Reference, proton::Value)
    {
    }

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method creates the data.
    ///
    elle::Status        Reference::Create()
    {
      // set the initial state.
      this->state(proton::State::dirty);

      return elle::Status::Ok;
    }

    ///
    /// this method sets the target way.
    ///
    elle::Status        Reference::Target(const elle::String&     target)
    {
      // set the target.
      this->target = target;

      // set the reference as dirty.
      this->state(proton::State::dirty);

      return elle::Status::Ok;
    }

    ///
    /// this method retrieve the target.
    ///
    elle::Status        Reference::Resolve(elle::String&        target) const
    {
      // retrieve the target.
      target = this->target;

      return elle::Status::Ok;
    }

    ///
    /// this method returns the length of the referenced way.
    ///
    elle::Status        Reference::Capacity(Size&               size) const
    {
      // retrieve the length.
      size = static_cast<Size>(this->target.length());

      return elle::Status::Ok;
    }

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this function dumps the object.
    ///
    elle::Status        Reference::Dump(elle::Natural32         margin) const
    {
      elle::String      alignment(margin, ' ');

      std::cout << alignment << "[Reference] " << std::endl;

      if (Value::Dump(margin + 2) == elle::Status::Error)
        throw Exception("unable to dump the value");

      // dump the target.
      std::cout << alignment << elle::io::Dumpable::Shift << "[Target] "
                << this->target << std::endl;

      return elle::Status::Ok;
    }

//
// ---------- printable -------------------------------------------------------
//

    void
    Reference::print(std::ostream& stream) const
    {
      stream << "reference("
             << this->target
             << ")";
    }

  }
}

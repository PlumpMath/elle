#pragma once

#include <unordered_set>

#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>

#include <elle/Error.hh>
#include <elle/Option.hh>
#include <elle/Printable.hh>
#include <elle/attribute.hh>
#include <elle/serialization/Serializer.hh>

#include <elle/reactor/Barrier.hh>

namespace elle
{
  namespace athena
  {
    namespace paxos
    {
      /// XXX[doc].
      ///
      template <typename T,
                typename Version,
                typename ClientId,
                typename ServerId = ClientId>
      class Server
        : public elle::Printable
      {
        /*------.
        | Types |
        `------*/
      public:
        using Self = Server;
        using Quorum = std::unordered_set<ServerId>;

        /*---------.
        | Proposal |
        `---------*/
      public:
        /// A Proposal for the consensus.
        ///
        /// @see https://en.wikipedia.org/wiki/Paxos_(computer_science).
        ///
        /// "Each attempt to define an agreed value v is performed with
        ///  proposals which may or may not be accepted by Acceptors.
        ///  Each proposal is uniquely numbered for a given Proposer. The value
        ///  corresponding to a numbered proposal can be computed as part of
        ///  running the Paxos protocol, but need not be."
        ///
        /// A proposal is defined as a tuple (version, round, sender).
        struct Proposal
        {
          /// Construct an empty Proposal (used for deserialization purpose).
          Proposal();
          /// Construct a Proposal for a given tuple (version, round, sender).
          Proposal(Version version, int round, ClientId sender);
          Version version;
          int round;
          ClientId sender;
          /// Check if this proposal is equal to @a given one.
          ///
          /// @param rhs Another proposal.
          /// @returns Whether proposals are equal.
          bool
          operator ==(Proposal const& rhs) const;
          /// Check if this proposal is lesser than @a given one.
          ///
          /// @param rhs Another proposal.
          /// @returns Whether this proposal is lesser than the given proposal.
          bool
          operator <(Proposal const& rhs) const;
          /// (De)Serialize a Proposal.
          void
          serialize(elle::serialization::Serializer& s);
          using serialization_tag = elle::serialization_tag;
          friend
          std::ostream&
          operator <<(std::ostream& output,
                      typename Server<T, Version, ClientId>::Proposal const& p)
          {
            elle::fprintf(output, "%f:%f:%f", p.version, p.round, p.sender);
            return output;
          }
        };

        /*---------.
        | Accepted |
        `---------*/
      public:
        /// An accepted Proposal.
        struct Accepted
        {
          /// Deserialize an Accepted proposal.
          Accepted(elle::serialization::SerializerIn& s, elle::Version const& v);
          /// Construct a Accepted from @a proposal and @a value.
          ///
          /// @param proposal The proposal accepted.
          /// @param value The value agreed.
          /// @param confirmed Whether the acceptation is confirmed.
          Accepted(Proposal proposal, elle::Option<T, Quorum> value,
                   bool confirmed);
          Proposal proposal;
          elle::Option<T, Quorum> value;
          bool confirmed;
          void
          serialize(elle::serialization::Serializer& s, elle::Version const& v);
          using serialization_tag = elle::serialization_tag;
          friend
          std::ostream&
          operator <<(std::ostream& output, Accepted const& accepted)
          {
            elle::fprintf(output, "Accepted(%f)", accepted.proposal);
            return output;
          }
        };

        /*------------.
        | WrongQuorum |
        `------------*/
      public:
        /// An wrong quorum Error.
        class WrongQuorum
          : public elle::Error
        {
        public:
          using Super = elle::Error;
          /// Construct a WrongQuorum exception.
          ///
          /// @param excepted The expected Quorum.
          /// @param effective The effective Quorum.
          WrongQuorum(Quorum expected, Quorum effective);
          /// Deserialize a WrongQuorum exception.
          WrongQuorum(elle::serialization::SerializerIn& input,
                      elle::Version const& version);
          /// (De)Serialize a WrongQuorum exception.
          void
          serialize(elle::serialization::Serializer& s,
                    elle::Version const& version) override;
          ELLE_ATTRIBUTE_R(Quorum, expected);
          ELLE_ATTRIBUTE_R(Quorum, effective);
        private:
          void
          _serialize(elle::serialization::Serializer& s,
                     elle::Version const& version);
        };
      private:
        static const elle::serialization::Hierarchy<elle::Exception>::
        Register<WrongQuorum> _register_wrong_quorum_serialization;

        /*-------------.
        | PartialState |
        `-------------*/
      public:
        /// XXX[doc].
        class PartialState
          : public elle::Error
        {
        public:
          using Super = elle::Error;
          PartialState(Proposal p);
          PartialState(elle::serialization::SerializerIn& input,
                       elle::Version const& version);

          void
          serialize(elle::serialization::Serializer& s,
                    elle::Version const& version) override;
          ELLE_ATTRIBUTE_R(Proposal, proposal);
        private:
          void
          _serialize(elle::serialization::Serializer& s,
                     elle::Version const& version);
        };
      private:
        static const elle::serialization::Hierarchy<elle::Exception>::
        Register<PartialState> _register_partial_state_serialization;

        /*-------------.
        | Construction |
        `-------------*/
      public:
        Server(ServerId id, Quorum quorum,
               elle::Version version = elle::serialization_tag::version);
        ELLE_ATTRIBUTE_R(ServerId, id);
        ELLE_ATTRIBUTE_R(Quorum, quorum);
        ELLE_ATTRIBUTE_R(boost::optional<T>, value);
        ELLE_ATTRIBUTE_R(elle::Version, version);
        ELLE_ATTRIBUTE_R(bool, partial);

        /*----------.
        | Consensus |
        `----------*/
      public:
        /// Propose @a Proposal to @a Quorum.
        ///
        /// If the Proposal is already outdated, return the newest Proposal.
        ///
        boost::optional<Accepted>
        propose(Quorum q, Proposal p);
        Proposal
        accept(Quorum q, Proposal p, elle::Option<T, Quorum> value);
        void
        confirm(Quorum q, Proposal p);
        boost::optional<Accepted>
        get(Quorum q);
        Quorum
        current_quorum() const;
        boost::optional<Accepted>
        current_value() const;
        Version
        current_version() const;
        struct VersionState
        {
          VersionState(Proposal p, boost::optional<Accepted> a = {});
          Proposal proposal;
          boost::optional<Accepted> accepted;
          Version
          version() const;
          VersionState(elle::serialization::SerializerIn& s,
                       elle::Version const& v);
          void
          serialize(elle::serialization::Serializer& s, elle::Version const& v);
        using serialization_tag = elle::serialization_tag;
        };
        ELLE_ATTRIBUTE(boost::optional<VersionState>, state);

      private:
        struct _Details;
        friend struct _Details;

        /*--------------.
        | Serialization |
        `--------------*/
      public:
        Server(elle::serialization::SerializerIn& s, elle::Version const& v);
        void
        serialize(elle::serialization::Serializer& s, elle::Version const& v);
        using serialization_tag = elle::serialization_tag;

        /*----------.
        | Printable |
        `----------*/
      public:
        void
        print(std::ostream& output) const override;
      };
    }
  }
}

#include <elle/athena/paxos/Server.hxx>

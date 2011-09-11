//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/network/Slot.hh
//
// created       julien quintard   [wed feb  3 21:04:37 2010]
// updated       julien quintard   [sun sep  4 13:23:00 2011]
//

///
/// this very special include is required as Socket needs to know Slot
/// while Slot inherits Socket. including Socket.hh normally makes moc,
/// the QT meta object compiler, unable to detect the QObject classes.
///
/// therefore, Socket.hh is not included when moc processes a header file.
///
#ifndef Q_MOC_RUN
# include <elle/network/Socket.hh>
#endif

#ifndef ELLE_NETWORK_SLOT_HH
#define ELLE_NETWORK_SLOT_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/core/Natural.hh>
#include <elle/core/String.hh>

#include <elle/standalone/Region.hh>

#include <elle/radix/Status.hh>

#include <elle/concurrency/Event.hh>
#include <elle/concurrency/Signal.hh>

#include <elle/network/Socket.hh>
#include <elle/network/Packet.hh>
#include <elle/network/Raw.hh>
#include <elle/network/Port.hh>
#include <elle/network/Session.hh>

#include <elle/idiom/Close.hh>
# include <QObject>
# include <QUdpSocket>
#include <elle/idiom/Open.hh>

namespace elle
{
  using namespace core;
  using namespace standalone;
  using namespace radix;
  using namespace concurrency;

  namespace network
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// a slot represents a non-connected socket, typically implemented
    /// through the UDP protocol.
    ///
    class Slot:
      public ::QObject,

      public Socket
    {
      Q_OBJECT;

    public:
      //
      // constructors & destructors
      //
      Slot();
      ~Slot();

      //
      // methods
      //
      Status		Create();
      Status		Create(const Port);

      Status		Write(const Locus&,
			      const Packet&);
      Status		Read(Locus&,
			     Raw&);

      template <typename I>
      Status		Send(const Locus&,
			     const I,
			     const Event& = Event::Null);
      template <typename O>
      Status		Receive(const Event&,
				O);
      template <typename I,
		typename O>
      Status		Call(const Locus&,
			     const I,
			     O);
      template <typename I>
      Status		Reply(const I,
			      Session* = NULL);

      //
      // callbacks
      //
      Status		Dispatch();

      //
      // interfaces
      //

      // dumpable
      Status		Dump(const Natural32 = 0) const;

      //
      // attributes
      //
      Port		port;

      ::QUdpSocket*	socket;

      //
      // signals
      //
      struct
      {
	Signal<
	  Parameters<>
	  >		ready;
	Signal<
	  Parameters<
	    const String&
	    >
	  >		error;
      }			signal;

    private slots:
      //
      // slots
      //
      void		_ready();
      void		_error(const QAbstractSocket::SocketError);
    };

  }
}

//
// ---------- templates -------------------------------------------------------
//

#include <elle/network/Slot.hxx>

#endif

//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/idiom/Open.hh
//
// created       julien quintard   [mon mar  8 23:05:41 2010]
// updated       julien quintard   [mon jun 27 07:16:31 2011]
//

//
// ---------- macro-functions -------------------------------------------------
//

///
/// these macro functions are used to make using the Elle library
/// functionalities easier.
///

//
// ---------- elle ------------------------------------------------------------
//

///
/// this macro function is sometimes use to group several parameter which
/// can be pretty useful when a parameter contains a comma for instance.
///
#define _(_parameters_...)						\
  _parameters_

//
// ---------- object ----------------------------------------------------------
//

///
/// this macro generates the prototypes for the fundamental object
/// method.
///
/// for more information please refer to radix/Object.hh
///
#define declare(_type_)							\
  _type_&		operator=(const _type_&);			\
									\
  elle::core::Boolean	operator!=(const _type_&) const;		\
									\
  elle::core::Boolean	operator<=(const _type_&) const;		\
									\
  elle::core::Boolean	operator>=(const _type_&) const;		\
									\
  elle::radix::Status	Imprint(elle::core::Natural32&) const;		\
									\
  elle::radix::Status	Clone(elle::radix::Object*&)			\
    const;

///
/// this macro function makes it easy for classes to derive from Object
/// as only a copy constructor and some operators such as ==, <, > etc.
/// must be manually written, should the class need to provide such
/// operators.
///
/// everything else, assignment, != operator, sizeof etc. are automatically
/// generated by the following.
///
#define embed(_type_, _template_)					\
  _template_								\
  _type_&	_type_::operator=(const _type_&		object)		\
  {									\
    enter();								\
									\
    if (this == &object)						\
      return (*this);							\
									\
    if (this->Recycle(&object) == elle::radix::StatusError)		\
      yield("unable to recycle the object", *this);			\
									\
    return (*this);							\
  }									\
									\
  _template_								\
  elle::core::Boolean	_type_::operator!=(const _type_& object) const	\
  {									\
    return (!(*this == object));					\
  }									\
									\
  _template_								\
  elle::core::Boolean	_type_::operator<=(const _type_& object) const	\
  {									\
    return ((*this == object) || (*this < object));			\
  }									\
									\
  _template_								\
  elle::core::Boolean	_type_::operator>=(const _type_& object) const	\
  {									\
    return ((*this == object) || (*this > object));			\
  }									\
									\
  _template_								\
  elle::radix::Status	_type_::Imprint(elle::core::Natural32& size)	\
    const								\
  {									\
    enter();								\
									\
    size = sizeof (_type_);						\
									\
    leave();								\
  }									\
									\
  _template_								\
  elle::radix::Status	_type_::Clone(					\
                          elle::radix::Object*&				\
			    object)					\
    const								\
  {									\
    enter();								\
									\
    object = new _type_(*this);						\
									\
    leave();								\
  }

//
// ---------- report ----------------------------------------------------------
//

///
/// this macro-function register a report entry.
///
/// note that this macro function should never be called directly. instead
/// the macro functions below should be used: leave, escape, true, alert
/// etc.
///
#define report(_type_, _message_)					\
  do									\
    {									\
      elle::standalone::Report*	_report_;				\
      std::ostringstream	_tmp_;					\
									\
      _tmp_ << __FILE__ << ":" << __LINE__ << " # " << __FUNCTION__;	\
									\
      elle::core::String	_location_(_tmp_.str());		\
      elle::core::String	_time_(__DATE__ " " __TIME__);		\
									\
      if (elle::standalone::Report::Instance(_report_) !=		\
	  elle::radix::StatusError)					\
        _report_->Record(_type_,					\
			 _location_,					\
			 _time_,					\
			 _message_);					\
    } while (false)

///
/// this macro-function is used for successfully returning from
/// a normal method or function.
///
#define leave()								\
  do									\
    {									\
      release();							\
									\
      return (elle::radix::StatusOk);					\
    } while (false)

///
/// this macro-function just returns StatusTrue.
///
#define true()								\
  do									\
    {									\
      release();							\
									\
      return (elle::radix::StatusTrue);					\
    } while (false)

///
/// this macro-function just returns StatusFalse.
///
#define false()								\
  do									\
    {									\
      release();							\
									\
      return (elle::radix::StatusFalse);				\
    } while (false)

///
/// this macro-function reports a warning.
///
#define warn(_text_)							\
  report(elle::standalone::Report::TypeWarning, _text_)

///
/// this macro-function indicates that an error occured
/// and returns StatusError.
///
#define escape(_text_)							\
  do									\
    {									\
      report(elle::standalone::Report::TypeError, _text_);		\
									\
      release();							\
									\
      return (elle::radix::StatusError);				\
    } while (false)

///
/// this macro-function indicates that an error occured
/// and return StatusFalse.
///
#define flee(_text_)							\
  do									\
    {									\
      report(elle::standalone::Report::TypeError, _text_);		\
									\
      release();							\
									\
      return (elle::radix::StatusFalse);				\
    } while (false)							\

///
/// this macro-function reports an error and returns.
///
/// note that the return object is specifed, hence this function
/// perfectly fits when an error occurs in operators etc.
///
#define yield(_text_, _return_)						\
  do									\
    {									\
      report(elle::standalone::Report::TypeError, _text_);		\
									\
      release();							\
      									\
      return (_return_);						\
    } while (false)

///
/// this macro-function reports an error and returns.
///
/// XXX \todo this one should directly print on stderr or later
///     forward the message to a log application.
///
#define alert(_text_, _return_...)					\
  do									\
    {									\
      report(elle::standalone::Report::TypeError, _text_);		\
									\
      show();								\
									\
      release();							\
									\
      return _return_;							\
    } while (false)

///
/// this macro-function adds an failure, displays the stack and
/// stops the program.
///
#define fail(_text_)							\
  do									\
    {									\
      report(elle::standalone::Report::TypeFailure, _text_);		\
									\
      show();								\
									\
      ::exit(EXIT_FAILURE);						\
    } while (false)

///
/// this macro-function displays the error stack on the error output.
///
#define show()								\
  do									\
    {									\
      elle::standalone::Report*	_report_;				\
									\
      if (elle::standalone::Report::Instance(_report_) !=		\
	  elle::radix::StatusError)					\
	{								\
          _report_->Dump();						\
	  _report_->Flush();						\
	}								\
    } while (false)

///
/// this macro-function, in the case of reported errors, displays them
/// and exits. otherwise, the function does not do anything.
///
#define expose()							\
  do									\
    {									\
      elle::standalone::Report*	_report_;				\
									\
      if (elle::standalone::Report::Instance(_report_) !=		\
	  elle::radix::StatusError)					\
	{								\
	  if (_report_->container.empty() == false)			\
	    {								\
	      _report_->Dump();						\
	      _report_->Flush();					\
									\
	      ::exit(EXIT_FAILURE);					\
	    }								\
	}								\
    } while (false)

///
/// this macro-function flushes all the recorded messages.
///
#define purge()								\
  do									\
    {									\
      elle::standalone::Report*	_report_;				\
									\
      if (elle::standalone::Report::Instance(_report_) !=		\
	  elle::radix::StatusError)					\
	_report_->Flush();						\
    } while (false)

//
// ---------- maid ------------------------------------------------------------
//

///
/// this macro function allocates memory within the current scope through
/// the use of alloca() before creating a Garrison of Guards.
///
#define enter(_guards_...)						\
  elle::standalone::Maid::Garrison* _maid_ =				\
    elle::standalone::Maid::Install(					\
      (elle::core::Void*)						\
        alloca(sizeof (elle::standalone::Maid::Garrison)),		\
      ##_guards_)

///
/// this macro wraps a local non-pointer variable by creating a pointer
/// of the same type. this pointer will then be used by the Maid though
/// the developer has to specifically inform the system regarding
/// when to start and stop tracking the variable through the track()
/// and untrack() macro-functions.
///
#define wrap(_name_)							\
  typeof(_name_)*	_ ## _name_ ## _

///
/// this macro function emulates a slab tracking but for local
/// non-pointed variables.
///
#define local(_name_, _function_)					\
  slab(_ ## _name_ ## _, _function_)

///
/// this macro-function starts the tracking by setting it to the
/// variable's address.
///
#define track(_name_)							\
  _ ## _name_ ## _ = &_name_

///
/// unlike the previous one, this macro-function sets the pointer
/// to NULL, hence stopping the tracking process.
///
#define untrack(_name_)							\
  _ ## _name_ ## _ = NULL

///
/// this macro-function is similar to the previous one but in the
/// general case of pointers.
///
#define waive(_pointer_)						\
  _pointer_ = NULL

///
/// this macro-function creates a guard for class instances which will
/// then be relased through the delete statement.
///
#define instance(_pointer_)						\
  elle::standalone::Maid::Monitor(					\
    (elle::core::Void*)							\
      alloca(								\
        sizeof (elle::standalone::Maid::Instance<elle::core::Void*>)),	\
    _pointer_)

///
/// this macro-function creates a guard for slab i.e memory-based variables
/// allocated via specific functions such as malloc(), RSA_new() etc.
///
#define slab(_pointer_, _function_)					\
  elle::standalone::Maid::Monitor(					\
    (elle::core::Void*)							\
      alloca(								\
        sizeof (elle::standalone::Maid::Slab<elle::core::Void*,		\
	                                    elle::core::Void		\
                                              (*)(elle::core::Void*)>)),\
    _pointer_, _function_)

///
/// this macro-function releases the guarded objects cf Maid.
///
#define release()							\
  do									\
    {									\
      if (_maid_ != NULL)						\
	{								\
	  delete _maid_;						\
									\
	  _maid_ = NULL;						\
	}								\
    } while (false)

//
// ---------- message ---------------------------------------------------------
//

///
/// this macro-function defines an inward message part of the
/// interface.
///
#define inward(_tag_, _parameters_)					\
  namespace elle							\
  {									\
    namespace network							\
    {									\
      template <>							\
      class Message< _tag_ >:						\
        public Entity							\
      {									\
      public:								\
        typedef Parameters< _parameters_ >	P;			\
      };								\
    }									\
  }

///
/// this macro-function defines an outward message i.e response
/// to a previously received inward message.
///
/// note that outward messages' tag is the negative counterpart
/// of the corresponding inward message.
///
#define outward(_tag_, _parameters_)					\
  namespace elle							\
  {									\
    namespace network							\
    {									\
      template <>							\
      class Message< _tag_ >:						\
        public Entity							\
      {									\
      public:								\
        typedef Parameters< _parameters_ >	P;			\
      };								\
    }									\
  }

///
/// syntaxic sugar.
///
#define parameters(_parameters_...)					\
  _parameters_

//
// ---------- network ---------------------------------------------------------
//

///
/// this macro-function reserves a range of _capacity_ tags for the
/// _component_ which depends upon the _dependencies_ components.
///
#define range(_component_, _capacity_, _dependency_...)			\
  namespace elle							\
  {									\
    namespace network							\
    {									\
      template <>							\
      struct Range<_component_>:					\
        public Capacity<_capacity_>					\
      {									\
        static const int		First =				\
	  Dependency<_dependency_>::Last + 1;				\
	static const int		Last = First + Size;		\
      };								\
    }									\
  }

//
// ---------- XXX -------------------------------------------------------------
//

/* XXX

Stub:
template <const Tag G, typename... I>
Status		Stub(Bundle<G, Parameters<const I...> >& inputs)
{

}

Skeleton:

Procedure:
template <const Tag G, typename... I, typename... O>
Status		Stub(Bundle<G, Parameters<const I...> >& inputs,
                     Bundle<G, Parameters<O...> >& outputs)
{

}

Function:

procedure(&Object::Load,
	  inward(TagPath,
		 parameters(const Path)))
Status		Object::Load(const Path&		path, // input
			     Identifer&			identifier) // output
{
}

Stub: 
  client.Send(path)
  client.Receive(identifier)

=> Message<Path>
<= Message(Identifier)

Skeleton:
  server.Receive(path)
  server.Send(identifier)

 */

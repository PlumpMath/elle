# include <elle/meta.hh>
# include <elle/test.hh>

using namespace elle::meta;

namespace list
{
  namespace apply
  {
    template <typename ... Args>
    struct res
    {};

    typedef List<int, float, double> l;
    static_assert(std::is_same<typename l::apply<res>::type,
                               res<int, float, double>>::value,
                  "list::apply yielded the wrong type");
  }

  namespace filter
  {
    typedef List<int, std::string, void, float*> l;
    typedef typename l::filter<std::is_scalar>::type res;
    static_assert(std::is_same<res, List<int, float*>>::value,
                  "list::filter yielded the wrong type");
  }

  namespace prepend
  {
    typedef List<int, void> l;
    static_assert(std::is_same<l::prepend<float>::type,
                               List<float, int, void>>::value,
                  "list::prepend yielded the wrong type");
  }
}

static
void
dummy()
{}

ELLE_TEST_SUITE()
{
  auto& master = boost::unit_test::framework::master_test_suite();
  master.add(BOOST_TEST_CASE(dummy));
}
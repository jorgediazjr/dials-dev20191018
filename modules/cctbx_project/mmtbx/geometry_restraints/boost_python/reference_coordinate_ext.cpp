
#include <cctbx/boost_python/flex_fwd.h>
#include <cctbx/geometry_restraints/shared_wrapper_pickle.hpp>

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/args.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/return_by_value.hpp>
#include <boost/optional.hpp>

#include <mmtbx/geometry_restraints/reference_coordinate.h>
#include <cctbx/geometry_restraints/proxy_select.h>
#include <scitbx/array_family/boost_python/shared_wrapper.h>

namespace mmtbx { namespace geometry_restraints {
namespace boost_python {

  struct reference_coordinate_proxies_wrappers : boost::python::pickle_suite
  {
    typedef reference_coordinate_proxy w_t;

    static boost::python::tuple
      getinitargs(w_t const& self)
    {
      return boost::python::make_tuple(self.i_seqs,
        self.ref_sites,
        self.weight,
        self.limit,
        self.top_out
        );
    }

    static void
    wrap()
    {
      using namespace boost::python;
      typedef return_value_policy<return_by_value> rbv;
      typedef reference_coordinate_proxy w_t;
      class_<w_t>("reference_coordinate_proxy", no_init)
        .def(init<
          // This is an array just for compatibility with
          // cctbx::geometry_restraints::shared_proxy_select function!
          af::tiny<unsigned, 1> const&,
          scitbx::vec3<double>, double,
          double, bool>((
            arg("i_seqs"),
            arg("ref_sites"),
            arg("weight"),
            arg("limit") = -1.0,
            arg("top_out") = false)))
        .add_property("i_seqs", make_getter(&w_t::i_seqs, rbv()))
        .add_property("ref_sites", make_getter(&w_t::ref_sites, rbv()))
        .def_readwrite("weight", &w_t::weight)
        .def_pickle(reference_coordinate_proxies_wrappers())
        ;
      {
        typedef return_internal_reference<> rir;
        typedef scitbx::af::boost_python::shared_wrapper<reference_coordinate_proxy, rir> shared_w_t;
        shared_w_t::wrap(
          "shared_reference_coordinate_proxy")
          .def("proxy_select",
            (af::shared<w_t>(*)(
              af::const_ref<w_t> const&,
              std::size_t,
              af::const_ref<std::size_t> const&))
            cctbx::geometry_restraints::shared_proxy_select, (
              arg("n_seq"), arg("iselection")))
          .def("proxy_remove",
            (af::shared<w_t>(*)(
              af::const_ref<w_t> const&,
              af::const_ref<bool> const&))
            cctbx::geometry_restraints::shared_proxy_remove, (
              arg("selection")))
          .def("proxy_remove",
            (af::shared<w_t>(*)(
              af::const_ref<w_t> const&,
              af::const_ref<size_t> const&))
            cctbx::geometry_restraints::shared_proxy_remove, (
              arg("selection")))
          .def_pickle(shared_wrapper_pickle_suite< shared_w_t::w_t >())
          ;
      }

      def("reference_coordinate_residual_sum",
        (double(*)(
          af::const_ref<scitbx::vec3<double> > const&,
          af::const_ref<reference_coordinate_proxy> const&,
          af::ref<scitbx::vec3<double> > const&))
        reference_coordinate_residual_sum, (
          arg("sites_cart"),
          arg("proxies"),
          arg("gradient_array")));

    }
  }
  ;

  void wrap_reference ()
  {
    reference_coordinate_proxies_wrappers::wrap();
  }

}}} // namespace mmtbx::geometry_restraints::boost_python

BOOST_PYTHON_MODULE(mmtbx_reference_coordinate_ext)
{
  mmtbx::geometry_restraints::boost_python::wrap_reference();
}

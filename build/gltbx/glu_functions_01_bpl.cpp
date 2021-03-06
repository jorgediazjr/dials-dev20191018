/* *****************************************************
   THIS IS AN AUTOMATICALLY GENERATED FILE. DO NOT EDIT.
   *****************************************************

   Generated by:
     gltbx.generate_functions_bpl
 */

#include <gltbx/special_wrapper_support.h>
#include <gltbx/pointer_args_bpl.h>
#include <gltbx/error.h>

#if defined(__GNUC__) && __GNUC__ == 2 \
     && __GNUC_MINOR__ == 96 && __GNUC_PATCHLEVEL__ == 0
#define GLTBX_NO_OPAQUE_POINTERS
#else
#include <boost/python/return_value_policy.hpp>
#include <boost/python/return_opaque_pointer.hpp>
BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(GLUnurbs)
BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(GLUquadric)
BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(GLUtesselator)
#endif

namespace gltbx { namespace glu { namespace {

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_DeleteTess(
    boost::python::object const& py_tess)
  {
    boost::python::extract<GLUtesselator*> tess_proxy(py_tess);
    GLUtesselator* tess = tess_proxy();
    gluDeleteTess(tess);
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_Disk(
    boost::python::object const& py_quad,
    boost::python::object const& py_inner,
    boost::python::object const& py_outer,
    boost::python::object const& py_slices,
    boost::python::object const& py_loops)
  {
    boost::python::extract<GLUquadric*> quad_proxy(py_quad);
    GLUquadric* quad = quad_proxy();
    boost::python::extract<GLdouble> inner_proxy(py_inner);
    GLdouble inner = inner_proxy();
    boost::python::extract<GLdouble> outer_proxy(py_outer);
    GLdouble outer = outer_proxy();
    boost::python::extract<GLint> slices_proxy(py_slices);
    GLint slices = slices_proxy();
    boost::python::extract<GLint> loops_proxy(py_loops);
    GLint loops = loops_proxy();
    gluDisk(quad, inner, outer, slices, loops);
  }
#endif

  boost::python::str
  glu_ErrorString(boost::python::object const& py_error)
  {
    boost::python::extract<GLenum> error_proxy(py_error);
    GLenum error = error_proxy();
    return boost::python::str(
      reinterpret_cast<const char*>(gluErrorString(error)));
  }

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_GetNurbsProperty(
    boost::python::object const& py_nurb,
    boost::python::object const& py_property,
    boost::python::object const& py_data)
  {
    boost::python::extract<GLUnurbs*> nurb_proxy(py_nurb);
    GLUnurbs* nurb = nurb_proxy();
    boost::python::extract<GLenum> property_proxy(py_property);
    GLenum property = property_proxy();
    boost_python::converter<GLfloat> data_proxy(
      "data", py_data, 0, false);
    GLfloat* data = data_proxy.get();
    gluGetNurbsProperty(nurb, property, data);
    data_proxy.write_back();
  }
#endif

  boost::python::str
  glu_GetString(boost::python::object const& py_name)
  {
    boost::python::extract<GLenum> name_proxy(py_name);
    GLenum name = name_proxy();
    boost::python::str result(
      reinterpret_cast<const char*>(gluGetString(name)));
    return result;
  }

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_GetTessProperty(
    boost::python::object const& py_tess,
    boost::python::object const& py_which,
    boost::python::object const& py_data)
  {
    boost::python::extract<GLUtesselator*> tess_proxy(py_tess);
    GLUtesselator* tess = tess_proxy();
    boost::python::extract<GLenum> which_proxy(py_which);
    GLenum which = which_proxy();
    boost_python::converter<GLdouble> data_proxy(
      "data", py_data, 0, false);
    GLdouble* data = data_proxy.get();
    gluGetTessProperty(tess, which, data);
    data_proxy.write_back();
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_LoadSamplingMatrices(
    boost::python::object const& py_nurb,
    boost::python::object const& py_model,
    boost::python::object const& py_perspective,
    boost::python::object const& py_view)
  {
    boost::python::extract<GLUnurbs*> nurb_proxy(py_nurb);
    GLUnurbs* nurb = nurb_proxy();
    boost_python::converter<GLfloat> model_proxy(
      "model", py_model, 16, true);
    const GLfloat* model = model_proxy.get();
    boost_python::converter<GLfloat> perspective_proxy(
      "perspective", py_perspective, 16, true);
    const GLfloat* perspective = perspective_proxy.get();
    boost_python::converter<GLint> view_proxy(
      "view", py_view, 4, true);
    const GLint* view = view_proxy.get();
    gluLoadSamplingMatrices(nurb, model, perspective, view);
  }
#endif

  void
  glu_LookAt(
    boost::python::object const& py_eyeX,
    boost::python::object const& py_eyeY,
    boost::python::object const& py_eyeZ,
    boost::python::object const& py_centerX,
    boost::python::object const& py_centerY,
    boost::python::object const& py_centerZ,
    boost::python::object const& py_upX,
    boost::python::object const& py_upY,
    boost::python::object const& py_upZ)
  {
    boost::python::extract<GLdouble> eyeX_proxy(py_eyeX);
    GLdouble eyeX = eyeX_proxy();
    boost::python::extract<GLdouble> eyeY_proxy(py_eyeY);
    GLdouble eyeY = eyeY_proxy();
    boost::python::extract<GLdouble> eyeZ_proxy(py_eyeZ);
    GLdouble eyeZ = eyeZ_proxy();
    boost::python::extract<GLdouble> centerX_proxy(py_centerX);
    GLdouble centerX = centerX_proxy();
    boost::python::extract<GLdouble> centerY_proxy(py_centerY);
    GLdouble centerY = centerY_proxy();
    boost::python::extract<GLdouble> centerZ_proxy(py_centerZ);
    GLdouble centerZ = centerZ_proxy();
    boost::python::extract<GLdouble> upX_proxy(py_upX);
    GLdouble upX = upX_proxy();
    boost::python::extract<GLdouble> upY_proxy(py_upY);
    GLdouble upY = upY_proxy();
    boost::python::extract<GLdouble> upZ_proxy(py_upZ);
    GLdouble upZ = upZ_proxy();
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
  }

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  GLUnurbs*
  glu_NewNurbsRenderer()
  {
    return gluNewNurbsRenderer();
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  GLUquadric*
  glu_NewQuadric()
  {
    return gluNewQuadric();
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  GLUtesselator*
  glu_NewTess()
  {
    return gluNewTess();
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_NextContour(
    boost::python::object const& py_tess,
    boost::python::object const& py_type)
  {
    boost::python::extract<GLUtesselator*> tess_proxy(py_tess);
    GLUtesselator* tess = tess_proxy();
    boost::python::extract<GLenum> type_proxy(py_type);
    GLenum type = type_proxy();
    gluNextContour(tess, type);
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
#if defined(GL_XXX)
  void
  glu_NurbsCallbackDataEXT(
    boost::python::object const& py_nurb,
    boost::python::object const& py_userData)
  {
    boost::python::extract<GLUnurbs*> nurb_proxy(py_nurb);
    GLUnurbs* nurb = nurb_proxy();
    throw std::runtime_error(
      "Conversion not implemented:"
      " gluNurbsCallbackDataEXT(): GLvoid* userData");
  //  GLvoid* userData = 0;
  //  gluNurbsCallbackDataEXT(nurb, userData);
  }
#endif
#endif

} // namespace <anonymous>

namespace boost_python {

  void
  wrap_functions_01()
  {
    using namespace boost::python;
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluDeleteTess", glu_DeleteTess, (
      arg("tess")));
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluDisk", glu_Disk, (
      arg("quad"), arg("inner"), arg("outer"), arg("slices"), arg("loops")));
#endif
    def("gluErrorString", glu_ErrorString, (
      arg("error")));
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluGetNurbsProperty", glu_GetNurbsProperty, (
      arg("nurb"), arg("property"), arg("data")));
#endif
    def("gluGetString", glu_GetString, (
      arg("name")));
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluGetTessProperty", glu_GetTessProperty, (
      arg("tess"), arg("which"), arg("data")));
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluLoadSamplingMatrices", glu_LoadSamplingMatrices, (
      arg("nurb"), arg("model"), arg("perspective"), arg("view")));
#endif
    def("gluLookAt", glu_LookAt, (
      arg("eyeX"), arg("eyeY"), arg("eyeZ"), arg("centerX"), arg("centerY"),
      arg("centerZ"), arg("upX"), arg("upY"), arg("upZ")));
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluNewNurbsRenderer", glu_NewNurbsRenderer,
      return_value_policy<return_opaque_pointer>());
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluNewQuadric", glu_NewQuadric,
      return_value_policy<return_opaque_pointer>());
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluNewTess", glu_NewTess,
      return_value_policy<return_opaque_pointer>());
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluNextContour", glu_NextContour, (
      arg("tess"), arg("type")));
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
#if defined(GL_XXX)
    def("gluNurbsCallbackDataEXT", glu_NurbsCallbackDataEXT, (
      arg("nurb"), arg("userData")));
#endif
#endif
  }

}}} // namespace gltbx::glu::boost_python

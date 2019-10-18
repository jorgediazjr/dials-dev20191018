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
  glu_NurbsCallback(
    boost::python::object const& py_nurb,
    boost::python::object const& py_which,
    boost::python::object const& py_fn)
  {
    boost::python::extract<GLUnurbs*> nurb_proxy(py_nurb);
    GLUnurbs* nurb = nurb_proxy();
    boost::python::extract<GLenum> which_proxy(py_which);
    GLenum which = which_proxy();
    throw std::runtime_error(
      "Conversion not implemented:"
      " gluNurbsCallback(): glu_function_pointer fn");
  //  glu_function_pointer fn = 0;
  //  gluNurbsCallback(nurb, which, fn);
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_NurbsCurve(
    boost::python::object const& py_nurb,
    boost::python::object const& py_knotCount,
    boost::python::object const& py_knots,
    boost::python::object const& py_stride,
    boost::python::object const& py_control,
    boost::python::object const& py_order,
    boost::python::object const& py_type)
  {
    boost::python::extract<GLUnurbs*> nurb_proxy(py_nurb);
    GLUnurbs* nurb = nurb_proxy();
    boost::python::extract<GLint> knotCount_proxy(py_knotCount);
    GLint knotCount = knotCount_proxy();
    boost_python::converter<GLfloat> knots_proxy(
      "knots", py_knots, 0, false);
    GLfloat* knots = knots_proxy.get();
    boost::python::extract<GLint> stride_proxy(py_stride);
    GLint stride = stride_proxy();
    boost_python::converter<GLfloat> control_proxy(
      "control", py_control, 0, false);
    GLfloat* control = control_proxy.get();
    boost::python::extract<GLint> order_proxy(py_order);
    GLint order = order_proxy();
    boost::python::extract<GLenum> type_proxy(py_type);
    GLenum type = type_proxy();
    gluNurbsCurve(nurb, knotCount, knots, stride, control, order, type);
    knots_proxy.write_back();
    control_proxy.write_back();
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_NurbsProperty(
    boost::python::object const& py_nurb,
    boost::python::object const& py_property,
    boost::python::object const& py_value)
  {
    boost::python::extract<GLUnurbs*> nurb_proxy(py_nurb);
    GLUnurbs* nurb = nurb_proxy();
    boost::python::extract<GLenum> property_proxy(py_property);
    GLenum property = property_proxy();
    boost::python::extract<GLfloat> value_proxy(py_value);
    GLfloat value = value_proxy();
    gluNurbsProperty(nurb, property, value);
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_NurbsSurface(
    boost::python::object const& py_nurb,
    boost::python::object const& py_sKnotCount,
    boost::python::object const& py_sKnots,
    boost::python::object const& py_tKnotCount,
    boost::python::object const& py_tKnots,
    boost::python::object const& py_sStride,
    boost::python::object const& py_tStride,
    boost::python::object const& py_control,
    boost::python::object const& py_sOrder,
    boost::python::object const& py_tOrder,
    boost::python::object const& py_type)
  {
    boost::python::extract<GLUnurbs*> nurb_proxy(py_nurb);
    GLUnurbs* nurb = nurb_proxy();
    boost::python::extract<GLint> sKnotCount_proxy(py_sKnotCount);
    GLint sKnotCount = sKnotCount_proxy();
    boost_python::converter<GLfloat> sKnots_proxy(
      "sKnots", py_sKnots, 0, false);
    GLfloat* sKnots = sKnots_proxy.get();
    boost::python::extract<GLint> tKnotCount_proxy(py_tKnotCount);
    GLint tKnotCount = tKnotCount_proxy();
    boost_python::converter<GLfloat> tKnots_proxy(
      "tKnots", py_tKnots, 0, false);
    GLfloat* tKnots = tKnots_proxy.get();
    boost::python::extract<GLint> sStride_proxy(py_sStride);
    GLint sStride = sStride_proxy();
    boost::python::extract<GLint> tStride_proxy(py_tStride);
    GLint tStride = tStride_proxy();
    boost_python::converter<GLfloat> control_proxy(
      "control", py_control, 0, false);
    GLfloat* control = control_proxy.get();
    boost::python::extract<GLint> sOrder_proxy(py_sOrder);
    GLint sOrder = sOrder_proxy();
    boost::python::extract<GLint> tOrder_proxy(py_tOrder);
    GLint tOrder = tOrder_proxy();
    boost::python::extract<GLenum> type_proxy(py_type);
    GLenum type = type_proxy();
    gluNurbsSurface(nurb, sKnotCount, sKnots, tKnotCount, tKnots, sStride,
      tStride, control, sOrder, tOrder, type);
    sKnots_proxy.write_back();
    tKnots_proxy.write_back();
    control_proxy.write_back();
  }
#endif

  void
  glu_Ortho2D(
    boost::python::object const& py_left,
    boost::python::object const& py_right,
    boost::python::object const& py_bottom,
    boost::python::object const& py_top)
  {
    boost::python::extract<GLdouble> left_proxy(py_left);
    GLdouble left = left_proxy();
    boost::python::extract<GLdouble> right_proxy(py_right);
    GLdouble right = right_proxy();
    boost::python::extract<GLdouble> bottom_proxy(py_bottom);
    GLdouble bottom = bottom_proxy();
    boost::python::extract<GLdouble> top_proxy(py_top);
    GLdouble top = top_proxy();
    gluOrtho2D(left, right, bottom, top);
  }

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_PartialDisk(
    boost::python::object const& py_quad,
    boost::python::object const& py_inner,
    boost::python::object const& py_outer,
    boost::python::object const& py_slices,
    boost::python::object const& py_loops,
    boost::python::object const& py_start,
    boost::python::object const& py_sweep)
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
    boost::python::extract<GLdouble> start_proxy(py_start);
    GLdouble start = start_proxy();
    boost::python::extract<GLdouble> sweep_proxy(py_sweep);
    GLdouble sweep = sweep_proxy();
    gluPartialDisk(quad, inner, outer, slices, loops, start, sweep);
  }
#endif

  void
  glu_Perspective(
    boost::python::object const& py_fovy,
    boost::python::object const& py_aspect,
    boost::python::object const& py_zNear,
    boost::python::object const& py_zFar)
  {
    boost::python::extract<GLdouble> fovy_proxy(py_fovy);
    GLdouble fovy = fovy_proxy();
    boost::python::extract<GLdouble> aspect_proxy(py_aspect);
    GLdouble aspect = aspect_proxy();
    boost::python::extract<GLdouble> zNear_proxy(py_zNear);
    GLdouble zNear = zNear_proxy();
    boost::python::extract<GLdouble> zFar_proxy(py_zFar);
    GLdouble zFar = zFar_proxy();
    gluPerspective(fovy, aspect, zNear, zFar);
  }

  void
  glu_PickMatrix(
    boost::python::object const& py_x,
    boost::python::object const& py_y,
    boost::python::object const& py_delX,
    boost::python::object const& py_delY,
    boost::python::object const& py_viewport)
  {
    boost::python::extract<GLdouble> x_proxy(py_x);
    GLdouble x = x_proxy();
    boost::python::extract<GLdouble> y_proxy(py_y);
    GLdouble y = y_proxy();
    boost::python::extract<GLdouble> delX_proxy(py_delX);
    GLdouble delX = delX_proxy();
    boost::python::extract<GLdouble> delY_proxy(py_delY);
    GLdouble delY = delY_proxy();
    boost_python::converter<GLint> viewport_proxy(
      "viewport", py_viewport, 4, false);
    GLint* viewport = viewport_proxy.get();
    gluPickMatrix(x, y, delX, delY, viewport);
    viewport_proxy.write_back();
  }

  GLint
  glu_Project(
    boost::python::object const& py_objX,
    boost::python::object const& py_objY,
    boost::python::object const& py_objZ,
    boost::python::object const& py_model,
    boost::python::object const& py_proj,
    boost::python::object const& py_view,
    boost::python::object const& py_winX,
    boost::python::object const& py_winY,
    boost::python::object const& py_winZ)
  {
    boost::python::extract<GLdouble> objX_proxy(py_objX);
    GLdouble objX = objX_proxy();
    boost::python::extract<GLdouble> objY_proxy(py_objY);
    GLdouble objY = objY_proxy();
    boost::python::extract<GLdouble> objZ_proxy(py_objZ);
    GLdouble objZ = objZ_proxy();
    boost_python::converter<GLdouble> model_proxy(
      "model", py_model, 16, true);
    const GLdouble* model = model_proxy.get();
    boost_python::converter<GLdouble> proj_proxy(
      "proj", py_proj, 16, true);
    const GLdouble* proj = proj_proxy.get();
    boost_python::converter<GLint> view_proxy(
      "view", py_view, 4, true);
    const GLint* view = view_proxy.get();
    boost_python::converter<GLdouble> winX_proxy(
      "winX", py_winX, 1, false);
    GLdouble* winX = winX_proxy.get();
    boost_python::converter<GLdouble> winY_proxy(
      "winY", py_winY, 1, false);
    GLdouble* winY = winY_proxy.get();
    boost_python::converter<GLdouble> winZ_proxy(
      "winZ", py_winZ, 1, false);
    GLdouble* winZ = winZ_proxy.get();
    GLint result = gluProject(objX, objY, objZ, model, proj, view, winX,
      winY, winZ);
    winX_proxy.write_back();
    winY_proxy.write_back();
    winZ_proxy.write_back();
    return result;
  }

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_PwlCurve(
    boost::python::object const& py_nurb,
    boost::python::object const& py_count,
    boost::python::object const& py_data,
    boost::python::object const& py_stride,
    boost::python::object const& py_type)
  {
    boost::python::extract<GLUnurbs*> nurb_proxy(py_nurb);
    GLUnurbs* nurb = nurb_proxy();
    boost::python::extract<GLint> count_proxy(py_count);
    GLint count = count_proxy();
    boost_python::converter<GLfloat> data_proxy(
      "data", py_data, 0, false);
    GLfloat* data = data_proxy.get();
    boost::python::extract<GLint> stride_proxy(py_stride);
    GLint stride = stride_proxy();
    boost::python::extract<GLenum> type_proxy(py_type);
    GLenum type = type_proxy();
    gluPwlCurve(nurb, count, data, stride, type);
    data_proxy.write_back();
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_QuadricCallback(
    boost::python::object const& py_quad,
    boost::python::object const& py_which,
    boost::python::object const& py_fn)
  {
    boost::python::extract<GLUquadric*> quad_proxy(py_quad);
    GLUquadric* quad = quad_proxy();
    boost::python::extract<GLenum> which_proxy(py_which);
    GLenum which = which_proxy();
    throw std::runtime_error(
      "Conversion not implemented:"
      " gluQuadricCallback(): glu_function_pointer fn");
  //  glu_function_pointer fn = 0;
  //  gluQuadricCallback(quad, which, fn);
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_QuadricDrawStyle(
    boost::python::object const& py_quad,
    boost::python::object const& py_draw)
  {
    boost::python::extract<GLUquadric*> quad_proxy(py_quad);
    GLUquadric* quad = quad_proxy();
    boost::python::extract<GLenum> draw_proxy(py_draw);
    GLenum draw = draw_proxy();
    gluQuadricDrawStyle(quad, draw);
  }
#endif

#if !defined(GLTBX_NO_OPAQUE_POINTERS)
  void
  glu_QuadricNormals(
    boost::python::object const& py_quad,
    boost::python::object const& py_normal)
  {
    boost::python::extract<GLUquadric*> quad_proxy(py_quad);
    GLUquadric* quad = quad_proxy();
    boost::python::extract<GLenum> normal_proxy(py_normal);
    GLenum normal = normal_proxy();
    gluQuadricNormals(quad, normal);
  }
#endif

} // namespace <anonymous>

namespace boost_python {

  void
  wrap_functions_02()
  {
    using namespace boost::python;
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluNurbsCallback", glu_NurbsCallback, (
      arg("nurb"), arg("which"), arg("fn")));
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluNurbsCurve", glu_NurbsCurve, (
      arg("nurb"), arg("knotCount"), arg("knots"), arg("stride"),
      arg("control"), arg("order"), arg("type")));
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluNurbsProperty", glu_NurbsProperty, (
      arg("nurb"), arg("property"), arg("value")));
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluNurbsSurface", glu_NurbsSurface, (
      arg("nurb"), arg("sKnotCount"), arg("sKnots"), arg("tKnotCount"),
      arg("tKnots"), arg("sStride"), arg("tStride"), arg("control"),
      arg("sOrder"), arg("tOrder"), arg("type")));
#endif
    def("gluOrtho2D", glu_Ortho2D, (
      arg("left"), arg("right"), arg("bottom"), arg("top")));
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluPartialDisk", glu_PartialDisk, (
      arg("quad"), arg("inner"), arg("outer"), arg("slices"), arg("loops"),
      arg("start"), arg("sweep")));
#endif
    def("gluPerspective", glu_Perspective, (
      arg("fovy"), arg("aspect"), arg("zNear"), arg("zFar")));
    def("gluPickMatrix", glu_PickMatrix, (
      arg("x"), arg("y"), arg("delX"), arg("delY"), arg("viewport")));
    def("gluProject", glu_Project, (
      arg("objX"), arg("objY"), arg("objZ"), arg("model"), arg("proj"),
      arg("view"), arg("winX"), arg("winY"), arg("winZ")));
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluPwlCurve", glu_PwlCurve, (
      arg("nurb"), arg("count"), arg("data"), arg("stride"), arg("type")));
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluQuadricCallback", glu_QuadricCallback, (
      arg("quad"), arg("which"), arg("fn")));
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluQuadricDrawStyle", glu_QuadricDrawStyle, (
      arg("quad"), arg("draw")));
#endif
#if !defined(GLTBX_NO_OPAQUE_POINTERS)
    def("gluQuadricNormals", glu_QuadricNormals, (
      arg("quad"), arg("normal")));
#endif
  }

}}} // namespace gltbx::glu::boost_python
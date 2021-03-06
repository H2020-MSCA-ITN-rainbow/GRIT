#include <configuration.h>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <grit.h>
#include <glue.h>
#include <util.h>

#include "pygrit_sub_range.h"
#include "pygrit_types.h"

#include <memory>

//--- This is needed to use STL containers in Python
PYBIND11_MAKE_OPAQUE(std::vector<double>);

namespace py = pybind11;

namespace pygrit
{
  grit::SimplexSet filter(
                          grit::engine2d_type const & engine
                          , grit::SimplexSet  const & A
                          , char              const * module_name
                          , char              const * name
                          , py::object        const & info
                          )
  { 
    py::object condition = py::module::import(module_name).attr(name);
                                                
    grit::SimplexSet result;

    grit::SimplexSet::simplex0_const_iterator begin0 = A.begin0();
    grit::SimplexSet::simplex0_const_iterator end0   = A.end0();

    for (grit::SimplexSet::simplex0_const_iterator s0 = begin0; s0 != end0; ++s0)
    {
      if (py::bool_(condition(engine,*s0,info)))
        result.insert(*s0);
    }

    grit::SimplexSet::simplex1_const_iterator begin1 = A.begin1();
    grit::SimplexSet::simplex1_const_iterator end1   = A.end1();

    for (grit::SimplexSet::simplex1_const_iterator s1 = begin1; s1 != end1; ++s1)
    {
      if (py::bool_(condition(engine,*s1,info)))
        result.insert(*s1);
    }

    grit::SimplexSet::simplex2_const_iterator begin2 = A.begin2();
    grit::SimplexSet::simplex2_const_iterator end2   = A.end2();

    for (grit::SimplexSet::simplex2_const_iterator s2 = begin2; s2 != end2; ++s2)
    {
      if (py::bool_(condition(engine,*s2,info)))
        result.insert(*s2);
    }

    return result;
  }
}

PYBIND11_MODULE( pygrit, m ) 
{

  /**
   * A generic buffer class.
   * Is used to expose buffer like data from C++ to Python.
   */
  class BufData
  {
  public:

    void *_ptr = nullptr;         ///< Pointer to the underlying storage
    size_t _itemsize = 0;         ///< Size of individual items in bytes
    std::string _format;          ///< For homogeneous buffers, see https://docs.python.org/3/library/struct.html#format-characters
    size_t _ndim = 0;             ///< Number of dimensions
    std::vector<size_t> _shape;   ///< Shape of the buffer (1 entry per dimension)
    std::vector<size_t> _strides; ///< Number of entries between adjacent entries (for each per dimension)

  public:

    BufData(
            void * ptr
            , size_t itemsize
            , std::string const & format
            , size_t ndim
            , std::vector<size_t> const & shape
            , std::vector<size_t> const & strides
            )
    : _ptr(ptr)
    , _itemsize(itemsize)
    , _format(format)
    , _ndim(ndim)
    , _shape(shape)
    , _strides(strides)
    {
    }

    BufData(
            void * ptr
            , size_t itemsize
            , std::string const & format
            , size_t size
            )
    : BufData(ptr
              , itemsize
              , format
              , 1
              , std::vector<size_t> { size }
              , std::vector<size_t> { itemsize }
              )
    {
    }

    /***
     * Specialized constructor that creates 2D arrays of size rows x cols and each entry is of type format.
     */
    BufData(
            void * ptr                   // Raw data stored in row-major format.
            , std::string const & format // The data type of each element
            , size_t rows                // Number of rows
            , size_t cols                // The number of columns
            )
    : BufData(
              ptr
              , sizeof(float)
              , format
              , 2
              , std::vector<size_t> { rows, cols }
              , std::vector<size_t> { sizeof(float)*cols, sizeof(float) }
              )
    {
    }

  };

  py::class_<BufData> BufData_py(m, "BufData", py::buffer_protocol());
  BufData_py.def_buffer([](BufData& self)
                        { return py::buffer_info(
                                                 self._ptr,
                                                 self._itemsize,
                                                 self._format,
                                                 self._ndim,
                                                 self._shape,
                                                 self._strides); }
                        );

  using grit::param_type;
  using grit::engine2d_type;
  using grit::Simplex0;
  using grit::Simplex1;
  using grit::Simplex2;
  using grit::SimplexSet;
  
  using util::ConfigFile;

  using glue::Phase;
  using glue::VERTEX_ATTRIBUTE;
  using glue::EDGE_ATTRIBUTE;
  using glue::FACE_ATTRIBUTE;

  //--- Binding classes from util, grit and glue

  py::class_<ConfigFile>(m, "ConfigFile")
    .def(py::init<>())
    .def("get_value" , (std::string (ConfigFile::*)( std::string const &) const) &ConfigFile::get_value)
    .def("get_value" , (std::string (ConfigFile::*)( std::string const &
                                                   , std::string const &) const) &ConfigFile::get_value)
    .def("get_value" , (std::string (ConfigFile::*)( std::string const &
                                                   , std::string const &
                                                   , std::string const &) const) &ConfigFile::get_value)
    .def("get_value" , (std::string (ConfigFile::*)( std::string const &
                                                   , std::string const &
                                                   , std::string const &
                                                   , std::string const &) const) &ConfigFile::get_value)
    .def("exist"     , &ConfigFile::exist)
    .def("clear"     , &ConfigFile::clear)
    .def("load"      , &ConfigFile::load, py::arg("filename"), py::arg("throw_on_errors") = true);

  py::class_<param_type>(m, "Parameters")
  .def(py::init([]() { return new param_type(); }) )
    .def("number_of_subdomains", (unsigned int const & (param_type::*)() const) &param_type::number_of_subdomains)
    .def("number_of_subdomains", (unsigned int & (param_type::*)()) &param_type::number_of_subdomains)
    .def("set_upper_threshold_attribute", &param_type::set_upper_threshold_attribute)
    .def("set_lower_threshold_attribute", &param_type::set_lower_threshold_attribute);

  py::class_<engine2d_type>(m, "Engine2D")
    .def(py::init<>())
    .def("update", &engine2d_type::update)
    //.def("mesh", (default_grit_types::mesh_impl const & (engine2d_type::*)() const) &engine2d_type::mesh)
    //.def("mesh", (default_grit_types::mesh_impl & (engine2d_type::*)()) &engine2d_type::mesh)
    .def("create_attribute", [](engine2d_type & self, std::string const & name, unsigned int const dim) -> void
       {
         self.attributes().create_attribute( name, dim );
       })
    .def("get_all_simplices", [](engine2d_type & self) -> SimplexSet
       {
         return self.mesh().get_all_simplices();
       })
    .def("in_phase", [](engine2d_type const & self, unsigned int const & label, Simplex0 const & simplex) -> bool
       {
         return grit::InPhase( self.mesh(), label )(simplex); 
       })
    .def("in_phase", [](engine2d_type const & self, unsigned int const & label, Simplex1 const & simplex) -> bool
       {
         return grit::InPhase( self.mesh(), label )(simplex); 
       })
    .def("in_phase", [](engine2d_type const & self, unsigned int const & label, Simplex2 const & simplex) -> bool
       {
         return grit::InPhase(self.mesh(), label)(simplex); 
       })
    .def("is_dimension", [](engine2d_type const & self, unsigned int const & dim, Simplex0 const & simplex) -> bool
       {
         return (dim==0u);
       })
    .def("is_dimension", [](engine2d_type const & self, unsigned int const & dim, Simplex1 const & simplex) -> bool
       {
         return (dim==1u);
       })
    .def("is_dimension", [](engine2d_type const & self, unsigned int const & dim, Simplex2 const & simplex) -> bool
       {
         return (dim==2u);
       })
    .def("filter", [](
                      engine2d_type const & self
                      , SimplexSet  const & A
                      , char        const * module_name
                      , char        const * condition_name
                      , py::object  const & info 
                      ) -> SimplexSet
       { 
         return pygrit::filter(self, A, module_name, condition_name, info);
       }, py::keep_alive<0, 1>());

  py::class_<SimplexSet>(m, "SimplexSet")
    .def(py::init<>());

  py::class_<Simplex0>(m, "Simplex0")
    .def(py::init<>());

  py::class_<Simplex1>(m, "Simplex1")
    .def(py::init<>());

  py::class_<Simplex2>(m, "Simplex2")
    .def(py::init<>());

  py::class_<Phase>(m, "Phase")
    .def(py::init<>())
    .def("get_vertices", [](glue::Phase & self) -> BufData
       {
         return BufData(
                        const_cast<grit::Simplex0*>(self.m_vertices.data())  //Raw pointer to vertices
                        , sizeof(grit::Simplex0)     // Number of bytes for 1 Simplex0 type
                        , std::string("@L")       // 1 unsigned long
                        , self.m_vertices.size()   // Number of vertices
                        );

       }, py::keep_alive<0, 1>())
    .def("get_edges", [](glue::Phase & self) -> BufData
       {
         return BufData(
                        const_cast<glue::Tuple*>(self.m_edges.data())  //Raw pointer to edges
                        , sizeof(glue::Tuple)     // Number of bytes for one Tuple type
                        , std::string("@II")       // 2 unsigned ints
                        , self.m_edges.size()   // Number of triangles
                        );

       }, py::keep_alive<0, 1>())
    .def("get_triangles", [](glue::Phase & self) -> BufData
       {
         return BufData(
                        const_cast<glue::Triplet*>(self.m_triangles.data())  //Raw pointer to triangles
                        , sizeof(glue::Triplet)     // Number of bytes for one Triple type
                        , std::string("@III")       // 3 unsigned ints
                        , self.m_triangles.size()   // Number of triangles
                        );

       }, py::keep_alive<0, 1>());

  py::class_<VERTEX_ATTRIBUTE>(m, "VERTEX_ATTRIBUTE").def(py::init<>());
  py::class_<EDGE_ATTRIBUTE  >(m, "EDGE_ATTRIBUTE"  ).def(py::init<>());
  py::class_<FACE_ATTRIBUTE  >(m, "FACE_ATTRIBUTE"  ).def(py::init<>());

  //--- Binding functions from util, grit and glue
  m.def("get_data_file_path"              , &util::get_data_file_path                                   );
  m.def("generate_filename"               , &util::generate_filename                                    );
  
  m.def("make_parameters_from_config_file", &grit::make_parameters_from_config_file                     );
  m.def("init_engine_with_mesh_file"      , &grit::init_engine_with_mesh_file<grit::default_grit_types> );

  m.def("svg_draw"                        , &glue::svg_draw 
                                          , py::arg("filename")
                                          , py::arg("engine")
                                          , py::arg("parameters")
                                          , py::arg("attribute_name_x") = "current"
                                          , py::arg("attribute_name_y") = "current"                     );

  m.def("make_phase"                      , (Phase (*)(
                                                       engine2d_type  const &
                                                       , unsigned int const &
                                                       )) &glue::make_phase                             );

  m.def("make_phase"                      , (Phase (*)(
                                                       engine2d_type  const &
                                                       , SimplexSet   const &
                                                       )) &glue::make_phase                             );

  m.def("get_sub_range"                   , (void (*)(
                                                      engine2d_type                   const &
                                                      , Phase                         const &
                                                      , std::string                   const &
                                                      , pygrit::py_dense_array_double       &
                                                      , VERTEX_ATTRIBUTE              const &
                                                      )) &pygrit::get_sub_range                         );

  m.def("get_sub_range"                   , (void (*)(
                                                      engine2d_type                   const &
                                                      , Phase                         const &
                                                      , std::string                   const &
                                                      , pygrit::py_dense_array_double       &
                                                      , EDGE_ATTRIBUTE                const &
                                                      )) &pygrit::get_sub_range                         );

  m.def("get_sub_range"                   , (void (*)(
                                                      engine2d_type                   const &
                                                      , Phase                         const &
                                                      , std::string                   const &
                                                      , pygrit::py_dense_array_double       &
                                                      , FACE_ATTRIBUTE                const &
                                                      )) &pygrit::get_sub_range                         );

  m.def("get_sub_range_current"           , &pygrit::get_sub_range_current                              );
  m.def("get_sub_range_target"            , &pygrit::get_sub_range_target                               );

  m.def("set_sub_range"                   , (void (*)(
                                                      engine2d_type                         &
                                                      , Phase                         const &
                                                      , std::string                   const &
                                                      , pygrit::py_dense_array_double const &
                                                      , VERTEX_ATTRIBUTE              const &
                                                      )) &pygrit::set_sub_range                         );

  m.def("set_sub_range"                   , (void (*)(
                                                      engine2d_type                         &
                                                      , Phase                         const &
                                                      , std::string                   const &
                                                      , pygrit::py_dense_array_double const &
                                                      , EDGE_ATTRIBUTE                const &
                                                      )) &pygrit::set_sub_range                         );
  
  m.def("set_sub_range"                   , (void (*)(
                                                      engine2d_type                         &
                                                      , Phase                         const &
                                                      , std::string                   const &
                                                      , pygrit::py_dense_array_double const &
                                                      , FACE_ATTRIBUTE                const &
                                                      )) &pygrit::set_sub_range                         );

  m.def("set_sub_range_current"           , &pygrit::set_sub_range_current                              );
  m.def("set_sub_range_target"            , &pygrit::set_sub_range_target  
                                          , py::arg("engine")
                                          , py::arg("phase")
                                          , py::arg("x")
                                          , py::arg("y")
                                          , py::arg("using_partial_data") = false                       );

  m.def("clear_attribute"                 , (void (*)(
                                                      grit::engine2d_type       &
                                                      , std::string       const & 
                                                      , double            const &
                                                      , VERTEX_ATTRIBUTE  const & 
                                                      )) &glue::clear_attribute                         );

  m.def("clear_attribute"                 , (void (*)(
                                                      grit::engine2d_type       &
                                                      , std::string       const & 
                                                      , double            const &
                                                      , EDGE_ATTRIBUTE    const & 
                                                      )) &glue::clear_attribute                         );

  m.def("clear_attribute"                 , (void (*)(
                                                      grit::engine2d_type       &
                                                      , std::string       const & 
                                                      , double            const &
                                                      , FACE_ATTRIBUTE    const & 
                                                      )) &glue::clear_attribute                         );
}

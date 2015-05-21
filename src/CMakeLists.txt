cmake_minimum_required(VERSION 2.8)
 
PROJECT(Froggy3D)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(Froggy3D MACOSX_BUNDLE Froggy3D)
 
if(VTK_LIBRARIES)
  target_link_libraries(Froggy3D ${VTK_LIBRARIES})
else()
  target_link_libraries(Froggy3D vtkHybrid vtkWidgets)
endif()
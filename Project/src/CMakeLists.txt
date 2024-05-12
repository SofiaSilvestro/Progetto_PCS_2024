#Aggiungo i file che seguono (AGGIUNTA)
list(APPEND DFN_sources "${CMAKE_CURRENT_SOURCE_DIR}/Polygons.cpp")
list(APPEND DFN_headers "${CMAKE_CURRENT_SOURCE_DIR}/Polygons.hpp")
list(APPEND DFN_sources "${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp")
list(APPEND DFN_headers "${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp")

set(DFN_sources ${DFN_sources} PARENT_SCOPE)
set(DFN_headers ${DFN_headers} PARENT_SCOPE)
set(DFN_includes ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

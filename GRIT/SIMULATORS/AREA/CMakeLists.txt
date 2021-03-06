INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/GRIT/include
  ${PROJECT_SOURCE_DIR}/GLUE/include
  ${PROJECT_SOURCE_DIR}/UTIL/UTIL/include
  include
  )

FILE(GLOB
  AREA_SOURCES 
  src/*.cpp
  )

FILE(GLOB
  AREA_HEADERS
  include/*.h
  )

ADD_LIBRARY(
  area
  ${AREA_SOURCES}
  ${AREA_HEADERS}
  )

TARGET_LINK_LIBRARIES(
  area
  grit
  glue
  util
  )

IF(CMAKE_GENERATOR MATCHES Xcode)		
  SET_TARGET_PROPERTIES(area PROPERTIES XCODE_ATTRIBUTE_WARNING_CFLAGS "-Wall")
ENDIF(CMAKE_GENERATOR MATCHES Xcode)

SET_TARGET_PROPERTIES(area PROPERTIES DEBUG_POSTFIX "D" )
SET_TARGET_PROPERTIES(area PROPERTIES ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_SOURCE_DIR}/lib/")
SET_TARGET_PROPERTIES(area PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_SOURCE_DIR}/lib/")
SET_TARGET_PROPERTIES(area PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_SOURCE_DIR}/lib/")
SET_TARGET_PROPERTIES(area PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_SOURCE_DIR}/lib/")
SET_TARGET_PROPERTIES(area PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/")

set(target intersectionsplitter)

##########################################################################################################
# Lib Setup
##########################################################################################################

set(SOURCES
    src/IntersectionSplitter.cpp
    src/LineSegment.cpp
    src/IntersectionSplitterUtils.cpp
    src/StatusStructure.cpp
    src/EventQueue.cpp
    src/SegmentSet.cpp
    src/PrintUtils.cpp
)

set(PUBLIC_HEADER
    include/intersectionsplitter/intersectionsplitter_api.h
    include/intersectionsplitter/IntersectionSplitter.h
    include/intersectionsplitter/LineSegment.h
    include/intersectionsplitter/PrintUtils.h
)

set(HEADER
    ${PUBLIC_HEADER}

    include/intersectionsplitter/IntersectionSplitterUtils.h
    include/intersectionsplitter/StatusStructure.h
    include/intersectionsplitter/EventQueue.h
    include/intersectionsplitter/SegmentSet.h
)

add_library(${target} ${LIBRARY_TYPE} ${HEADER} ${SOURCES})


include_directories(
    ${PROJECT_BINARY_DIR}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE             CXX
    FOLDER                      ""
    COMPILE_DEFINITIONS_DEBUG    "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE  "${DEFAULT_COMPILE_DEFS_RELEASE}"  
    LINK_FLAGS_DEBUG             "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE           "${DEFAULT_LINKER_FLAGS_RELEASE}"   
    STATIC_LIBRARY_FLAGS_DEBUG   "${DEFAULT_LIBRARIAN_FLAGS_DEBUG}"
    STATIC_LIBRARY_FLAGS_RELEASE "${DEFAULT_LIBRARIAN_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d")


target_compile_options(${target} PRIVATE "${DEFAULT_COMPILE_FLAGS}")

IF (BUILD_TESTS)
    add_subdirectory(tests)
ENDIF()

add_definitions(-DINTERSECTIONSPLITTER_EXPORTS)
install_module(${target})

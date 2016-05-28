set(target intersectionsplitter)

##########################################################################################################
# Lib Setup
##########################################################################################################

set(SOURCES
    intersectionsplitter.cpp
    linesegment.cpp
    intersectionsplitterutils.cpp
    statusstructure.cpp
    eventqueue.cpp
    segmentset.cpp
    printutils.cpp
)

set(PUBLIC_HEADER
    intersectionsplitter_api.h
    intersectionsplitter.h
    linesegment.h
    printutils.h
)

set(HEADER
    ${PUBLIC_HEADER}

    intersectionsplitterutils.h
    statusstructure.h
    eventqueue.h
    segmentset.h
)

add_library(${target} ${HEADER} ${SOURCES})


set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE             CXX
    FOLDER                      ""
    COMPILE_DEFINITIONS                 "${DEFAULT_COMPILE_DEFS}"
    COMPILE_DEFINITIONS_DEBUG           "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE         "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_DEFINITIONS_RELWITHDEBINFO  "${DEFAULT_COMPILE_DEFS_RELWITHDEBINFO}"
    LINK_FLAGS                          "${DEFAULT_LINKER_FLAGS}"
    LINK_FLAGS_DEBUG                    "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE                  "${DEFAULT_LINKER_FLAGS_RELEASE}"
    LINK_FLAGS_RELWITHDEBINFO           "${DEFAULT_LINKER_FLAGS_RELWITHDEBINFO}"
    STATIC_LIBRARY_FLAGS_DEBUG          "${DEFAULT_LIBRARIAN_FLAGS_DEBUG}"
    STATIC_LIBRARY_FLAGS_RELEASE        "${DEFAULT_LIBRARIAN_FLAGS_RELEASE}"
    STATIC_LIBRARY_FLAGS_RELWITHDEBINFO "${DEFAULT_LIBRARIAN_FLAGS_RELWITHDEBINFO}"
    DEBUG_POSTFIX               "d")


target_compile_options(${target} PRIVATE "${DEFAULT_COMPILE_FLAGS}")

IF (BUILD_TESTS)
    add_subdirectory(tests)
ENDIF()

add_definitions(-DINTERSECTIONSPLITTER_EXPORTS)
install_module(${target})

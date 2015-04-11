set(target intersectionsplitter-tests)

include_directories(
    ${GTEST_INCLUDE_DIR}
    ${GMOCK_INCLUDE_DIR}
)

set(sources
    ../src/IntersectionSplitter.cpp
    ../src/LineSegment.cpp
    ../src/IntersectionSplitterUtils.cpp
    ../src/StatusStructure.cpp
    ../src/EventQueue.cpp
    ../src/SegmentSet.cpp
    ../src/PrintUtils.cpp

    IntersectionSplitterTest.cpp
    StatusStructureTest.cpp
    IntersectionSplitterRegressionTest.cpp
    TestUtils.hpp
)

source_group(tests REGULAR_EXPRESSION  ".*([.]h|[.]cpp|[.]hpp)")
source_group(qmocks REGULAR_EXPRESSION  ".*automoc.*([.]h|[.]cpp|[.]hpp)")
source_group(mocks REGULAR_EXPRESSION  "mocks/.*")

add_executable(${target} ${sources})

add_test(${target} ${target})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE             CXX
    FOLDER                      ""
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"  
    DEBUG_POSTFIX               "") # No debug postfix so that gtest runner for visual studio finds the test in debug mode


target_link_libraries(${target}
    gmock
    gmock_main
)

IF(UNIX)
	target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_FLAGS} -Wno-conversion)
ELSE()
	target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_FLAGS})
ENDIF()


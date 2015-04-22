set(target benchmarks)

set(SOURCES
    intersectionsplitterbenchmark.cpp
)

include_directories(
    ${HAYAI_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/intersectionsplitter/include
)

add_executable(${target} ${SOURCES})

target_link_libraries(${target}
    ${HAYAI_LIBRARIES}
    intersectionsplitter
)

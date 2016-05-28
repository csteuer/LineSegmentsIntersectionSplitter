set(target visualeditor)

##########################################################################################################
# Executable Setup
##########################################################################################################

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

QT5_WRAP_UI(UI_HEADER
    mainwindow.ui
)

set(SOURCES
    ${UI_HEADER}

    visualeditor.cpp
    mainwindow.h
    mainwindow.cpp
    interactivegraphicsscene.h
    interactivegraphicsscene.cpp
    graphicsview.h
    graphicsview.cpp
    graphicallinesegment.h
    graphicallinesegment.cpp
    utils.hpp
)

add_executable(${target} ${SOURCES})

qt5_use_modules(${target} Core Gui Widgets)
include_directories(
    ${CMAKE_SOURCE_DIR}
)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      ""
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d")

target_link_libraries(${target}
    intersectionsplitter
)

target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_FLAGS})

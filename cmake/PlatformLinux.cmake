message(STATUS "Configuring for platform Linux/GCC.")


# Enable C++11 support

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
	OUTPUT_VARIABLE GCC_VERSION)

if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
elseif(GCC_VERSION VERSION_EQUAL 4.6)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

set(LINUX_COMPILE_DEFS
	LINUX	                  # Linux system
        PIC		          # Position-independent code
	_REENTRANT                # Reentrant code
)
set(DEFAULT_COMPILE_DEFS_DEBUG
    ${LINUX_COMPILE_DEFS}
    _DEBUG	                  # Debug build
)
set(DEFAULT_COMPILE_DEFS_RELEASE
    ${LINUX_COMPILE_DEFS}
    NDEBUG	                  # Release build
)

set(LINUX_COMPILE_FLAGS
    -fvisibility=hidden
    -pthread
    -pipe
    -fPIC
    -fno-rtti
    -Wconversion
    -Wall
    -Wextra
    -pedantic
    -Werror
)

set(LINUX_TEST_COMPILE_FLAGS
    -fvisibility=hidden
    -pthread
    -pipe
    -fPIC
    -fno-rtti
    -Werror
    -Wall
    -Wextra
    -pedantic
    -Wno-sign-compare
)

if(CMAKE_COMPILER_IS_GNUCXX)
    # gcc
        set(LINUX_COMPILE_FLAGS
            ${LINUX_COMPILE_FLAGS}
            -Wtrampolines
            -Wfloat-equal
            -Wcast-qual
            -Wcast-align
        )
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # clang
    set(LINUX_COMPILE_FLAGS
        ${LINUX_COMPILE_FLAGS}
        -Weverything
        -Wno-sign-conversion
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-padded
)
endif()

set(LINUX_LINKER_FLAGS "-pthread")

set(DEFAULT_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})

set(DEFAULT_LINKER_FLAGS_RELWITHDEBINFO ${LINUX_LINKER_FLAGS})
set(DEFAULT_LINKER_FLAGS_RELEASE ${LINUX_LINKER_FLAGS})
set(DEFAULT_LINKER_FLAGS_DEBUG ${LINUX_LINKER_FLAGS})
set(DEFAULT_LINKER_FLAGS ${LINUX_LINKER_FLAGS})


# Add platform specific libraries for linking
set(EXTRA_LIBS "")

# LineSegmentsIntersectionSplitter

Calculates the intersections of a set of line segments and splits the line segments at their intersections.
Implements a modified version of the Bentley–Ottmann algorithm described in [de Berg, Mark, et al. Computational Geometry: Algorithms and Applications (2008)](http://link.springer.com/book/10.1007/978-3-540-77974-2).

## Dependencies

Requires gcc 4.7+ or clang 3.1+, cmake 2.8.12+, [gmock 1.7+](https://code.google.com/p/googlemock/) for building the tests (optional),
[hayai](https://github.com/nickbruun/hayai) for building the benchmarks (optional) and doxygen for generating the documentatíon (also optional).

## How to build

In the project root run:
~~~~~~~~~~~~~{.txt}
mkdir build
cd build
cmake ..
make
~~~~~~~~~~~~~
To install the library at the location defined by the `CMAKE_INSTALL_PREFIX` variable run `make install` inside the `build` directory.

Set `STATIC_INTERSECTIONSPLITTER=ON` to build a static library (shared by default).

Run `make doc` to generate the documentation (doxygen must be installed and and found by cmake).

### Build and run the tests

Clone [googletest](https://github.com/google/googletest) into `<project-root>/googletest` and set the cmake variable `BUILD_TESTS=ON`:

~~~~~~~~~~~~~{.sh}
git clone https://github.com/google/googletest.git
cd build
cmake .. -DBUILD_TESTS=ON
./intersectionsplitter/tests/intersectionsplitter-tests
~~~~~~~~~~~~~

### Build and run the benchmarks

Install [hayai](https://github.com/nickbruun/hayai), set the cmake variable `BUILD_BENCHMARKS=ON` then run the benchmark executable.

## How to use

~~~~~~~~~~~~~{.cpp}
#include <intersectionsplitter/intersectionsplitter.h>
#include <intersectionsplitter/printutils.h>

int main(int, char**) {

    std::vector<intersectionsplitter::LineSegmentPtr> input = {
        intersectionsplitter::LineSegment::create(0,2, 4,2),
        intersectionsplitter::LineSegment::create(2,4, 2,0)
    };

    std::cout << "Splitting: " << std::endl << input << std::endl;

    std::vector<intersectionsplitter::LineSegmentPtr> result = intersectionsplitter::splitLineSegmentsAtIntersections(input);

    std::cout << "Result: " << std::endl << result;

    return 0;

}

~~~~~~~~~~~~~

Output:
~~~~~~~~~~~~~{.txt}
Splitting:
[s: (0, 2), e: (4, 2), len: 4]
[s: (2, 4), e: (2, 0), len: 4]

Result:
[s: (2, 2), e: (2, 0), len: 2]
[s: (2, 2), e: (4, 2), len: 2]
[s: (2, 4), e: (2, 2), len: 2]
[s: (0, 2), e: (2, 2), len: 2]
~~~~~~~~~~~~~

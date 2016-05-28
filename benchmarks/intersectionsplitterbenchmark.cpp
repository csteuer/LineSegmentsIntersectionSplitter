#include <hayai.hpp>

#include <vector>

#include <intersectionsplitter/intersectionsplitter.h>

#define SEGMENT(x1, y1, x2, y2) intersectionsplitter::LineSegment::create((float) x1, (float) y1, (float) x2, (float) y2)

std::vector<intersectionsplitter::LineSegmentPtr> input = {
        SEGMENT(0, 0, 680, 680),
        SEGMENT(390, 153, 388, 388),
        SEGMENT(234, 157, 520, 520),
        SEGMENT(188, 374, -9, -9),
        SEGMENT(-249, 367, 138, 138),
        SEGMENT(-19, 11, 198, 198),
        SEGMENT(260, 44, 71, 71),
        SEGMENT(169, 58, 264, 264),
        SEGMENT(141, 526, 726, 726),
        SEGMENT(282, 453, 613, 613),
        SEGMENT(382, 207, 553, 553),
        SEGMENT(419, 475, 494, 494),
        SEGMENT(733, 409, 516, 516),
        SEGMENT(482, 465, 796, 796),
        SEGMENT(577, -111, 609, 609),
        SEGMENT(513, 32, 810, 810),
        SEGMENT(584, 170, 856, 856),
        SEGMENT(644, -117, 672, 672),
        SEGMENT(-169, 209, 190, 190),
        SEGMENT(-26, 537, 478, 478),
        SEGMENT(333, 491, 266, 266),
        SEGMENT(-341, 253, -127, -127),
        SEGMENT(-246, 496, -72, -72),
        SEGMENT(134, -196, -180, -180),
        SEGMENT(-166, -159, -83, -83),
        SEGMENT(-203, 139, -38, -38),
        SEGMENT(221, -84, -59, -59),
        SEGMENT(-63, 198, 87, 87),
        SEGMENT(163, 167, 189, 189),
        SEGMENT(100, 434, 213, 213),
        SEGMENT(538, 155, 436, 436),
        SEGMENT(323, 74, 310, 310),
        SEGMENT(304, 353, 494, 494),
        SEGMENT(387, 348, 392, 392),
        SEGMENT(555, 404, 588, 588),
        SEGMENT(635, 536, 718, 718),
        SEGMENT(721, 257, 836, 836),
        SEGMENT(801, -16, 925, 925),
        SEGMENT(836, 336, 1038, 1038),
        SEGMENT(929, 238, 906, 906),
        SEGMENT(829, 237, 988, 988),
        SEGMENT(1011, 194, 676, 676),
        SEGMENT(429, 600, 807, 807),
        SEGMENT(448, 551, 505, 505),
        SEGMENT(95, 609, -197, -197),
        SEGMENT(-132, 552, 157, 157),
        SEGMENT(34, 464, 81, 81),
        SEGMENT(-59, 453, -95, -95),
        SEGMENT(-300, 569, -175, -175),
        SEGMENT(1015, -144, 1111, 1111),
        SEGMENT(1160, -135, 1256, 1256),
        SEGMENT(960, 110, 1114, 1114),
        SEGMENT(1001, -206, 1246, 1246),
        SEGMENT(1248, -155, 1122, 1122)
};

BENCHMARK(IntersectionSplitterBenchmark, NoExtendedIntersectionDistance, 100, 1) {
    intersectionsplitter::splitLineSegmentsAtIntersections(input);
}

BENCHMARK(IntersectionSplitterBenchmark, WithExtendedIntersectionDistance, 100, 1) {
    intersectionsplitter::splitLineSegmentsAtIntersections(input, 10.f);
}

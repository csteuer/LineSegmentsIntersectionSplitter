#include <gmock/gmock.h>

#include <intersectionsplitter/IntersectionSplitter.h>

#include "TestUtils.hpp"

#include <vector>

using namespace intersectionsplitter;

TEST(IntersectionSplitterTestRegressionTest, regression1) {

    std::vector<LineSegmentPtr> input = {
        Segment(234, 374, 234, 332),
        Segment(245, 362, 272, 362),
        Segment(272, 357, 246, 357),
        Segment(246, 357, 246, 328),
        Segment(246, 364, 246, 401),
        Segment(225, 393, 225, 374),
        Segment(233, 398, 233, 384),
        Segment(231, 402, 242, 402),
        Segment(239, 335, 245, 328),
        Segment(243, 332, 235, 332),
        Segment(243, 401, 237, 395),
        Segment(233, 374, 225, 374),
        Segment(241, 371, 241, 376),
        Segment(272, 362, 272, 357),
        Segment(230, 406, 232, 405)
    };

    std::vector<LineSegmentPtr> splitResult = splitLineSegmentsAtIntersections(input);

    ASSERT_THAT(splitResult, testing::UnorderedElementsAreArray({
        SegmentMatch(234, 374, 234, 332),
        SegmentMatch(245, 362, 272, 362),
        SegmentMatch(272, 357, 246, 357),
        SegmentMatch(246, 357, 246, 328),
        SegmentMatch(246, 364, 246, 401),
        SegmentMatch(225, 393, 225, 374),
        SegmentMatch(233, 398, 233, 384),
        SegmentMatch(231, 402, 242, 402),

        SegmentMatch(239, 335,           241.57143f, 332),
        SegmentMatch(241.57143f, 332,    245, 328),

        SegmentMatch(243, 332,           241.57143f, 332),
        SegmentMatch(241.57143f, 332,    235, 332),

        SegmentMatch(243, 401, 237, 395),
        SegmentMatch(233, 374, 225, 374),
        SegmentMatch(241, 371, 241, 376),
        SegmentMatch(272, 362, 272, 357),
        SegmentMatch(230, 406, 232, 405)
    }));

}

TEST(IntersectionSplitterTestRegressionTest, regression2) {

    std::vector<LineSegmentPtr> input = {
        Segment(47, 430, 47, 447),
        Segment(58, 446, 58, 432),
        Segment(50, 446, 57, 446),
        Segment(54, 431, 49, 431)
    };

    std::vector<LineSegmentPtr> splitResult = splitLineSegmentsAtIntersections(input, 4.f);

    ASSERT_THAT(splitResult, testing::UnorderedElementsAreArray({
        SegmentMatch(47, 431, 47, 446),
        SegmentMatch(47, 431, 47, 430),
        SegmentMatch(47, 446, 47, 447),

        SegmentMatch(47, 446, 50, 446),
        SegmentMatch(50, 446, 57, 446),
        SegmentMatch(57, 446, 58, 446),

        SegmentMatch(58, 446, 58, 432),
        SegmentMatch(58, 432, 58, 431),

        SegmentMatch(47, 431, 49, 431),
        SegmentMatch(54, 431, 49, 431),
        SegmentMatch(54, 431, 58, 431)
    }));
}

TEST(IntersectionSplitterTestRegressionTest, regression3) {

    std::vector<LineSegmentPtr> input = {
        Segment(9, 483, 47, 483),
        Segment(5, 485, 5, 510),
        Segment(47, 483, 47, 459),
        Segment(16, 494, 48, 494),
        Segment(60, 481, 47, 495),
        Segment(16, 494, 16, 508),
        Segment(58, 469, 58, 460),
        Segment(49, 460, 57, 460),
        Segment(6, 509, 15, 509),
        Segment(8, 487, 11, 484),
        Segment(58, 481, 58, 477),
        Segment(57, 464, 58, 465)
    };

    std::vector<LineSegmentPtr> splitResult = splitLineSegmentsAtIntersections(input, 4.f);

    ASSERT_THAT(splitResult, testing::UnorderedElementsAreArray({
        SegmentMatch(5, 509, 6, 509),
        SegmentMatch(6, 509, 15, 509),
        SegmentMatch(15, 509, 16, 509),
        SegmentMatch(16, 509, 16, 508),
        SegmentMatch(16, 508, 16, 494),
        SegmentMatch(16, 494, 48, 494),
        SegmentMatch(47, 495, 47.9286, 494),
        SegmentMatch(47.9286, 494, 58, 483.153846153846),
        SegmentMatch(58, 483.153846153846, 60, 481),
        SegmentMatch(58, 483.153846153846, 58, 481),
        SegmentMatch(58, 481, 58, 477),
        SegmentMatch(58, 469, 58, 465),
        SegmentMatch(58, 465, 58, 460),
        SegmentMatch(58, 465, 57, 464),
        SegmentMatch(58, 460, 57, 460),
        SegmentMatch(57, 460, 49, 460),
        SegmentMatch(49, 460, 47, 460),
        SegmentMatch(47, 460, 47, 459),
        SegmentMatch(47, 460, 47, 483),
        SegmentMatch(47, 483, 12, 483),
        SegmentMatch(12, 483, 9, 483),
        SegmentMatch(12, 483, 11, 484),
        SegmentMatch(11, 484, 8, 487),
        SegmentMatch(9, 483, 5, 483),
        SegmentMatch(5, 483, 5, 485),
        SegmentMatch(5, 485, 5, 509),
        SegmentMatch(5, 509, 5, 510),
    }));
}

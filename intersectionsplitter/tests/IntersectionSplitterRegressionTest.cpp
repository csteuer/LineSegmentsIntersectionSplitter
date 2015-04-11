#include <gmock/gmock.h>

#include <intersectionsplitter/IntersectionSplitter.h>

#include "TestUtils.hpp"

#include <vector>

using namespace intersectionsplitter;

TEST(IntersectionSplitterTestRegressionTest, regression1) {

    std::vector<LineSegmentPtr> input = {
        Fragment(234, 374, 234, 332),
        Fragment(245, 362, 272, 362),
        Fragment(272, 357, 246, 357),
        Fragment(246, 357, 246, 328),
        Fragment(246, 364, 246, 401),
        Fragment(225, 393, 225, 374),
        Fragment(233, 398, 233, 384),
        Fragment(231, 402, 242, 402),
        Fragment(239, 335, 245, 328),
        Fragment(243, 332, 235, 332),
        Fragment(243, 401, 237, 395),
        Fragment(233, 374, 225, 374),
        Fragment(241, 371, 241, 376),
        Fragment(272, 362, 272, 357),
        Fragment(230, 406, 232, 405)
    };

    std::vector<LineSegmentPtr> splitResult = splitLineSegmentsAtIntersections(input);

    ASSERT_THAT(splitResult, testing::UnorderedElementsAreArray({
        FragmentMatch(234, 374, 234, 332),
        FragmentMatch(245, 362, 272, 362),
        FragmentMatch(272, 357, 246, 357),
        FragmentMatch(246, 357, 246, 328),
        FragmentMatch(246, 364, 246, 401),
        FragmentMatch(225, 393, 225, 374),
        FragmentMatch(233, 398, 233, 384),
        FragmentMatch(231, 402, 242, 402),

        FragmentMatch(239, 335,           241.57143f, 332),
        FragmentMatch(241.57143f, 332,    245, 328),

        FragmentMatch(243, 332,           241.57143f, 332),
        FragmentMatch(241.57143f, 332,    235, 332),

        FragmentMatch(243, 401, 237, 395),
        FragmentMatch(233, 374, 225, 374),
        FragmentMatch(241, 371, 241, 376),
        FragmentMatch(272, 362, 272, 357),
        FragmentMatch(230, 406, 232, 405)
    }));

}

TEST(IntersectionSplitterTestRegressionTest, regression2) {

    std::vector<LineSegmentPtr> input = {
        Fragment(47, 430, 47, 447),
        Fragment(58, 446, 58, 432),
        Fragment(50, 446, 57, 446),
        Fragment(54, 431, 49, 431)
    };

    std::vector<LineSegmentPtr> splitResult = splitLineSegmentsAtIntersections(input, 4.f);

    ASSERT_THAT(splitResult, testing::UnorderedElementsAreArray({
        FragmentMatch(47, 431, 47, 446),
        FragmentMatch(47, 431, 47, 430),
        FragmentMatch(47, 446, 47, 447),

        FragmentMatch(47, 446, 50, 446),
        FragmentMatch(50, 446, 57, 446),
        FragmentMatch(57, 446, 58, 446),

        FragmentMatch(58, 446, 58, 432),
        FragmentMatch(58, 432, 58, 431),

        FragmentMatch(47, 431, 49, 431),
        FragmentMatch(54, 431, 49, 431),
        FragmentMatch(54, 431, 58, 431)
    }));
}

TEST(IntersectionSplitterTestRegressionTest, regression3) {

    std::vector<LineSegmentPtr> input = {
        Fragment(9, 483, 47, 483),
        Fragment(5, 485, 5, 510),
        Fragment(47, 483, 47, 459),
        Fragment(16, 494, 48, 494),
        Fragment(60, 481, 47, 495),
        Fragment(16, 494, 16, 508),
        Fragment(58, 469, 58, 460),
        Fragment(49, 460, 57, 460),
        Fragment(6, 509, 15, 509),
        Fragment(8, 487, 11, 484),
        Fragment(58, 481, 58, 477),
        Fragment(57, 464, 58, 465)
    };

    std::vector<LineSegmentPtr> splitResult = splitLineSegmentsAtIntersections(input, 4.f);

    ASSERT_THAT(splitResult, testing::UnorderedElementsAreArray({
        FragmentMatch(5, 509, 6, 509),
        FragmentMatch(6, 509, 15, 509),
        FragmentMatch(15, 509, 16, 509),
        FragmentMatch(16, 509, 16, 508),
        FragmentMatch(16, 508, 16, 494),
        FragmentMatch(16, 494, 48, 494),
        FragmentMatch(47, 495, 47.9286, 494),
        FragmentMatch(47.9286, 494, 58, 483.153846153846),
        FragmentMatch(58, 483.153846153846, 60, 481),
        FragmentMatch(58, 483.153846153846, 58, 481),
        FragmentMatch(58, 481, 58, 477),
        FragmentMatch(58, 469, 58, 465),
        FragmentMatch(58, 465, 58, 460),
        FragmentMatch(58, 465, 57, 464),
        FragmentMatch(58, 460, 57, 460),
        FragmentMatch(57, 460, 49, 460),
        FragmentMatch(49, 460, 47, 460),
        FragmentMatch(47, 460, 47, 459),
        FragmentMatch(47, 460, 47, 483),
        FragmentMatch(47, 483, 12, 483),
        FragmentMatch(12, 483, 9, 483),
        FragmentMatch(12, 483, 11, 484),
        FragmentMatch(11, 484, 8, 487),
        FragmentMatch(9, 483, 5, 483),
        FragmentMatch(5, 483, 5, 485),
        FragmentMatch(5, 485, 5, 509),
        FragmentMatch(5, 509, 5, 510),
    }));
}

#include <gmock/gmock.h>

#include <intersectionsplitter/IntersectionSplitter.h>

#include "TestUtils.hpp"

using namespace intersectionsplitter;

TEST(WallFragmentIntersectionSplitterTest, singleFragment) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,0, 1,0)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(FragmentMatch(0,0, 1,0)));
}

TEST(WallFragmentIntersectionSplitterTest, twoFragmentsNoIntersection) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,0, 1,0),
        Fragment(2,0, 2,2)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(FragmentMatch(0,0, 1,0), FragmentMatch(2,0, 2,2)));
}

TEST(WallFragmentIntersectionSplitterTest, twoFragmentsWithIntersection) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,0, 2,4),
        Fragment(2,0, 0,4)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(0,0, 1,2), FragmentMatch(1,2, 2,4),
                    FragmentMatch(2,0, 1,2), FragmentMatch(1,2, 0,4)));
}

TEST(WallFragmentIntersectionSplitterTest, twoFragmentsNoIntersectionButSameEndPoint) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,0, 1,2),
        Fragment(1,2, 2,4)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(FragmentMatch(0,0, 1,2), FragmentMatch(1,2, 2,4)));
}

TEST(WallFragmentIntersectionSplitterTest, threeFragmentsIntersectingAtSamePoint) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,0, 2,4),
        Fragment(2,0, 0,4),
        Fragment(0,2, 2,2),
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(0,0, 1,2), FragmentMatch(1,2, 2,4),
                    FragmentMatch(2,0, 1,2), FragmentMatch(1,2, 0,4),
                    FragmentMatch(0,2, 1,2), FragmentMatch(1,2, 2,2)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectionPointOnLine) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,0, 2,2),
        Fragment(2,0, 2,4)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(0,0, 2,2),
                    FragmentMatch(2,0, 2,2),
                    FragmentMatch(2,2, 2,4)));
}

TEST(WallFragmentIntersectionSplitterTest, lineIntersectedTwoTimes) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,0, 10,5),
        Fragment(2,0, 6,4),
        Fragment(9,3, 7,5)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(0,0, 4,2),
                    FragmentMatch(4,2, 8,4),
                    FragmentMatch(8,4, 10,5),

                    FragmentMatch(2,0, 4,2),
                    FragmentMatch(4,2, 6,4),

                    FragmentMatch(9,3, 8,4),
                    FragmentMatch(8,4, 7,5)));
}

TEST(WallFragmentIntersectionSplitterTest, multipleIntersections) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,3, 4,3),
        Fragment(4,2, 8,2),
        Fragment(5,2, 3,4),
        Fragment(8,0, 8,2)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(0,3, 4,3),

                    FragmentMatch(4,3, 3,4),
                    FragmentMatch(4,3, 5,2),

                    FragmentMatch(4,2, 5,2),
                    FragmentMatch(5,2, 8,2),

                    FragmentMatch(8,2, 8,0)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectWithExtendForward) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,0, 2,0),
        Fragment(4,0, 4,4)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 2.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(FragmentMatch(0,0, 2,0), FragmentMatch(2,0, 4,0), FragmentMatch(4,0, 4,4)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectWithExtendBackward) {

    std::vector<LineSegmentPtr> input = {
        Fragment(2,0, 4,0),
        Fragment(0,0, 0,4)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 2.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(FragmentMatch(0,0, 0,4), FragmentMatch(0,0, 2,0), FragmentMatch(2,0, 4,0)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectWithExtendTwice) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,0, 2,0),
        Fragment(4,0, 4,4),
        Fragment(5,0, 5,4)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(FragmentMatch(0,0, 2,0), FragmentMatch(2,0, 4,0), FragmentMatch(4,0, 5,0), FragmentMatch(4,0, 4,4), FragmentMatch(5,0, 5,4)));
}

TEST(WallFragmentIntersectionSplitterTest, floatCoordinates) {

    std::vector<LineSegmentPtr> input = {
        Fragment(239, 335, 245, 328),
        Fragment(243, 332, 235, 332)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(239, 335,           241.57143f, 332),
                    FragmentMatch(241.57143f, 332,    245, 328),

                    FragmentMatch(243, 332,           241.57143f, 332),
                    FragmentMatch(241.57143f, 332,    235, 332)));
}

TEST(WallFragmentIntersectionSplitterTest, noIntersectRegression) {

    std::vector<LineSegmentPtr> input = {
        Fragment(233, 398, 233, 384),
        Fragment(243, 401, 237, 395)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(233, 398, 233, 384),
                    FragmentMatch(243, 401, 237, 395)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectFirstNormalAndThemWithExtendForward) {

    std::vector<LineSegmentPtr> input = {
        Fragment(5, 20, 5, 5),
        Fragment(0, 15, 10, 15),
        Fragment(0, 4, 10, 4)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(5, 20, 5, 15),
                    FragmentMatch(5, 15,  5, 5),
                    FragmentMatch(5, 5,  5, 4),

                    FragmentMatch(0, 15,  5, 15),
                    FragmentMatch(5, 15,  10, 15),

                    FragmentMatch(0, 4,  5, 4),
                    FragmentMatch(5, 4,  10, 4)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectInPast) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0, 5, 0, 0),
        Fragment(4, 4, 10, 4),
        Fragment(0, -1, 10, -1)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(0, 5, 0, 4),
                    FragmentMatch(0, 4, 0, 0),
                    FragmentMatch(0, 0, 0, -1),

                    FragmentMatch(0, 4,  4, 4),
                    FragmentMatch(4, 4,  10, 4),

                    FragmentMatch(0, -1,  10, -1)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectTwoLeft) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0, 5, 0, 0),
        Fragment(1, 2, 2, 1),
        Fragment(2, 0, 4, 0)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(0, 5, 0, 3),
                    FragmentMatch(0, 3, 0, 0),

                    FragmentMatch(0, 3, 1, 2),
                    FragmentMatch(1, 2, 2, 1),
                    FragmentMatch(2, 1, 3, 0),

                    FragmentMatch(0, 0,  2, 0),
                    FragmentMatch(2, 0,  3, 0),
                    FragmentMatch(3, 0,  4, 0)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectTwoRight) {

    std::vector<LineSegmentPtr> input = {
        Fragment(5, 4, 5, 0),
        Fragment(2, 5, 4, 3),
        Fragment(1, 1, 3, 1)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(5, 4, 5, 2),
                    FragmentMatch(5, 2, 5, 1),
                    FragmentMatch(5, 1, 5, 0),

                    FragmentMatch(2, 5, 4, 3),
                    FragmentMatch(4, 3, 5, 2),
                    FragmentMatch(5, 2, 6, 1),

                    FragmentMatch(1, 1,  3, 1),
                    FragmentMatch(3, 1,  5, 1),
                    FragmentMatch(5, 1,  6, 1)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectTwoRegression) {

    std::vector<LineSegmentPtr> input = {
        Fragment(5, 510, 5, 485),
        Fragment(9, 483, 47, 483)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(5, 510, 5, 485),
                    FragmentMatch(5, 485, 5, 483),
                    FragmentMatch(5, 483, 9, 483),
                    FragmentMatch(9, 483, 47, 483)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectAtEndpointRegression) {

    std::vector<LineSegmentPtr> input = {
        Fragment(1253, 2406, 1231.448, 2371.106),
        Fragment(1251.831,  2406.73, 1262.438, 2400.101)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(1253, 2406, 1231.448, 2371.106),
                    FragmentMatch(1251.831,  2406.73, 1253, 2406),
                    FragmentMatch(1253, 2406, 1262.438, 2400.101)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectAtEndpointAndLeft) {

    std::vector<LineSegmentPtr> input = {
        Fragment(0,10, 0,0),
        Fragment(2,10, 2,0),
        Fragment(4,10, 4,0),
        Fragment(4,5, 10,5),
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(0,10, 0,5), FragmentMatch(0,5, 0, 0),
                    FragmentMatch(2,10, 2,5), FragmentMatch(2,5, 2, 0),
                    FragmentMatch(4,10, 4,5), FragmentMatch(4,5, 4, 0),
                    FragmentMatch(0, 5, 2,5), FragmentMatch(2,5, 4, 5), FragmentMatch(4,5, 10,5)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectionNearStartOfBothFragments) {

    std::vector<LineSegmentPtr> input = {
        Fragment(641.6922, 1519.385, 649, 1515),
        Fragment(641.8182, 1519.364, 640, 1518)
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 0.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(
                    FragmentMatch(641.6922, 1519.385, 649, 1515),
                    FragmentMatch(641.8182, 1519.364, 640, 1518)));
}


#include <gmock/gmock.h>

#include <intersectionsplitter/intersectionsplitter.h>

#include "testutils.hpp"

using namespace intersectionsplitter;

TEST(WallFragmentIntersectionSplitterTest, singleFragment) {
    std::vector<LineSegmentPtr> input = {Segment(0, 0, 1, 0)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 1, 0)));
}

TEST(WallFragmentIntersectionSplitterTest, twoFragmentsNoIntersection) {
    std::vector<LineSegmentPtr> input = {Segment(0, 0, 1, 0), Segment(2, 0, 2, 2)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 1, 0), SegmentMatch(2, 0, 2, 2)));
}

TEST(WallFragmentIntersectionSplitterTest, twoFragmentsWithIntersection) {
    std::vector<LineSegmentPtr> input = {Segment(0, 0, 2, 4), Segment(2, 0, 0, 4)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 1, 2), SegmentMatch(1, 2, 2, 4), SegmentMatch(2, 0, 1, 2), SegmentMatch(1, 2, 0, 4)));
}

TEST(WallFragmentIntersectionSplitterTest, twoFragmentsNoIntersectionButSameEndPoint) {
    std::vector<LineSegmentPtr> input = {Segment(0, 0, 1, 2), Segment(1, 2, 2, 4)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 1, 2), SegmentMatch(1, 2, 2, 4)));
}

TEST(WallFragmentIntersectionSplitterTest, threeFragmentsIntersectingAtSamePoint) {
    std::vector<LineSegmentPtr> input = {
        Segment(0, 0, 2, 4), Segment(2, 0, 0, 4), Segment(0, 2, 2, 2),
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 1, 2), SegmentMatch(1, 2, 2, 4), SegmentMatch(2, 0, 1, 2), SegmentMatch(1, 2, 0, 4),
                                                      SegmentMatch(0, 2, 1, 2), SegmentMatch(1, 2, 2, 2)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectionPointOnLine) {
    std::vector<LineSegmentPtr> input = {Segment(0, 0, 2, 2), Segment(2, 0, 2, 4)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 2, 2), SegmentMatch(2, 0, 2, 2), SegmentMatch(2, 2, 2, 4)));
}

TEST(WallFragmentIntersectionSplitterTest, lineIntersectedTwoTimes) {
    std::vector<LineSegmentPtr> input = {Segment(0, 0, 10, 5), Segment(2, 0, 6, 4), Segment(9, 3, 7, 5)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 4, 2), SegmentMatch(4, 2, 8, 4), SegmentMatch(8, 4, 10, 5),

                                                      SegmentMatch(2, 0, 4, 2), SegmentMatch(4, 2, 6, 4),

                                                      SegmentMatch(9, 3, 8, 4), SegmentMatch(8, 4, 7, 5)));
}

TEST(WallFragmentIntersectionSplitterTest, multipleIntersections) {
    std::vector<LineSegmentPtr> input = {Segment(0, 3, 4, 3), Segment(4, 2, 8, 2), Segment(5, 2, 3, 4), Segment(8, 0, 8, 2)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 3, 4, 3),

                                                      SegmentMatch(4, 3, 3, 4), SegmentMatch(4, 3, 5, 2),

                                                      SegmentMatch(4, 2, 5, 2), SegmentMatch(5, 2, 8, 2),

                                                      SegmentMatch(8, 2, 8, 0)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectWithExtendForward) {
    std::vector<LineSegmentPtr> input = {Segment(0, 0, 2, 0), Segment(4, 0, 4, 4)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 2.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 2, 0), SegmentMatch(2, 0, 4, 0), SegmentMatch(4, 0, 4, 4)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectWithExtendBackward) {
    std::vector<LineSegmentPtr> input = {Segment(2, 0, 4, 0), Segment(0, 0, 0, 4)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 2.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 0, 4), SegmentMatch(0, 0, 2, 0), SegmentMatch(2, 0, 4, 0)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectWithExtendTwice) {
    std::vector<LineSegmentPtr> input = {Segment(0, 0, 2, 0), Segment(4, 0, 4, 4), Segment(5, 0, 5, 4)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 0, 2, 0), SegmentMatch(2, 0, 4, 0), SegmentMatch(4, 0, 5, 0), SegmentMatch(4, 0, 4, 4),
                                                      SegmentMatch(5, 0, 5, 4)));
}

TEST(WallFragmentIntersectionSplitterTest, floatCoordinates) {
    std::vector<LineSegmentPtr> input = {Segment(239, 335, 245, 328), Segment(243, 332, 235, 332)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(239, 335, 241.57143f, 332), SegmentMatch(241.57143f, 332, 245, 328),

                                                      SegmentMatch(243, 332, 241.57143f, 332), SegmentMatch(241.57143f, 332, 235, 332)));
}

TEST(WallFragmentIntersectionSplitterTest, noIntersectRegression) {
    std::vector<LineSegmentPtr> input = {Segment(233, 398, 233, 384), Segment(243, 401, 237, 395)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(233, 398, 233, 384), SegmentMatch(243, 401, 237, 395)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectFirstNormalAndThemWithExtendForward) {
    std::vector<LineSegmentPtr> input = {Segment(5, 20, 5, 5), Segment(0, 15, 10, 15), Segment(0, 4, 10, 4)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(5, 20, 5, 15), SegmentMatch(5, 15, 5, 5), SegmentMatch(5, 5, 5, 4),

                                                      SegmentMatch(0, 15, 5, 15), SegmentMatch(5, 15, 10, 15),

                                                      SegmentMatch(0, 4, 5, 4), SegmentMatch(5, 4, 10, 4)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectInPast) {
    std::vector<LineSegmentPtr> input = {Segment(0, 5, 0, 0), Segment(4, 4, 10, 4), Segment(0, -1, 10, -1)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 5, 0, 4), SegmentMatch(0, 4, 0, 0), SegmentMatch(0, 0, 0, -1),

                                                      SegmentMatch(0, 4, 4, 4), SegmentMatch(4, 4, 10, 4),

                                                      SegmentMatch(0, -1, 10, -1)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectTwoLeft) {
    std::vector<LineSegmentPtr> input = {Segment(0, 5, 0, 0), Segment(1, 2, 2, 1), Segment(2, 0, 4, 0)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 5, 0, 3), SegmentMatch(0, 3, 0, 0),

                                                      SegmentMatch(0, 3, 1, 2), SegmentMatch(1, 2, 2, 1), SegmentMatch(2, 1, 3, 0),

                                                      SegmentMatch(0, 0, 2, 0), SegmentMatch(2, 0, 3, 0), SegmentMatch(3, 0, 4, 0)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectTwoRight) {
    std::vector<LineSegmentPtr> input = {Segment(5, 4, 5, 0), Segment(2, 5, 4, 3), Segment(1, 1, 3, 1)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(5, 4, 5, 2), SegmentMatch(5, 2, 5, 1), SegmentMatch(5, 1, 5, 0),

                                                      SegmentMatch(2, 5, 4, 3), SegmentMatch(4, 3, 5, 2), SegmentMatch(5, 2, 6, 1),

                                                      SegmentMatch(1, 1, 3, 1), SegmentMatch(3, 1, 5, 1), SegmentMatch(5, 1, 6, 1)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectTwoRegression) {
    std::vector<LineSegmentPtr> input = {Segment(5, 510, 5, 485), Segment(9, 483, 47, 483)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(5, 510, 5, 485), SegmentMatch(5, 485, 5, 483), SegmentMatch(5, 483, 9, 483),
                                                      SegmentMatch(9, 483, 47, 483)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectAtEndpointRegression) {
    std::vector<LineSegmentPtr> input = {Segment(1253, 2406, 1231.448, 2371.106), Segment(1251.831, 2406.73, 1262.438, 2400.101)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(1253, 2406, 1231.448, 2371.106), SegmentMatch(1251.831, 2406.73, 1253, 2406),
                                                      SegmentMatch(1253, 2406, 1262.438, 2400.101)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectAtEndpointAndLeft) {
    std::vector<LineSegmentPtr> input = {
        Segment(0, 10, 0, 0), Segment(2, 10, 2, 0), Segment(4, 10, 4, 0), Segment(4, 5, 10, 5),
    };

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 4.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(0, 10, 0, 5), SegmentMatch(0, 5, 0, 0), SegmentMatch(2, 10, 2, 5), SegmentMatch(2, 5, 2, 0),
                                                      SegmentMatch(4, 10, 4, 5), SegmentMatch(4, 5, 4, 0), SegmentMatch(0, 5, 2, 5), SegmentMatch(2, 5, 4, 5),
                                                      SegmentMatch(4, 5, 10, 5)));
}

TEST(WallFragmentIntersectionSplitterTest, intersectionNearStartOfBothFragments) {
    std::vector<LineSegmentPtr> input = {Segment(641.6922, 1519.385, 649, 1515), Segment(641.8182, 1519.364, 640, 1518)};

    std::vector<LineSegmentPtr> result = splitLineSegmentsAtIntersections(input, 0.f);

    EXPECT_THAT(result, testing::UnorderedElementsAre(SegmentMatch(641.6922, 1519.385, 649, 1515), SegmentMatch(641.8182, 1519.364, 640, 1518)));
}

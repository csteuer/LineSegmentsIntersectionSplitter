#include <gmock/gmock.h>

#include <intersectionsplitter/StatusStructure.h>
#include <intersectionsplitter/IntersectionSplitterUtils.h>

#include "TestUtils.hpp"

#include <vector>

using namespace intersectionsplitter;

#define Directed_Fragment(x0,y0, x1,y1) directSegment(Fragment(x0,y0, x1,y1))

void expect_inorder(const std::vector<LineSegmentPtr>& order, StatusStructure& ss) {

    for (size_t i = 0; i < order.size(); i++) {

        LineSegmentPtr current = order.at(i);

        if (i == 0) {
            EXPECT_THAT(ss.leftNeighbourOf(current), ::testing::IsNull());
        } else {
            EXPECT_THAT(ss.leftNeighbourOf(current), ::testing::Eq(order.at(i - 1)));
        }

        if (i == order.size() - 1) {
            EXPECT_THAT(ss.rightNeighbourOf(current), ::testing::IsNull());
        } else {
            EXPECT_THAT(ss.rightNeighbourOf(current), ::testing::Eq(order.at(i + 1)));
        }
    }

}


TEST(WallFragmentIntersectionSplitterStatusStructureTest, verticalFragments) {


    LineSegmentPtr a = Directed_Fragment(0,1, 0,-1);
    LineSegmentPtr b = Directed_Fragment(1,1, 1,-1);
    LineSegmentPtr c = Directed_Fragment(2,1, 2,-1);



    StatusStructure ss;

    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));

    expect_inorder({a, b, c}, ss);
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, nonVerticalFragments) {

    LineSegmentPtr a = Directed_Fragment(0,2, 4,-2);
    LineSegmentPtr b = Directed_Fragment(6,2, 4,-1);
    LineSegmentPtr c = Directed_Fragment(3,3, 18,-1);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));


    expect_inorder({a, b, c}, ss);
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, horizontalFragment) {

    LineSegmentPtr a = Directed_Fragment(3,2, 0,-2);
    LineSegmentPtr b = Directed_Fragment(2,0, 6,0);
    LineSegmentPtr c = Directed_Fragment(5,2, 10,-2);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));


    expect_inorder({a, b, c}, ss);
}



TEST(WallFragmentIntersectionSplitterStatusStructureTest, fragmentsWithSameStartOnScanLine) {

    LineSegmentPtr a = Directed_Fragment(4,0, 3,-1);
    LineSegmentPtr b = Directed_Fragment(4,0, 2,-4);
    LineSegmentPtr c = Directed_Fragment(4,0, 5,-1);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));


    expect_inorder({a, b, c}, ss);
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, fragmentsWithSameStartOnScanLine_insertTogether) {

    std::vector<LineSegmentPtr> input = {
        Directed_Fragment(4,0, 3,-1),
        Directed_Fragment(4,0, 2,-4),
        Directed_Fragment(4,0, 5,-1)
    };

    StatusStructure ss;
    ss.insert(input, Point(0, 0));


    expect_inorder(input, ss);
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, fragmentsWithSameStartOnScanLineWithHorizontal) {

    LineSegmentPtr a = Directed_Fragment(3,0, 0,-3);
    LineSegmentPtr b = Directed_Fragment(3,0, 6,-1);
    LineSegmentPtr c = Directed_Fragment(3,0, 5, 0);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));


    expect_inorder({a, b, c}, ss);
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, fragmentsWithSameStartOnScanLineWithHorizontal_insertTogether) {

    std::vector<LineSegmentPtr> input = {
        Directed_Fragment(3,0, 0,-3),
        Directed_Fragment(3,0, 6,-1),
        Directed_Fragment(3,0, 5, 0)
    };

    StatusStructure ss;
    ss.insert(input, Point(0, 0));


    expect_inorder(input, ss);
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, testInsertFragmentsWithSameStartOnScanLineIntoNonEmptyStatus) {

    LineSegmentPtr a = Directed_Fragment(0,2, 1,-1);
    LineSegmentPtr b = Directed_Fragment(5,2, 3,-2);

    std::vector<LineSegmentPtr> input = {
        Directed_Fragment(3,0, 0,-3),
        Directed_Fragment(3,0, 6,-1),
        Directed_Fragment(3,0, 5, 0)
    };

    StatusStructure ss;

    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(input, Point(0, 0));

    expect_inorder({a, input[0], input[1], input[2], b}, ss);
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, testStartpointsAtAlternatingHeight) {

    LineSegmentPtr a = Directed_Fragment(9,4, 7,-2);
    LineSegmentPtr b = Directed_Fragment(7,3, 5,-3);
    LineSegmentPtr c = Directed_Fragment(10,3, 9,-3);
    LineSegmentPtr d = Directed_Fragment(5,2, 3,-4);
    LineSegmentPtr e = Directed_Fragment(7,2, 6,-4);
    LineSegmentPtr f = Directed_Fragment(9,2, 8,-4);
    LineSegmentPtr g = Directed_Fragment(11,2, 11,-4);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));
    ss.insert(d, Point(0, 0));
    ss.insert(e, Point(0, 0));
    ss.insert(f, Point(0, 0));
    ss.insert(g, Point(0, 0));

    expect_inorder({d, b, e, a, f, c, g}, ss);

}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, testDelete) {

    LineSegmentPtr a = Directed_Fragment(4.5,5, 4.5,-1);
    LineSegmentPtr b = Directed_Fragment(4,4, 4,-2);
    LineSegmentPtr c = Directed_Fragment(2,3, 2,-3);
    LineSegmentPtr d = Directed_Fragment(6,3, 6,-3);
    LineSegmentPtr e = Directed_Fragment(1,2, 1,-4);
    LineSegmentPtr f = Directed_Fragment(3,2, 3,-3);
    LineSegmentPtr g = Directed_Fragment(5,2, 5,-4);
    LineSegmentPtr h = Directed_Fragment(7,2, 7,-4);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));
    ss.insert(d, Point(0, 0));
    ss.insert(e, Point(0, 0));
    ss.insert(f, Point(0, 0));
    ss.insert(g, Point(0, 0));
    ss.insert(h, Point(0, 0));

    expect_inorder({e, c, f, b, a, g, d, h}, ss);

    ss.remove(a);

    expect_inorder({e, c, f, b, g, d, h}, ss);

    ss.remove(b);

    expect_inorder({e, c, f, g, d, h}, ss);

    ss.remove(g);

    expect_inorder({e, c, f, d, h}, ss);

    ss.remove(d);

    expect_inorder({e, c, f, h}, ss);

    ss.remove(h);

    expect_inorder({e, c, f}, ss);

    ss.remove(c);

    expect_inorder({e, f}, ss);

    ss.remove(e);

    expect_inorder({f}, ss);

    ss.remove(f);

}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, testFragmentLeftOfPoint) {

    LineSegmentPtr a = Directed_Fragment(7,4, 5,-3);
    LineSegmentPtr b = Directed_Fragment(5,3, 3,-2);
    LineSegmentPtr c = Directed_Fragment(8,3, 12, -3);
    LineSegmentPtr d = Directed_Fragment(8,2, 9,-2);
    LineSegmentPtr e = Directed_Fragment(7,1, 6, -3);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));
    ss.insert(d, Point(0, 0));
    ss.insert(e, Point(0, 0));


    expect_inorder({b, a, e, d, c}, ss);

    EXPECT_THAT(ss.fragmentLeftOf(Point(11,0)), ::testing::Eq(c));
    EXPECT_THAT(ss.fragmentLeftOf(Point(9,0)), ::testing::Eq(d));
    EXPECT_THAT(ss.fragmentLeftOf(Point(7,0)), ::testing::Eq(e));
    EXPECT_THAT(ss.fragmentLeftOf(Point(6,0)), ::testing::Eq(a));
    EXPECT_THAT(ss.fragmentLeftOf(Point(5,0)), ::testing::Eq(b));

    EXPECT_THAT(ss.fragmentLeftOf(Point(10,0)), ::testing::Eq(d));
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, testFragmentLeftOfPointWithHorizontalAndSameStartpoint) {

    LineSegmentPtr a = Directed_Fragment(2,1, 2,-2);
    LineSegmentPtr b = Directed_Fragment(4,0, 3,-2);
    LineSegmentPtr c = Directed_Fragment(4,0, 6, 0);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));


    expect_inorder({a, b, c}, ss);

    EXPECT_THAT(ss.fragmentLeftOf(Point(4,0)), ::testing::Eq(a));
    EXPECT_THAT(ss.fragmentLeftOf(Point(7,0)), ::testing::Eq(c));
    EXPECT_THAT(ss.fragmentLeftOf(Point(4.1f,0.f)), ::testing::Eq(c));
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, testLeftOfPointEmpty) {
    StatusStructure ss;
    EXPECT_THAT(ss.fragmentLeftOf(Point(4,0)), ::testing::IsNull());
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, testFragmentRightOfPoint) {

    LineSegmentPtr a = Directed_Fragment(7,4, 5,-3);
    LineSegmentPtr b = Directed_Fragment(5,3, 3,-2);
    LineSegmentPtr c = Directed_Fragment(8,3, 12, -3);
    LineSegmentPtr d = Directed_Fragment(8,2, 9,-2);
    LineSegmentPtr e = Directed_Fragment(7,1, 6, -3);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));
    ss.insert(d, Point(0, 0));
    ss.insert(e, Point(0, 0));


    expect_inorder({b, a, e, d, c}, ss);

    EXPECT_THAT(ss.fragmentRightOf(Point(9,0)), ::testing::Eq(c));
    EXPECT_THAT(ss.fragmentRightOf(Point(8,0)), ::testing::Eq(d));
    EXPECT_THAT(ss.fragmentRightOf(Point(6,0)), ::testing::Eq(e));
    EXPECT_THAT(ss.fragmentRightOf(Point(5,0)), ::testing::Eq(a));
    EXPECT_THAT(ss.fragmentRightOf(Point(3,0)), ::testing::Eq(b));

    EXPECT_THAT(ss.fragmentRightOf(Point(10,0)), ::testing::IsNull());
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, testFragmentRightOfPointWithHorizontalAndSameStartpoint) {

    LineSegmentPtr a = Directed_Fragment(2,1, 2,-2);
    LineSegmentPtr b = Directed_Fragment(4,0, 3,-2);
    LineSegmentPtr c = Directed_Fragment(4,0, 6, 0);

    StatusStructure ss;
    ss.insert(a, Point(0, 0));
    ss.insert(b, Point(0, 0));
    ss.insert(c, Point(0, 0));


    expect_inorder({a, b, c}, ss);

    EXPECT_THAT(ss.fragmentRightOf(Point(4,0)), ::testing::IsNull());
    EXPECT_THAT(ss.fragmentRightOf(Point(7,0)), ::testing::IsNull());
    EXPECT_THAT(ss.fragmentRightOf(Point(3,0)), b);
    EXPECT_THAT(ss.fragmentRightOf(Point(2,0)), ::testing::Eq(b));
    EXPECT_THAT(ss.fragmentRightOf(Point(1,0)), ::testing::Eq(a));
}

TEST(WallFragmentIntersectionSplitterStatusStructureTest, removeNonExistendElements) {

    StatusStructure ss;
    ss.insert(Directed_Fragment(0,1, 0,-1), Point(0, 0));

    // Remove fragment above the scan line
    ss.remove(Directed_Fragment(0,3, 0,2));

    // Remove fragment below the scan line
    ss.remove(Directed_Fragment(0,-2, 0,-4));

}





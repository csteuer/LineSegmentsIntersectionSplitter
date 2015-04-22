#pragma once

#include <gmock/gmock.h>
#include <intersectionsplitter/LineSegment.h>
#include <intersectionsplitter/PrintUtils.h>


#define StackSegment(x1, y1, x2, y2) intersectionsplitter::LineSegment::init((float) x1, (float) y1, (float) x2, (float) y2)
#define Segment(x1, y1, x2, y2) intersectionsplitter::LineSegment::create((float) x1, (float) y1, (float) x2, (float) y2)
#define SegmentMatch(x1, y1, x2, y2) ::testing::Pointee(StackSegment(x1, y1, x2, y2))



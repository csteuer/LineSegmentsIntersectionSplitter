#pragma once

#include <gmock/gmock.h>
#include <intersectionsplitter/LineSegment.h>
#include <intersectionsplitter/PrintUtils.h>

#define StackSegment(x1, y1, x2, y2) \
    intersectionsplitter::LineSegment::init(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2))
#define Segment(x1, y1, x2, y2) \
    intersectionsplitter::LineSegment::create(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2))
#define SegmentMatch(x1, y1, x2, y2) ::testing::Pointee(StackSegment(x1, y1, x2, y2))

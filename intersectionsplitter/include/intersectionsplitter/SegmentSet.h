#pragma once

#include <unordered_set>
#include <vector>

#include "LineSegment.h"

namespace intersectionsplitter {

class SegmentSet {
    typedef std::unordered_set<LineSegmentPtr> Container;

public:
    void insert(LineSegmentPtr segment);

    void insert(const std::vector<LineSegmentPtr>& segments);

    std::vector<LineSegmentPtr> segments();

    Container::const_iterator begin();

    Container::const_iterator end();

private:
    Container m_fragments;
};
}

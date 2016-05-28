#include "segmentset.h"

#include <assert.h>

namespace intersectionsplitter {

void SegmentSet::insert(LineSegmentPtr segment) {
    assert(segment->valid());
    m_fragments.insert(segment);
}

void SegmentSet::insert(const std::vector<LineSegmentPtr>& segments) {
#ifndef NDEBUG
    for (size_t i = 0; i < segments.size(); i++) {
        assert(segments.at(i)->valid());
    }
#endif

    m_fragments.insert(segments.begin(), segments.end());
}

std::vector<LineSegmentPtr> SegmentSet::segments() const {
    return std::vector<LineSegmentPtr>(m_fragments.begin(), m_fragments.end());
}

SegmentSet::Container::const_iterator SegmentSet::begin() {
    return m_fragments.begin();
}

SegmentSet::Container::const_iterator SegmentSet::end() {
    return m_fragments.end();
}
}

#pragma once

#include <unordered_map>
#include <vector>
#include <cstddef>

#include "linesegment.h"

namespace intersectionsplitter {

class Point;
class StatusNode;
class StatusStructure {
public:
    void insert(LineSegmentPtr fragment, const Point& scanLinePosition);
    void insert(const std::vector<LineSegmentPtr>& fragmentsWithSameStart, const Point& scanLinePosition);

    void remove(const LineSegmentPtr& fragment);

    LineSegmentPtr leftNeighbourOf(const LineSegmentPtr& fragment) const;

    LineSegmentPtr rightNeighbourOf(const LineSegmentPtr& fragment) const;

    LineSegmentPtr fragmentLeftOf(const Point& point) const;

    LineSegmentPtr fragmentRightOf(const Point& point) const;

    void swapNeighbours(LineSegmentPtr a, LineSegmentPtr b);

    void clear();

private:
    std::vector<LineSegmentPtr> m_tree;
    LineSegmentPtr getAt(size_t index) const;
    LineSegmentPtr leftChild(size_t index) const;
    LineSegmentPtr rightChild(size_t index) const;
    size_t getLeftNeighbourIndex(size_t index) const;
    size_t getRightNeighbourIndex(size_t index) const;
    void insertAt(size_t index, LineSegmentPtr frag);
    void move(size_t from, size_t to);
    size_t indexOf(const LineSegmentPtr& frag, bool& outValid) const;
    size_t getClosestTo(const Point& point, float& outDist) const;

    std::unordered_map<LineSegmentPtr, size_t> m_index_map;
};
}

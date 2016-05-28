#include "statusstructure.h"
#include "intersectionsplitterutils.h"

#include <queue>
#include <utility>
#include <stdexcept>
#include <assert.h>
#include <cmath>

namespace intersectionsplitter {

static size_t getLeftChildIndex(size_t index) {
    return (index << 1) + 1;  // index * 2 + 1
}

static size_t getRightChildIndex(size_t index) {
    return (index << 1) + 2;
}

static bool isLeftChild(size_t index) {
    return index % 2 != 0;
}

static size_t getParentIndex(size_t index) {
    if (isLeftChild(index)) {
        return (index - 1) >> 1;
    } else {
        return (index - 2) >> 1;
    }
}

LineSegmentPtr StatusStructure::getAt(size_t index) const {
    if (index < m_tree.size()) {
        return m_tree[index];
    }

    return nullptr;
}

LineSegmentPtr StatusStructure::leftChild(size_t index) const {
    return getAt(getLeftChildIndex(index));
}

LineSegmentPtr StatusStructure::rightChild(size_t index) const {
    return getAt(getRightChildIndex(index));
}

LineSegmentPtr StatusStructure::parent(size_t index) const {
    if (index == 0) {
        return nullptr;
    }

    return getAt(getParentIndex(index));
}

void StatusStructure::insertAt(size_t index, LineSegmentPtr frag) {
    if (index >= m_tree.size()) {
        m_tree.resize(index + 1, nullptr);
    }

    m_tree[index] = frag;
    m_index_map[frag] = index;
}

void StatusStructure::move(size_t from, size_t to) {
    if (to >= m_tree.size()) {
        return;
    }

    // Movement must be recursive and breath-first instead of depth-first (otherwise we would overwrite elements before they are moved)
    std::queue<std::pair<size_t, size_t>> moveQueue;
    moveQueue.push(std::make_pair(from, to));

    while (!moveQueue.empty()) {
        from = moveQueue.front().first;
        to = moveQueue.front().second;
        moveQueue.pop();

        auto toMoveFrag = getAt(from);
        insertAt(to, toMoveFrag);

        if (toMoveFrag != nullptr) {
            // move the children of the moved element
            insertAt(from, nullptr);                                                           // clear the element
            moveQueue.push(std::make_pair(getLeftChildIndex(from), getLeftChildIndex(to)));    // move left children
            moveQueue.push(std::make_pair(getRightChildIndex(from), getRightChildIndex(to)));  // move right children
        }
    }
}

size_t StatusStructure::indexOf(const LineSegmentPtr frag, bool& outValid) const {
    auto it = m_index_map.find(frag);
    outValid = it != m_index_map.end();
    if (!outValid) {
        return 0;
    }
    assert(m_tree[it->second] == frag);
    return it->second;
}

void StatusStructure::insert(LineSegmentPtr fragment, const Point& scanLinePosition) {
    size_t currentIndex = 0;
    auto currentFrag = getAt(currentIndex);

    while (currentFrag != nullptr) {
        if (currentFrag == fragment) {
            return;
        }

        if (isALeftOfBOnScanLine(*fragment, *currentFrag, scanLinePosition)) {
            currentIndex = getLeftChildIndex(currentIndex);
        } else {
            currentIndex = getRightChildIndex(currentIndex);
        }

        currentFrag = getAt(currentIndex);
    }

    insertAt(currentIndex, fragment);
}

void StatusStructure::insert(const std::vector<LineSegmentPtr>& fragmentsWithSameStart, const Point& scanLinePosition) {
    for (const auto& frag : fragmentsWithSameStart) {
        insert(frag, scanLinePosition);
    }
}

void StatusStructure::remove(const LineSegmentPtr fragment) {
    bool valid;
    size_t index = indexOf(fragment, valid);

    if (!valid) {
        return;
    }

    m_index_map.erase(fragment);

    if (leftChild(index) == nullptr) {
        move(getRightChildIndex(index), index);
        return;
    } else if (rightChild(index) == nullptr) {
        move(getLeftChildIndex(index), index);
        return;
    }

    // get direct left neighbour of index (most right child in left subtree)
    size_t directLeftNeighbourIndex = getLeftChildIndex(index);
    while (rightChild(directLeftNeighbourIndex) != nullptr) {
        directLeftNeighbourIndex = getRightChildIndex(directLeftNeighbourIndex);
    }

    insertAt(index, getAt(directLeftNeighbourIndex));
    move(getLeftChildIndex(directLeftNeighbourIndex), directLeftNeighbourIndex);
}

size_t StatusStructure::getLeftNeighbourIndex(size_t index) const {
    size_t currentIndex = index;

    if (leftChild(currentIndex) != nullptr) {
        // if there is a left sub tree find the most right leaf in that tree
        currentIndex = getLeftChildIndex(currentIndex);

        while (rightChild(currentIndex) != nullptr) {
            currentIndex = getRightChildIndex(currentIndex);
        }

        return currentIndex;
    }

    // the root element can't be the right child of another element
    if (currentIndex == 0) {
        return index;
    }

    if (!isLeftChild(currentIndex)) {
        // current element is the right child of its parent... hence the parent is the left neighbour
        return getParentIndex(currentIndex);
    }

    // current element is the left child of its parent... traverse upwards to the first node that is not a left child or the root node
    while (currentIndex > 0 && isLeftChild(currentIndex)) {
        currentIndex = getParentIndex(currentIndex);
    }

    if (currentIndex == 0) {
        return index;
    }

    // the fragment at currentIndex is right of its parent and the fragment that
    // we started with is the most left child of this fragment... hence the parent is
    // the direct left neighbour
    currentIndex = getParentIndex(currentIndex);

    return currentIndex;
}

LineSegmentPtr StatusStructure::leftNeighbourOf(const LineSegmentPtr fragment) {
    bool valid;
    size_t index = indexOf(fragment, valid);

    if (!valid) {
        std::runtime_error("StatusStructure::leftNeighbourOf: Invalid argument! No such fragment exists");
    }

    size_t leftNeighbourIndex = getLeftNeighbourIndex(index);

    if (leftNeighbourIndex == index) {
        return nullptr;
    }

    return getAt(leftNeighbourIndex);
}

size_t StatusStructure::getRightNeighbourIndex(size_t index) const {
    size_t currentIndex = index;

    if (rightChild(currentIndex) != nullptr) {
        // if there is a right sub tree find the most left leaf in that tree
        currentIndex = getRightChildIndex(currentIndex);

        while (leftChild(currentIndex) != nullptr) {
            currentIndex = getLeftChildIndex(currentIndex);
        }

        return currentIndex;
    }

    // the root element can't be the left child of another element
    if (currentIndex == 0) {
        return index;
    }

    if (isLeftChild(currentIndex)) {
        // current element is the left child of its parent... hence the parent is the right neighbour
        return getParentIndex(currentIndex);
    }

    // current element is the right child of its parent... traverse upwards to the first node that is not a right child or the root node
    while (currentIndex > 0 && !isLeftChild(currentIndex)) {
        currentIndex = getParentIndex(currentIndex);
    }

    if (currentIndex == 0) {
        return index;
    }

    currentIndex = getParentIndex(currentIndex);

    return currentIndex;
}

LineSegmentPtr StatusStructure::rightNeighbourOf(const LineSegmentPtr fragment) {
    bool valid;
    size_t index = indexOf(fragment, valid);

    if (!valid) {
        std::runtime_error("StatusStructure::rightNeighbourOf: Invalid argument! No such fragment exists");
    }

    size_t rightNeighbourIndex = getRightNeighbourIndex(index);

    if (rightNeighbourIndex == index) {
        return nullptr;
    }

    return getAt(rightNeighbourIndex);
}

size_t StatusStructure::getClosestTo(const Point& point, float& outDist) const {
    size_t index = 0;
    auto currentFrag = getAt(index);

    if (currentFrag == nullptr) {
        return index;
    }

    float dist = getPositionOnScanLine(currentFrag, point);
    size_t closestIndex = index;
    outDist = dist;

    for (;;) {
        if (dist > 0 && leftChild(index) != nullptr) {
            index = getLeftChildIndex(index);
        } else if (dist < 0 && rightChild(index) != nullptr) {
            index = getRightChildIndex(index);
        } else {
            break;
        }

        currentFrag = getAt(index);
        dist = getPositionOnScanLine(currentFrag, point);

        if (std::fabs(outDist) > std::fabs(dist)) {
            closestIndex = index;
            outDist = dist;
        }
    }

    return closestIndex;
}

LineSegmentPtr StatusStructure::fragmentLeftOf(const Point& point) {
    float dist;
    size_t closestIndex = getClosestTo(point, dist);
    auto frag = getAt(closestIndex);

    if (frag == nullptr) {
        return nullptr;
    }

    if (dist > 0 || nearZero(dist)) {
        // The fragment at closestIndex is not left of the point (equal or right)
        // find the first left neighbour that is left of the point.
        while (dist > 0 || nearZero(dist)) {
            size_t leftNeighbourIndex = getLeftNeighbourIndex(closestIndex);

            if (leftNeighbourIndex == closestIndex) {
                // no left neighbour...
                return nullptr;
            }

            closestIndex = leftNeighbourIndex;

            frag = getAt(closestIndex);

            if (frag == nullptr) {
                return nullptr;
            }

            dist = getPositionOnScanLine(frag, point);
        }
    } else {
        // There could be other fragments that intersect the scan line at the same position as frag (e.g same startpoint as frag or intersection with frag)
        // Move right until we find a fragment that is right (or equal) to the point.. return the last fragment that was left
        for (;;) {
            size_t rightNeighbourIndex = getRightNeighbourIndex(closestIndex);
            auto rightFrag = getAt(rightNeighbourIndex);
            dist = getPositionOnScanLine(rightFrag, point);

            if (dist > 0 || nearZero(dist) || rightFrag == nullptr || rightNeighbourIndex == closestIndex) {
                break;
            }

            closestIndex = rightNeighbourIndex;
            frag = rightFrag;
        }
    }

    return frag;
}

LineSegmentPtr StatusStructure::fragmentRightOf(const Point& point) {
    float dist;
    size_t closestIndex = getClosestTo(point, dist);
    auto frag = getAt(closestIndex);

    if (frag == nullptr) {
        return nullptr;
    }

    if (dist < 0 || nearZero(dist)) {
        // find the first right neighbour that is right of the point
        while (dist < 0 || nearZero(dist)) {
            size_t rightNeighbourIndex = getRightNeighbourIndex(closestIndex);

            if (rightNeighbourIndex == closestIndex) {
                return nullptr;
            }

            closestIndex = rightNeighbourIndex;

            frag = getAt(closestIndex);

            if (frag == nullptr) {
                return nullptr;
            }

            dist = getPositionOnScanLine(frag, point);
        }
    } else if (nearZero(dist)) {
        // find the last left neighbour that is still right of the point
        for (;;) {
            size_t leftNeighbourIndex = getLeftNeighbourIndex(closestIndex);
            auto leftFrag = getAt(leftNeighbourIndex);
            dist = getPositionOnScanLine(leftFrag, point);

            if (dist < 0 || nearZero(dist) || leftFrag == nullptr || leftNeighbourIndex == closestIndex) {
                break;
            }

            closestIndex = leftNeighbourIndex;
            frag = leftFrag;
        }
    }

    return frag;
}

void StatusStructure::swapNeighbours(LineSegmentPtr a, LineSegmentPtr b) {
    assert(leftNeighbourOf(a) == b || rightNeighbourOf(a) == b);

    bool valid;
    size_t a_index = indexOf(a, valid);

    if (!valid) {
        throw std::runtime_error("StatusStructure::swapNeighbours: Invalid argument fragment a does not exist.");
    }

    size_t b_index = indexOf(b, valid);

    if (!valid) {
        throw std::runtime_error("StatusStructure::swapNeighbours: Invalid argument fragment b does not exist.");
    }

    insertAt(a_index, b);
    insertAt(b_index, a);
}

void StatusStructure::clear() {
    m_tree.clear();
}
}

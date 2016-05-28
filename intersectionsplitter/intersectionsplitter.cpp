#include "intersectionsplitter.h"
#include "statusstructure.h"
#include "eventqueue.h"
#include "intersectionsplitterutils.h"
#include "segmentset.h"

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <queue>
#include <assert.h>

//#define TRACE_INTERSECTIONSPLITTER

#ifdef TRACE_INTERSECTIONSPLITTER
#include <iostream>
#include <iomanip>
#define TRACE(x) std::cout << std::setprecision(6) << x << std::endl
#else
#define TRACE(x)
#endif

namespace intersectionsplitter {

static LineSegmentPtr getLeftmostFragmentIn(const std::vector<LineSegmentPtr>& fragments, const Point& eventPoint) {
    LineSegmentPtr leftMostFragment = nullptr;
    for (auto& fragment : fragments) {
        if (leftMostFragment == nullptr || isALeftOfBOnScanLine(*fragment, *leftMostFragment, eventPoint)) {
            leftMostFragment = fragment;
        }
    }
    return leftMostFragment;
}

static LineSegmentPtr getRightmostFragmentIn(const std::vector<LineSegmentPtr>& fragments, const Point& eventPoint) {
    LineSegmentPtr rightMostFragment = nullptr;
    for (auto& fragment : fragments) {
        if (rightMostFragment == nullptr || isALeftOfBOnScanLine(*rightMostFragment, *fragment, eventPoint)) {
            rightMostFragment = fragment;
        }
    }
    return rightMostFragment;
}

class IntersectionSplitter {
public:
    IntersectionSplitter(float extendedIntersectionDistance) {
        m_extendedIntersectionDistance = extendedIntersectionDistance;
    }

    std::vector<LineSegmentPtr> splitSegments(const std::vector<LineSegmentPtr>& fragments) {
        bool extendFragments = m_extendedIntersectionDistance > 0.f && !nearZero(m_extendedIntersectionDistance);

        std::vector<LineSegmentPtr> backExtendFragments;

        // Insert fragments into the event queue
        for (LineSegmentPtr segment : fragments) {
            // Modify fragments f so that: f.start.y > f.end.y || (f.start.y == f.end.y && f.start.x < f.end.x)
            directSegment(segment);

            // Add an event for the start and end point
            addNewFragmentToEventQueue(segment);
        }

        // Add fragment extensions to the event queue
        if (extendFragments) {
            for (LineSegmentPtr segment : fragments) {
                // Add extensions fragments at both endpoints (only if no such fragment already exists)
                Point frag_vec = segment->vec().normalized();

                LineSegmentPtr forward_extend = LineSegment::create(segment->end(), segment->end() + frag_vec * m_extendedIntersectionDistance);
                if (addNewFragmentToEventQueueExclusive(forward_extend)) {
                    m_forwardExtendFragments.insert(forward_extend);
                    m_extendFragments[forward_extend] = segment;
                } else {
                    TRACE("Do not add forwad extend " << forward_extend << " of " << segment << " (an equal fragment already exists)");
                }

                LineSegmentPtr backwards_extend = LineSegment::create(segment->start() - frag_vec * m_extendedIntersectionDistance, segment->start());
                if (addNewFragmentToEventQueueExclusive(backwards_extend)) {
                    m_extendFragments[backwards_extend] = segment;
                    backExtendFragments.push_back(backwards_extend);
                } else {
                    TRACE("Do not add backward extend " << backwards_extend << " of " << segment << " (an equal fragment already exists)");
                }
            }
        }

        SegmentSet result;
        result.insert(fragments);

        while (!m_eventQueue.empty()) {
            EventQueue::EventPoint current = m_eventQueue.pop();

            TRACE("Current: " << current.first.x() << ", " << current.first.y());

            // all intersections of the fragments that end at the current EventPoint are already found
            for (const LineSegmentPtr& fragment : current.second.fragmentsEndingHere) {
                TRACE("Remove: " << fragment);
                m_scope.remove(fragment);
            }

            // ...on the other hand fragments that start at the current point need to be considered now
            TRACE("Insert: " << current.second.fragmentsStartingHere);
            m_scope.insert(current.second.fragmentsStartingHere, current.first);

            if (current.second.fragmentsStartingHere.empty()) {
                LineSegmentPtr left = m_scope.fragmentLeftOf(current.first);
                if (left == nullptr)
                    continue;
                LineSegmentPtr right = m_scope.rightNeighbourOf(left);  // m_scope.fragmentRightOf(current.first);
                if (right == nullptr)
                    continue;
                assert(left != right);
                intersectAndSplit(left, right, current, result);
            } else {
                LineSegmentPtr left_1 = getLeftmostFragmentIn(current.second.fragmentsStartingHere, current.first);
                LineSegmentPtr left_2 = m_scope.leftNeighbourOf(left_1);
                assert(left_1 != left_2);
                intersectAndSplit(left_1, left_2, current, result);

                LineSegmentPtr right_1 = getRightmostFragmentIn(current.second.fragmentsStartingHere, current.first);
                LineSegmentPtr right_2 = m_scope.rightNeighbourOf(right_1);
                assert(right_1 != right_2);
                intersectAndSplit(right_1, right_2, current, result);
            }
        }
        // Intersections of a back extending fragment a with another fragment b at the start of a
        // are not recognized (b is removed when a is added). Hence a is not added to the result altough it
        // intersects another fragment... we have to dect those intersections manually
        EventQueue matcherQueue;
        for (LineSegmentPtr backExtSeg : backExtendFragments) {
            matcherQueue.getOrInsert(backExtSeg->start()).fragmentsStartingHere.push_back(backExtSeg);
        }

        for (LineSegmentPtr resultSeg : result) {
            matcherQueue.getOrInsert(resultSeg->end()).fragmentsEndingHere.push_back(resultSeg);
        }

        while (!matcherQueue.empty()) {
            EventQueue::EventPoint current = matcherQueue.pop();
            if (!current.second.fragmentsStartingHere.empty() && !current.second.fragmentsEndingHere.empty()) {
                result.insert(current.second.fragmentsStartingHere);
            }
        }

        m_eventQueue.clear();
        m_scope.clear();
        m_extendFragments.clear();
        m_forwardExtendFragments.clear();

        return result.segments();
    }

    void setExtendedIntersectionDistance(float extendedIntersectionDistance) {
        m_extendedIntersectionDistance = extendedIntersectionDistance;
    }

protected:
    int m_counter = 0;

    bool addNewFragmentToEventQueueExclusive(LineSegmentPtr fragment) {
        EventQueue::EventPointValue& start_ev = m_eventQueue.getOrInsert(fragment->start());
        for (const LineSegmentPtr& other : start_ev.fragmentsStartingHere) {
            if (*other == *fragment) {
                return false;
            }
        }

        start_ev.fragmentsStartingHere.push_back(fragment);
        m_eventQueue.getOrInsert(fragment->end()).fragmentsEndingHere.push_back(fragment);

        return true;
    }

    void addNewFragmentToEventQueue(LineSegmentPtr fragment) {
        m_eventQueue.getOrInsert(fragment->start()).fragmentsStartingHere.push_back(fragment);
        m_eventQueue.getOrInsert(fragment->end()).fragmentsEndingHere.push_back(fragment);
    }

    void intersectAndSplit(LineSegmentPtr a, LineSegmentPtr b, const EventQueue::EventPoint& currentEvent, SegmentSet& fragments) {
        if (a == nullptr || b == nullptr) {
            return;
        }

        auto ext_it_a = m_extendFragments.find(a);
        auto ext_it_b = m_extendFragments.find(b);

        if ((ext_it_a != m_extendFragments.end() && ext_it_b != m_extendFragments.end() && ext_it_a->second == ext_it_b->second) ||
            (ext_it_a != m_extendFragments.end() && ext_it_a->second == b) || (ext_it_b != m_extendFragments.end() && ext_it_b->second == a)) {
            return;
        }

        m_counter++;

        Point intersectionPoint;
        float a_dist;
        float b_dist;
        int intersectType = -1;
        if ((intersectType = intersection(a, b, intersectionPoint, a_dist, b_dist, 0.f)) < 1) {
            return;
        }

        if (intersectType == 2 && ext_it_a != m_extendFragments.end() && ext_it_b != m_extendFragments.end()) {
            // Do not handle intersections between collinear extension fragments
            return;
        }

        TRACE("[" << m_counter << "] Found intersection between " << a << " and " << b << " at: " << intersectionPoint.x() << ", " << intersectionPoint.y());

        EventQueue::EventPointValue dummy;
        EventQueue::EventPointValue* eventPointValue = &dummy;
        bool isPast = true;
        if (inOrder(currentEvent.first, intersectionPoint) || nearZero((currentEvent.first - intersectionPoint).squaredLength())) {
            // The intersection point is below or at the same height and right of the current event point, hence is must be considered as a future event
            eventPointValue = &m_eventQueue.getOrInsert(intersectionPoint);
            isPast = false;
        }

        bool a_splitted = handleIntersectionOfFragment(a, a_dist, eventPointValue, intersectionPoint, isPast, fragments, currentEvent);
        bool b_splitted = handleIntersectionOfFragment(b, b_dist, eventPointValue, intersectionPoint, isPast, fragments, currentEvent);

        if (!a_splitted && !b_splitted) {
            // both fragments intersect but the intersection point is to close to the endpoints
            // hence neither a and b are split and the status structure is not changed.
            // However since a and b intersect they must change places in the status structure
            m_scope.swapNeighbours(a, b);
        }
    }

    bool validSplitDistance(float distance) {
        return !zeroDist(distance);
    }

    bool handleIntersectionOfFragment(LineSegmentPtr fragment, float dist, EventQueue::EventPointValue* intersectionEventPoint, const Point& intersectionPoint,
                                      bool isPast, SegmentSet& fragments, const EventQueue::EventPoint& currentEvent) {
        LineSegmentPtr split = nullptr;

        bool splitValid = validSplitDistance(dist) && validSplitDistance(dist - fragment->length());

        if (splitValid) {
            m_scope.remove(fragment);

            split = LineSegment::create(intersectionPoint, fragment->end());
            fragment->setEnd(intersectionPoint);

            intersectionEventPoint->fragmentsEndingHere.push_back(fragment);
            intersectionEventPoint->fragmentsStartingHere.push_back(split);
            m_eventQueue.getOrInsert(split->end()).fragmentsEndingHere.push_back(split);

            if (!isPast) {
                m_scope.insert(fragment, currentEvent.first);
            }

            TRACE("Modified: " << fragment);
            TRACE("Created : " << split);
        }

        bool isForwardExtendFragment = m_forwardExtendFragments.count(fragment) > 0;

        if (isForwardExtendFragment && validSplitDistance(dist)) {
            fragments.insert(fragment);
            if (splitValid) {
                m_extendFragments[split] = m_extendFragments[fragment];
                m_forwardExtendFragments.insert(split);
            }
        } else if (splitValid) {
            // a normal split or a backwars extending fragment that was split in between
            fragments.insert(split);
        } else if (!isForwardExtendFragment && m_extendFragments.count(fragment) > 0 && !validSplitDistance(dist)) {
            // A backwards extending fragment whose endpoint lies on another fragment
            fragments.insert(fragment);
        }

        return splitValid;
    }

    float m_extendedIntersectionDistance;
    EventQueue m_eventQueue;
    StatusStructure m_scope;

    std::unordered_set<LineSegmentPtr> m_forwardExtendFragments;
    std::unordered_map<LineSegmentPtr, LineSegmentPtr> m_extendFragments;
};

std::vector<LineSegmentPtr> splitLineSegmentsAtIntersections(const std::vector<LineSegmentPtr>& segments, float extendedIntersectionDistance) {
    IntersectionSplitter splitterInstance(extendedIntersectionDistance);
    return splitterInstance.splitSegments(segments);
}
}

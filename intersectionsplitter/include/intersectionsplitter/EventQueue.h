#pragma once

#include <vector>
#include <map>

#include "LineSegment.h"

namespace intersectionsplitter {

class EventQueue {
public:
    EventQueue() {
    }

    struct EventPointValue {
        std::vector<LineSegmentPtr> fragmentsStartingHere;
        std::vector<LineSegmentPtr> fragmentsEndingHere;
    };

    typedef std::pair<Point, EventPointValue> EventPoint;

    EventPointValue& getOrInsert(Point eventPointPosition);

    EventPoint pop();

    bool empty() const;

    void clear();

private:
    struct eventqueueorder {
        bool operator()(const Point& lhs, const Point& rhs) const;
    };

    typedef std::map<Point, EventPointValue, eventqueueorder> EventQueueStore;
    EventQueueStore m_eventQueue;
};
}

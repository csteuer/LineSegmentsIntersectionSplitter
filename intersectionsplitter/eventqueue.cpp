#include "eventqueue.h"
#include "intersectionsplitterutils.h"

namespace intersectionsplitter {

EventQueue::EventPointValue& EventQueue::getOrInsert(Point eventPointPosition) {
    EventPoint p(eventPointPosition, EventPointValue());
    std::pair<EventQueueStore::iterator, bool> insertResult = m_eventQueue.insert(p);

    return insertResult.first->second;
}

EventQueue::EventPoint EventQueue::pop() {
    EventPoint p = *m_eventQueue.begin();
    m_eventQueue.erase(m_eventQueue.begin());
    return p;
}

bool EventQueue::empty() const {
    return m_eventQueue.empty();
}

void EventQueue::clear() {
    m_eventQueue.clear();
}

bool EventQueue::eventqueueorder::operator()(const Point& lhs, const Point& rhs) const {
    return inOrder(lhs, rhs);
}
}

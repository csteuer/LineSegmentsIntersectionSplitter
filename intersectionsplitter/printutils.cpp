#include "printutils.h"

namespace intersectionsplitter {

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x() << ", " << p.y() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const LineSegment& vec) {
    os << "[s: " << vec.start() << ", e: " << vec.end() << ", len: " << vec.length() << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const LineSegmentPtr& vec) {
    if (vec == nullptr) {
        os << "null";
    } else {
        os << *vec;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<LineSegmentPtr>& list) {
    for (const auto& segment : list) {
        os << segment << std::endl;
    }

    return os;
}
}

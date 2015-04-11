#pragma once

#include <iostream>
#include <vector>

#include "LineSegment.h"

namespace intersectionsplitter {
    ::std::ostream& operator<<(::std::ostream& os, const Point& p);
    ::std::ostream& operator<<(::std::ostream& os, const LineSegment& vec);
    ::std::ostream& operator<<(::std::ostream& os, const LineSegmentPtr& vec);
    ::std::ostream& operator<<(::std::ostream& os, const std::vector<LineSegmentPtr>& list);
}

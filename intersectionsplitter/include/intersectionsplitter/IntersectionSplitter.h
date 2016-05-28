#pragma once

#include <vector>

#include "intersectionsplitter_api.h"
#include "LineSegment.h"

/**
 * @brief contains the intersectionsplitter types and functions
 */
namespace intersectionsplitter {

/**
 * @brief computes all intersections between the segments and splits the segments at the intersection points.
 * @param segments a list of line segments.
 * @param extendedIntersectionDistance the maximum distance between the nearest endpoint of a segment and an intersection.
 * (for both directions a segment is only extended to the nearest intersection)
 * @return the splitted segments.
 */
INTERSECTIONSPLITTER_API std::vector<LineSegmentPtr> splitLineSegmentsAtIntersections(const std::vector<LineSegmentPtr>& segments,
                                                                                      float extendedIntersectionDistance = 0.f);
}

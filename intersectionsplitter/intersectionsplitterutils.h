#pragma once

#include "linesegment.h"

namespace intersectionsplitter {

bool nearZero(const float& value);

bool zeroDist(float dist);

bool inOrder(const Point& lhs, const Point& rhs);

LineSegmentPtr directSegment(LineSegmentPtr frag);

/**
 * @brief calculates the intersection of two fragments. If a and b overlap the endpoint of b that is closest to the start of a is reported as intersection.
 * @param outIntersectionPoint the intersection point if a and b intersect, otherwise unchanged
 * @param outDistanceFromAStart the distance between a's start point and the intersection point (undefined of no intersection)
 * @param outDistanceFromBStart the distance between b's start point and the intersection point (undefined of no intersection)
 * @param maxDistance the maximum distance between the intersection point and the nearest endpoint of both fragments
 * @return -1 -> a,b do not intersect but are parallel,
 *         -2 -> a,b do not intersect but are on the same line,
 *          0 -> a,b do not intersect,
 *          1 -> a,b intersect,
 *          2 -> a,b intersect and are on the same line (overlap)
 */
int intersection(const LineSegment& a, const LineSegment& b, Point& outIntersectionPoint, float& outDistanceFromAStart, float& outDistanceFromBStart,
                 float maxDistance = 0.f);
int intersection(const LineSegmentPtr& a, const LineSegmentPtr& b, Point& outIntersectionPoint, float& outDistanceFromAStart, float& outDistanceFromBStart,
                 float maxDistance = 0.f);

/**
 * @brief calculates the intersection of a wall fragment with an infinite line
 * @param lineSupportVector an arbitrary point on the line
 * @param lineNormedVector the normalized direction vector of the line
 * @param segment the line fragment
 * @return the signed eucildian distance from the lineSupportVector to the intersection point or nan (not a number) if there is no intersection
 */
float lineIntersection(const Point& lineSupportVector, const Point& lineNormedVector, const LineSegment& segment);
float lineIntersection(const Point& lineSupportVector, const Point& lineNormedVector, const LineSegmentPtr& segment);

/**
 * @brief calculates the position of the fragment on the scan line
 * @param segment the fragment
 * @param eventPoint the event point that determines the position of the scan line (the scan line goes through the event point)
 * @return the position of the fragment on the scan line for the current event point
 */
float getPositionOnScanLine(const LineSegment& segment, const Point& eventPoint);
float getPositionOnScanLine(const LineSegmentPtr& segment, const Point& eventPoint);

/**
 * @brief determines wether a is left of b on the scan line
 * @param eventPoint the event point that determines the position of the scan line (the scan line goes through the event point)
 * @return  true if a is left of b, false otherwise
 */
bool isALeftOfBOnScanLine(const LineSegment& segment_a, const LineSegment& segment_b, const Point& eventPoint);
bool isALeftOfBOnScanLine(const LineSegmentPtr& segment_a, const LineSegmentPtr& segment_b, const Point& eventPoint);
}

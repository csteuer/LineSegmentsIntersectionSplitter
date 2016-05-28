#include <intersectionsplitter/IntersectionSplitterUtils.h>

#include <cmath>
#include <limits>
#include <stdexcept>
#include <assert.h>

namespace intersectionsplitter {

constexpr Point scanLineVec(1.f, 0.f);

bool zeroDist(float dist) {
    return std::fabs(dist) < 0.1f;
}

bool nearZero(const float& value) {
    const float epsilon = 0.0001f;

    return std::fabs(value) < epsilon;
}

bool inOrder(const Point& lhs, const Point& rhs) {
    if (nearZero(lhs.y() - rhs.y())) {
        return lhs.x() < rhs.x();
    }
    return lhs.y() > rhs.y();
}

LineSegmentPtr directSegment(LineSegmentPtr frag) {
    if (!inOrder(frag->start(), frag->end())) {
        frag->swapEndPoints();
    }

    return frag;
}

int intersection(const LineSegment& a, const LineSegment& b, Point& outIntersectionPoint, float& outDistanceFromAStart, float& outDistanceFromBStart,
                 float maxDistance) {
    // Let: a_s = a.start(), a_v = a.vec(), b_s = b.start() and b_v = b.vec()
    // Solve: a_s + t * a_v = b_s + r * b_v
    // (a_s + t * a_v) x b_v        = (b_s + r * b_v) x b_v | x denotes the cross product
    // a_s x b_v + t * a_v x b_v    = b_s x b_v             | v x v = 0
    // t * a_v x b_v                = b_s x b_v - a_s x b_v
    // t                            = ((b_s - a_s) x b_v) / (a_v x b_v)
    // r                            = ((a_s - b_s) x a_v) / -(a_v x b_v)

    Point a_vec = a.vec().normalized();
    Point b_vec = b.vec().normalized();

    float a_len = a.length();
    float b_len = b.length();

    Point diff = b.start() - a.start();
    float cross_vecs = a_vec.cross(b_vec);
    float cross_diff = diff.cross(b_vec);

    if (nearZero(cross_vecs)) {
        // a and b are collinear
        if (!nearZero(cross_diff)) {
            // a and b are parallel
            return -1;
        }

        // a and b are on the same line... check for overlap by projecting b's endpoints onto a
        float proj_bs = a_vec.dot(diff);                 // b.start() onto a
        float proj_be = a_vec.dot(b.end() - a.start());  // b.end() onto a

        float nearest = proj_bs;
        outDistanceFromBStart = 0.f;
        if (std::fabs(proj_be) < std::fabs(proj_bs)) {
            nearest = proj_be;
            outDistanceFromBStart = b_len;
        }

        if (nearest < -maxDistance || nearest > a_len + maxDistance) {
            // no overlap
            return -2;
        }

        outDistanceFromAStart = nearest;
        outIntersectionPoint = a.start() + nearest * a_vec;

        return 2;
    }

    float t = cross_diff / cross_vecs;
    if (t < -maxDistance || t > a_len + maxDistance) {
        return 0;
    }

    float r = (-diff).cross(a_vec) / -cross_vecs;
    if (r < -maxDistance || r > b_len + maxDistance) {
        return 0;
    }

    outDistanceFromAStart = t;
    outDistanceFromBStart = r;
    outIntersectionPoint = a.start() + t * a_vec;

    return 1;
}

int intersection(const LineSegmentPtr a, const LineSegmentPtr b, Point& outIntersectionPoint, float& outDistanceFromAStart, float& outDistanceFromBStart,
                 float maxDistance) {
    return intersection(*a, *b, outIntersectionPoint, outDistanceFromAStart, outDistanceFromBStart, maxDistance);
}

float lineIntersection(const Point& lineSupportVector, const Point& lineNormedVector, const LineSegment& segment) {
    // See intersection function for explanation
    Point diff = segment.start() - lineSupportVector;
    Point frag_vec = segment.vec();
    float crossVecs = lineNormedVector.cross(frag_vec);
    float crossDiff = diff.cross(frag_vec);

    if (nearZero(crossVecs)) {
        if (!nearZero(crossDiff)) {
            return std::numeric_limits<float>::quiet_NaN();
        }

        float distance = diff.length();

        if (lineNormedVector.dot(diff) > 0) {
            return distance;
        }

        return -distance;
    }

    float r = (-diff).cross(lineNormedVector) / -crossVecs;
    if ((r < 0 && !nearZero(r)) || (r > 1 && !nearZero(1.f - r))) {
        return std::numeric_limits<float>::quiet_NaN();
    }

    float t = crossDiff / crossVecs;

    return t;
}

float lineIntersection(const Point& lineSupportVector, const Point& lineNormedVector, const LineSegmentPtr segment) {
    return lineIntersection(lineSupportVector, lineNormedVector, *segment);
}

float getPositionOnScanLine(const LineSegment& segment, const Point& eventPoint) {
    float val = lineIntersection(eventPoint, scanLineVec, segment);
    assert(!std::isnan(val));
    return val;
}

float getPositionOnScanLine(const LineSegmentPtr segment, const Point& eventPoint) {
    return getPositionOnScanLine(*segment, eventPoint);
}

bool isALeftOfBOnScanLine(const LineSegment& segment_a, const LineSegment& segment_b, const Point& eventPoint) {
    float posA = getPositionOnScanLine(segment_a, eventPoint);
    float posB = getPositionOnScanLine(segment_b, eventPoint);

    if (nearZero(posA - posB)) {
        // Both fragments have the same position on the scan line (they intersect the scan line at the same point)

        // Fragments parallel to the scan line are always right of fragments not parallel to the scan line
        // (note: this convention only works if event points are ordered from left to right if they have the same y-coordinate...
        // hence a fragment parallel to the scan line always ends right of the event point. Otherwise its end point would be its start point)
        bool aParallelToScanLine = nearZero(segment_a.vec().cross(scanLineVec));
        bool bParallelToScanLine = nearZero(segment_b.vec().cross(scanLineVec));

        if (aParallelToScanLine && bParallelToScanLine) {
            // If both are parallel...
            return segment_a.end().x() < segment_b.end().x();
        } else if (aParallelToScanLine) {
            return false;
        } else if (bParallelToScanLine) {
            return true;
        }

        // Move the scan line down (as far as possible) and use the fragments position on that scan line
        Point newScanLinePos(0.f, std::max(segment_a.end().y(), segment_b.end().y()));

        posA = getPositionOnScanLine(segment_a, newScanLinePos);
        posB = getPositionOnScanLine(segment_b, newScanLinePos);

        if (nearZero(posA - posB)) {
            // The fragments are on the same (vertical) line... let the one with the lowest y-coordinate be left
            if (!nearZero(segment_a.end().y() - segment_b.end().y())) {
                return segment_a.end().y() < segment_b.end().y();
            }

            if (!nearZero(segment_a.start().y() - segment_b.start().y())) {
                return segment_a.start().y() < segment_b.start().y();
            }

            throw std::runtime_error("Duplicate Fragment.");
        }
    }

    return posA < posB;
}

bool isALeftOfBOnScanLine(const LineSegmentPtr segment_a, const LineSegmentPtr segment_b, const Point& eventPoint) {
    return isALeftOfBOnScanLine(*segment_a, *segment_b, eventPoint);
}
}

#pragma once

#include <memory>

#include "intersectionsplitter_api.h"

namespace intersectionsplitter {

class INTERSECTIONSPLITTER_API Point {
public:
    constexpr Point() : m_x(0.f), m_y(0.f) {
    }

    constexpr Point(float x, float y) : m_x(x), m_y(y) {
    }

    float x() const;
    void setX(float x);

    float y() const;
    void setY(float y);

    Point operator-(const Point& other) const;
    Point operator+(const Point& other) const;
    Point operator*(const float& value) const;
    Point normalized() const;
    float cross(const Point& other) const;
    float dot(const Point& other) const;
    float length() const;
    float squaredLength() const;

private:
    float m_x;
    float m_y;
};

Point operator*(const float& scalar, const Point& point);
Point operator-(const Point& point);

class LineSegment;
typedef std::shared_ptr<LineSegment> LineSegmentPtr;

class INTERSECTIONSPLITTER_API LineSegment {
public:
    static LineSegmentPtr create(const Point& start, const Point& end);
    static LineSegmentPtr create(float x1, float y1, float x2, float y2);
    static LineSegment init(const Point& start, const Point& end);
    static LineSegment init(float x1, float y1, float x2, float y2);

    const Point& start() const;
    void setStart(const Point& start);

    const Point& end() const;
    void setEnd(const Point& end);

    void swapEndPoints();

    Point vec() const;

    float length() const;

    bool valid() const;

    bool operator==(const LineSegment& other) const;

private:
    LineSegment(const Point& start, const Point& end);
    Point m_start;
    Point m_end;
};
}

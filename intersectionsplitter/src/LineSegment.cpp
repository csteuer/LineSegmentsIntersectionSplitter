#include <intersectionsplitter/LineSegment.h>
#include <intersectionsplitter/IntersectionSplitterUtils.h>
#include <cmath>

namespace intersectionsplitter {

    Point::Point()
    {
        m_x = 0.f;
        m_y = 0.f;
    }

    Point::Point(float x, float y)
    {
        m_x = x;
        m_y = y;
    }

    float Point::x() const
    {
        return m_x;
    }

    void Point::setX(float x)
    {
        m_x = x;
    }
    float Point::y() const
    {
        return m_y;
    }

    void Point::setY(float y)
    {
        m_y = y;
    }

    Point Point::operator-(const Point& other) const
    {
        return Point(this->x() - other.x(), this->y() - other.y());
    }

    Point Point::operator+(const Point& other) const
    {
        return Point(this->x() + other.x(), this->y() + other.y());
    }

    Point Point::operator*(const float& value) const
    {
        return Point(this->x() * value, this->y() * value);
    }

    Point Point::normalized() const
    {
        float len_inv = 1 / this->length();
        return Point(this->x() * len_inv, this->y() * len_inv);
    }

    float Point::cross(const Point& other) const
    {
        return this->x() * other.y() - this->y() * other.x();
    }

    float Point::dot(const Point& other) const
    {
        return this->x() * other.x() + this->y() * other.y();
    }

    float Point::length() const
    {
        return std::sqrt(this->squaredLength());
    }

    float Point::squaredLength() const
    {
        return this->x() * this->x() + this->y() * this->y();
    }

    LineSegment::LineSegment(const Point& start, const Point& end)
    {
        m_start = start;
        m_end = end;
    }

    LineSegmentPtr LineSegment::create(const Point& start, const Point& end)
    {
        return std::shared_ptr<LineSegment>(new LineSegment(start, end));
    }

    LineSegmentPtr LineSegment::create(float x1, float y1, float x2, float y2)
    {
        return create(Point(x1, y1), Point(x2, y2));
    }

    LineSegment LineSegment::init(const Point& start, const Point& end)
    {
        return LineSegment(start, end);
    }

    LineSegment LineSegment::init(float x1, float y1, float x2, float y2)
    {
        return init(Point(x1, y1), Point(x2, y2));
    }

    const Point& LineSegment::start() const
    {
        return m_start;
    }

    void LineSegment::setStart(const Point& start)
    {
        m_start = start;
    }

    const Point& LineSegment::end() const
    {
        return m_end;
    }

    void LineSegment::setEnd(const Point& end)
    {
        m_end = end;
    }

    void LineSegment::swapEndPoints()
    {
        Point tmp = m_end;
        m_end = m_start;
        m_start = tmp;
    }

    Point LineSegment::vec() const
    {
        return m_end - m_start;
    }

    float LineSegment::length() const
    {
        return vec().length();
    }

    bool LineSegment::valid() const
    {
        return std::isfinite(m_start.x()) && std::isfinite(m_start.y()) && std::isfinite(m_end.x()) && std::isfinite(m_end.y()) && !nearZero(this->length());
    }

    bool LineSegment::operator==(const LineSegment& other) const
    {
        return (nearZero((m_start - other.start()).squaredLength()) && nearZero((m_end - other.end()).squaredLength())) ||
                (nearZero((m_end - other.start()).squaredLength()) && nearZero((m_start - other.end()).squaredLength()));
    }

    Point operator*(const float& scalar, const Point& point)
    {
        return point * scalar;
    }

    Point operator-(const Point& point)
    {
        return Point(-point.x(), -point.y());
    }

}

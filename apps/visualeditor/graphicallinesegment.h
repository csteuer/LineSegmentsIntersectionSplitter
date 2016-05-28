#pragma once

#include <intersectionsplitter/linesegment.h>
#include <memory>

class QGraphicsItemGroup;
class QPointF;
class QGraphicsEllipseItem;
class QGraphicsLineItem;

class GraphicalLineSegment {
public:
    GraphicalLineSegment(intersectionsplitter::LineSegmentPtr lineSegment, QGraphicsItemGroup* graphicsContainer);
    GraphicalLineSegment(const QPointF& start, const QPointF& end, QGraphicsItemGroup* graphicsContainer);

    intersectionsplitter::LineSegmentPtr getLineSegment();
    const intersectionsplitter::LineSegmentPtr getLineSegment() const;

    void updateGraphicalRepresentation();

    const QGraphicsEllipseItem* getEndItem() const;
    const QGraphicsEllipseItem* getStartItem() const;

    ~GraphicalLineSegment();

private:
    void updatePositions();

    QGraphicsEllipseItem* m_startPointGraphicsItem;
    QGraphicsEllipseItem* m_endPointGraphicsItem;
    QGraphicsLineItem* m_lineGraphicsItem;
    QGraphicsItemGroup* m_graphicsContainer;
    intersectionsplitter::LineSegmentPtr m_lineSegment;
};

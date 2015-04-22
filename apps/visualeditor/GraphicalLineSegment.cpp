#include "GraphicalLineSegment.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsScene>

#include "Utils.hpp"

const float pointRadius = 5.f;
const QBrush pointBrush(Qt::blue, Qt::SolidPattern);
const QPen pointPen(QBrush(Qt::black, Qt::SolidPattern), 1);
const QPen linePen(QBrush(Qt::black, Qt::SolidPattern), 2);


GraphicalLineSegment::GraphicalLineSegment(intersectionsplitter::LineSegmentPtr lineSegment, QGraphicsItemGroup* graphicsContainer)
{
    m_startPointGraphicsItem = nullptr;
    m_endPointGraphicsItem = nullptr;
    m_lineGraphicsItem = nullptr;

    m_lineSegment = lineSegment;
    m_graphicsContainer = graphicsContainer;
    updateGraphicalRepresentation();
}

GraphicalLineSegment::GraphicalLineSegment(const QPointF& start, const QPointF& end, QGraphicsItemGroup* graphicsContainer)
{
    m_startPointGraphicsItem = nullptr;
    m_endPointGraphicsItem = nullptr;
    m_lineGraphicsItem = nullptr;

    m_lineSegment = intersectionsplitter::LineSegment::create((float) start.x(), (float) start.y(), (float) end.x(), (float) end.y());
    m_graphicsContainer = graphicsContainer;
    updateGraphicalRepresentation();
}

intersectionsplitter::LineSegmentPtr GraphicalLineSegment::getLineSegment()
{
    return m_lineSegment;
}

const intersectionsplitter::LineSegmentPtr GraphicalLineSegment::getLineSegment() const
{
    return m_lineSegment;
}

const QGraphicsEllipseItem* GraphicalLineSegment::getEndItem() const
{
    return m_startPointGraphicsItem;
}

const QGraphicsEllipseItem* GraphicalLineSegment::getStartItem() const
{
    return m_endPointGraphicsItem;
}

GraphicalLineSegment::~GraphicalLineSegment()
{

}

QGraphicsEllipseItem* createPointItem() {
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-pointRadius, -pointRadius, 2.f * pointRadius, 2.f * pointRadius);
    item->setBrush(pointBrush);
    item->setPen(pointPen);
    item->setZValue(1);
    return item;
}

bool hasPointAt(const QGraphicsItemGroup* group, const intersectionsplitter::Point& position) {
    return getEllipseItemAt(group->scene(), group->mapToScene(toQPoint(position))) != nullptr;
}

void GraphicalLineSegment::updateGraphicalRepresentation()
{
    // Update positions first as this might change the outcome of the hasPointAt method (e.g. consider if the points of a line segment are swapped)
    updatePositions();

    if (m_startPointGraphicsItem == nullptr && !hasPointAt(m_graphicsContainer, m_lineSegment->start())) {
        m_startPointGraphicsItem = createPointItem();
        m_graphicsContainer->addToGroup(m_startPointGraphicsItem);
    }

    if (m_endPointGraphicsItem == nullptr && !hasPointAt(m_graphicsContainer, m_lineSegment->end())) {
        m_endPointGraphicsItem = createPointItem();
        m_graphicsContainer->addToGroup(m_endPointGraphicsItem);
    }

    if (!m_lineGraphicsItem) {
        m_lineGraphicsItem = new QGraphicsLineItem();
        m_lineGraphicsItem->setPen(linePen);
        m_lineGraphicsItem->setZValue(0);
        m_graphicsContainer->addToGroup(m_lineGraphicsItem);
    }

    updatePositions();
}

void GraphicalLineSegment::updatePositions()
{
    if (m_lineGraphicsItem) {
        m_lineGraphicsItem->setLine(QLineF(0, 0, m_lineSegment->vec().x(), m_lineSegment->vec().y()));
        m_lineGraphicsItem->setPos(m_lineSegment->start().x(), m_lineSegment->start().y());
    }

    if (m_startPointGraphicsItem) {
        m_startPointGraphicsItem->setPos(m_lineSegment->start().x(), m_lineSegment->start().y());
    }

    if (m_endPointGraphicsItem) {
        m_endPointGraphicsItem->setPos(m_lineSegment->end().x(), m_lineSegment->end().y());
    }
}

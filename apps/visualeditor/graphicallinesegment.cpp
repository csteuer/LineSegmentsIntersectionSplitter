#include "graphicallinesegment.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsScene>

#include "utils.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

const double pointRadius = 5.0;
const QBrush pointBrush(Qt::blue, Qt::SolidPattern);
const QPen pointPen(QBrush(Qt::black, Qt::SolidPattern), 1);
const QPen linePen(QBrush(Qt::black, Qt::SolidPattern), 2);
#ifdef __clang__
#pragma clang diagnostic pop
#endif

GraphicalLineSegment::GraphicalLineSegment(intersectionsplitter::LineSegmentPtr lineSegment, QGraphicsItemGroup* graphicsContainer) {
    m_startPointGraphicsItem = nullptr;
    m_endPointGraphicsItem = nullptr;
    m_lineGraphicsItem = nullptr;

    m_lineSegment = lineSegment;
    m_graphicsContainer = graphicsContainer;
    updateGraphicalRepresentation();
}

GraphicalLineSegment::GraphicalLineSegment(const QPointF& start, const QPointF& end, QGraphicsItemGroup* graphicsContainer) {
    m_startPointGraphicsItem = nullptr;
    m_endPointGraphicsItem = nullptr;
    m_lineGraphicsItem = nullptr;

    m_lineSegment = intersectionsplitter::LineSegment::create(static_cast<float>(start.x()), static_cast<float>(start.y()), static_cast<float>(end.x()),
                                                              static_cast<float>(end.y()));
    m_graphicsContainer = graphicsContainer;
    updateGraphicalRepresentation();
}

intersectionsplitter::LineSegmentPtr GraphicalLineSegment::getLineSegment() {
    return m_lineSegment;
}

const intersectionsplitter::LineSegmentPtr GraphicalLineSegment::getLineSegment() const {
    return m_lineSegment;
}

const QGraphicsEllipseItem* GraphicalLineSegment::getEndItem() const {
    return m_startPointGraphicsItem;
}

const QGraphicsEllipseItem* GraphicalLineSegment::getStartItem() const {
    return m_endPointGraphicsItem;
}

GraphicalLineSegment::~GraphicalLineSegment() {
}

static QGraphicsEllipseItem* createPointItem() {
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-pointRadius, -pointRadius, 2.0 * pointRadius, 2.0 * pointRadius);
    item->setBrush(pointBrush);
    item->setPen(pointPen);
    item->setZValue(1);
    return item;
}

static bool hasPointAt(const QGraphicsItemGroup* group, const intersectionsplitter::Point& position) {
    return getEllipseItemAt(group->scene(), group->mapToScene(toQPoint(position))) != nullptr;
}

void GraphicalLineSegment::updateGraphicalRepresentation() {
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

void GraphicalLineSegment::updatePositions() {
    const double lineStartX = static_cast<double>(m_lineSegment->start().x());
    const double lineStartY = static_cast<double>(m_lineSegment->start().y());

    if (m_lineGraphicsItem) {
        m_lineGraphicsItem->setLine(QLineF(0, 0, static_cast<double>(m_lineSegment->vec().x()), static_cast<double>(m_lineSegment->vec().y())));
        m_lineGraphicsItem->setPos(lineStartX, lineStartY);
    }

    if (m_startPointGraphicsItem) {
        m_startPointGraphicsItem->setPos(lineStartX, lineStartY);
    }

    if (m_endPointGraphicsItem) {
        m_endPointGraphicsItem->setPos(static_cast<double>(m_lineSegment->end().x()), static_cast<double>(m_lineSegment->end().y()));
    }
}

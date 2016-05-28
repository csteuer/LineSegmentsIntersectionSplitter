#include "interactivegraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

#include "graphicallinesegment.h"
#include "utils.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

InteractiveGraphicsScene::InteractiveGraphicsScene() : QGraphicsScene(QRect(0, 0, 200, 200)) {
    m_sceneRoot = new QGraphicsItemGroup();
    this->addItem(m_sceneRoot);
    m_currentLineInCreation = nullptr;
}

InteractiveGraphicsScene::~InteractiveGraphicsScene() {
}

void InteractiveGraphicsScene::addLineSegment(intersectionsplitter::LineSegmentPtr lineSegment) {
    if (m_lineSegmentsSet.count(lineSegment.get()) > 0) {
        return;
    }

    m_graphicalLineSegments.push_back(std::unique_ptr<GraphicalLineSegment>(new GraphicalLineSegment(lineSegment, m_sceneRoot)));
    m_lineSegmentsSet.insert(lineSegment.get());
}

std::vector<intersectionsplitter::LineSegmentPtr> InteractiveGraphicsScene::getLineSegments() {
    std::vector<intersectionsplitter::LineSegmentPtr> out;
    for (std::unique_ptr<GraphicalLineSegment>& gfxSegment : m_graphicalLineSegments) {
        out.push_back(gfxSegment->getLineSegment());
    }

    return out;
}

void InteractiveGraphicsScene::updateLineSegmentsGraphicalRepresentation() {
    for (const auto& lineSegmentGfx : m_graphicalLineSegments) {
        lineSegmentGfx->updateGraphicalRepresentation();
    }
}

void InteractiveGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        event->accept();
        m_lastPressPos = event->screenPos();
        m_lastDragPos = event->scenePos();
        m_dragMode = true;
    }
}

void InteractiveGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton && (event->screenPos() - m_lastPressPos).manhattanLength() < 10) {
        event->accept();

        handleMouseClick(event);
    }
    m_dragMode = false;
}

void InteractiveGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_dragMode) {
        QPointF delta = event->scenePos() - m_lastDragPos;
        m_lastDragPos = event->scenePos();
        handleDrag(delta);
    } else if (m_currentLineInCreation != nullptr) {
        QPointF pos = tryGetPositionOfExistingPoint(event->scenePos(), m_sceneRoot->mapFromScene(event->scenePos()), m_currentLineInCreation->getEndItem());
        m_currentLineInCreation->getLineSegment()->setEnd(intersectionsplitter::Point(static_cast<float>(pos.x()), static_cast<float>(pos.y())));
        m_currentLineInCreation->updateGraphicalRepresentation();
    }
}

QPointF InteractiveGraphicsScene::tryGetPositionOfExistingPoint(const QPointF& atScenePos, const QPointF& defaultPos, const QGraphicsEllipseItem* except) {
    QGraphicsEllipseItem* p = getEllipseItemAt(this, atScenePos, except);

    if (p != nullptr) {
        return p->pos();
    }

    return defaultPos;
}

void InteractiveGraphicsScene::handleMouseClick(QGraphicsSceneMouseEvent* event) {
    QPointF pos = tryGetPositionOfExistingPoint(event->scenePos(), m_sceneRoot->mapFromScene(event->scenePos()));

    if (m_currentLineInCreation != nullptr) {
        m_lineSegmentsSet.insert(m_currentLineInCreation->getLineSegment().get());
        m_graphicalLineSegments.push_back(std::move(m_currentLineInCreation));
        m_currentLineInCreation = nullptr;

    } else {
        m_currentLineInCreation = std::unique_ptr<GraphicalLineSegment>(new GraphicalLineSegment(pos, pos, m_sceneRoot));
    }
}

void InteractiveGraphicsScene::handleDrag(const QPointF& delta) {
    m_sceneRoot->setTransform(m_sceneRoot->transform().translate(delta.x(), delta.y()));
}

#pragma once

#include <memory>
#include <unordered_set>

#include <QGraphicsScene>
#include <QPointF>

#include <intersectionsplitter/linesegment.h>

class GraphicalLineSegment;

class InteractiveGraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    InteractiveGraphicsScene();
    virtual ~InteractiveGraphicsScene();

    void addLineSegment(intersectionsplitter::LineSegmentPtr lineSegment);
    std::vector<intersectionsplitter::LineSegmentPtr> getLineSegments();

    void updateLineSegmentsGraphicalRepresentation();

    // QGraphicsScene interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void handleMouseClick(QGraphicsSceneMouseEvent* event);
    void handleDrag(const QPointF& delta);

    QPointF tryGetPositionOfExistingPoint(const QPointF& atScenePos, const QPointF& defaultPos, const QGraphicsEllipseItem* except = nullptr);

    QPointF m_lastPressPos;
    QPointF m_lastDragPos;
    bool m_dragMode;
    std::unique_ptr<GraphicalLineSegment> m_currentLineInCreation;
    QGraphicsItemGroup* m_sceneRoot;

    std::list<std::unique_ptr<GraphicalLineSegment>> m_graphicalLineSegments;
    std::unordered_set<intersectionsplitter::LineSegment*> m_lineSegmentsSet;
};

#pragma once

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <intersectionsplitter/linesegment.h>

inline QPointF toQPoint(const intersectionsplitter::Point& p) {
    return QPointF(static_cast<qreal>(p.x()), static_cast<qreal>(p.y()));
}

/**
 * @brief returns the first QGraphicsEllipseItem at a position in the group
 * @param scene the scene that should be queried
 * @param point the position in the scene coordinate system
 * @param except an item that will not be found (however other items at the same position but with lower z-value will be found instead)
 * @return a pointer to the ellipse item or nullptr if there is no item at that position
 */
inline QGraphicsEllipseItem* getEllipseItemAt(const QGraphicsScene* scene, const QPointF& point, const QGraphicsEllipseItem* except = nullptr) {
    QList<QGraphicsItem*> items = scene->items(point);

    for (QGraphicsItem* item : items) {
        QGraphicsEllipseItem* p = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        if (p != nullptr && p != except) {
            return p;
        }
    }

    return nullptr;
}

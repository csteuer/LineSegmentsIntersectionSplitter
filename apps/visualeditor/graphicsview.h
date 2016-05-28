#pragma once

#include <QGraphicsView>
#include <QPoint>

class GraphicsView : public QGraphicsView {
public:
    GraphicsView(QWidget* parent = nullptr);

protected:
    // Capture wheel events for zooming
    virtual void wheelEvent(QWheelEvent* event);
};

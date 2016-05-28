#include "graphicsview.h"

#include <QWheelEvent>
#include <QMouseEvent>

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {
    this->setDragMode(QGraphicsView::NoDrag);
    this->viewport()->setMouseTracking(true);
    setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
}

void GraphicsView::wheelEvent(QWheelEvent* event) {
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if (event->delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

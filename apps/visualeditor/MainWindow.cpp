#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QRect>

#include "InteractiveGraphicsScene.h"

#include <intersectionsplitter/IntersectionSplitter.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new InteractiveGraphicsScene();
    ui->graphicsView->setScene(scene);

    connect(ui->splitBtn, SIGNAL(clicked()), this, SLOT(splitLines()));
    connect(ui->exportBtn, SIGNAL(clicked()), this, SLOT(exportLineSegments()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}


void MainWindow::splitLines()
{
    std::vector<intersectionsplitter::LineSegmentPtr> input = scene->getLineSegments();
    std::vector<intersectionsplitter::LineSegmentPtr> output = intersectionsplitter::splitLineSegmentsAtIntersections(input);

    scene->updateLineSegmentsGraphicalRepresentation();

    for (intersectionsplitter::LineSegmentPtr& segment : output) {
        scene->addLineSegment(segment);
    }
}

void MainWindow::exportLineSegments()
{
    std::vector<intersectionsplitter::LineSegmentPtr> segments = scene->getLineSegments();

    for (size_t i = 0; i < segments.size(); i++) {
        intersectionsplitter::LineSegmentPtr lineSegment = segments.at(i);
        std::cout << "SEGMENT(" << lineSegment->start().x() << ", " << lineSegment->start().y() << ", " << lineSegment->end().x() << ", " << lineSegment->end().x() << ")";
        if (i != segments.size() - 1) std::cout << ",";
        std::cout << std::endl;
    }
}

#include "MainWindow.h"
#include "PathWindow.h"

#include <QAction>
#include <QDir>
#include <QToolBar>
#include <iostream>

MainWindow::MainWindow() {
  setWindowTitle("Gcode-o-tron");

  pathWindow = new PathWindow;
  setCentralWidget(pathWindow);

  toolBar = addToolBar(tr("Manage"));
  auto startAction = new QAction(tr("Load gcode"));
  toolBar->addAction(startAction);

  resize(800, 800);

  QDir path("/home/jacob/Downloads");
  parser = std::make_unique<GCodeParser>(
      path.absoluteFilePath("150-DIAMOND_TREE.gcode").toStdString());

  parser->addObserver(this);

  connect(startAction, &QAction::triggered, this, [this]() {
    if (parser)
      parser->run();
  });
}

MainWindow::~MainWindow() { // i enable forward declared unique_ptrs from header
}

void MainWindow::newSegment(Segment s) {
  // root add new segment
  // for (const auto &v : s) {
  //   std::cout << v.x << std::endl;
  // }
  static int segcount = 0;
  segcount++;
  setWindowTitle(QString("Segments: %1").arg(segcount));
  pathWindow->addSegment(std::move(s));
}

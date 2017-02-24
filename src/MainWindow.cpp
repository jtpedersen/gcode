#include "MainWindow.h"
#include "PathWindow.h"

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
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

  connect(startAction, &QAction::triggered, this, [this]() {
    const auto folder =
        QStandardPaths::standardLocations(QStandardPaths::DownloadLocation)[0];
    const auto fn = QFileDialog::getOpenFileName(
        this, tr("Open GCode"), folder, tr("Gcode Files (*.ngc *.nc *.gcode)"));
    loadFile(fn);
  });
}

MainWindow::~MainWindow() { // i enable forward declared unique_ptrs from header
}

void MainWindow::loadFile(const QString &path) {
  pathWindow->clear();
  parser = std::make_unique<GCodeParser>(path.toStdString());
  parser->addObserver(this);
  parser->run();
}

void MainWindow::newSegment(Segment s) {
  static int segcount = 0;
  segcount++;
  setWindowTitle(QString("Segments: %1").arg(segcount));
  pathWindow->addSegment(std::move(s));
}

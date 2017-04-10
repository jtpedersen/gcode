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
  auto startAction = new QAction(tr("Load gcode"), this);
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
  parser = new GCodeParser(path.toStdString(), this);
  connect(parser, &GCodeParser::newSegment, pathWindow,
          &PathWindow::addSegment);
  connect(parser, &QThread::finished, parser, &QObject::deleteLater);
  connect(parser, &QThread::finished, this, [this] { pathWindow->update(); });
  parser->start();
}

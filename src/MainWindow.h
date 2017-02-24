#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "GCodeParser.h"

#include <QMainWindow>
#include <memory>

class PathWindow;
class QToolBar;

class MainWindow : public QMainWindow, public SegmentObserver {
public:
  MainWindow();
  virtual ~MainWindow();

  virtual void newSegment(Segment s) override;

private:
  void loadFile(const QString &);

  PathWindow *pathWindow;
  QToolBar *toolBar;
  std::unique_ptr<GCodeParser> parser;
};

#endif /* !MAINWINDOW_H_ */

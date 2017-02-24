#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "GCodeParser.h"

#include <QMainWindow>
#include <memory>

class PathWindow;
class QToolBar;

class MainWindow : public QMainWindow {
public:
  MainWindow();
  virtual ~MainWindow();

private:
  void loadFile(const QString &);

  PathWindow *pathWindow;
  QToolBar *toolBar;
  GCodeParser *parser;
};

#endif /* !MAINWINDOW_H_ */

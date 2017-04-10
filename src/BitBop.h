#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>

class QLabel;
class QToolBar;

class BitBop : public QMainWindow {
public:
  BitBop();

private:
  QLabel *main;
  QToolBar *toolBar;

  void loadFile(const QString& fileName);
};

#endif /* !MAINWINDOW_H_ */

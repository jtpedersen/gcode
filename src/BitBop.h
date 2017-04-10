#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include <QImage>

class QLabel;
class QToolBar;

class BitBop : public QMainWindow {
public:
  BitBop();

private:
  QLabel *main;
  QToolBar *toolBar;
  QImage image;

  void loadFile(const QString &fileName);

  void endMillIt();
};

#endif /* !MAINWINDOW_H_ */

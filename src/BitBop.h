#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QImage>
#include <QMainWindow>
#include <vector>

class QLabel;
class QToolBar;

class ToolKernel {
  std::vector<float> heights;
  int size;
  ToolKernel(int size);
public:
  // Create a flat endmill at size 
  static ToolKernel endmill(int size);
  // static ToolKernel vbit();
  // static ToolKernel ballnose();
  /// Return depth at i,j z if outsize, centered on size/2 size/2
  float get(int i, int j);
  /// width/height its square
  int w() const;
};

class BitBop : public QMainWindow {
public:
  BitBop();

private:
  QLabel *main;
  QToolBar *toolBar;
  QImage image;

  void loadFile(const QString &fileName);

  void endMillIt();

  void toolConvolve(const QImage &src, QImage &dst, const ToolKernel k);
};

#endif /* !MAINWINDOW_H_ */

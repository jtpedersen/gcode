#include "BitBop.h"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QLabel>
#include <QStandardPaths>
#include <QToolBar>

#include <iostream>
#include <limits>

ToolKernel::ToolKernel(int size) : size(size) { heights.reserve(size * size); };

ToolKernel ToolKernel::endmill(int size) {
  ToolKernel tk(size);
  const auto rSq = (size / 2) * (size / 2);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      const auto dx = std::abs(i - size / 2);
      const auto dy = std::abs(j - size / 2);
      const auto dSq = dx * dx + dy * dy;
      const auto idx = i + size * j;
      tk.heights[idx] = (dSq < rSq) ? 0 : std::numeric_limits<int>::max();
    }
  }
  return tk;
}

float ToolKernel::get(int i, int j) {
  const auto x = i + size / 2;
  const auto y = j + size / 2;
  if (x < 0 || x >= size || y < 0 || y >= size)
    return 0.0f;
  return heights[x + size * y];
}

int ToolKernel::w() const { return size; }

BitBop::BitBop() {
  setWindowTitle("Bit bop");

  main = new QLabel;
  setCentralWidget(main);

  toolBar = addToolBar(tr("Manage"));
  auto startAction = new QAction(tr("Load image"), this);
  auto emAction = new QAction(tr("End mill"), this);
  toolBar->addAction(startAction);
  toolBar->addAction(emAction);

  resize(800, 800);

  connect(startAction, &QAction::triggered, this, [this]() {
    const auto folder =
        QStandardPaths::standardLocations(QStandardPaths::DownloadLocation)[0];
    const auto fn = QFileDialog::getOpenFileName(
        this, tr("Open heightmap"), folder,
        tr("Image Files (*.png *.jpg *.jpeg *.gif)"));
    loadFile(fn);
  });

  connect(emAction, &QAction::triggered, this, [this]() { endMillIt(); });
}

void BitBop::loadFile(const QString &path) {
  image = QImage(path);
  image = image.convertToFormat(QImage::Format_Grayscale8);
  const auto pixels = QPixmap::fromImage(image).scaled(main->size());
  main->setPixmap(pixels);
}

void BitBop::endMillIt() {
  auto dst = image.copy();
  toolConvolve(image, dst, ToolKernel::endmill(4));
  const auto pixels = QPixmap::fromImage(dst);
  main->setPixmap(pixels);
}

void BitBop::toolConvolve(const QImage &src, QImage &dst, const ToolKernel tk) {

  auto eval = [&src, &tk, N = tk.w() / 2 ](int x, int y) {
    const auto p = src.constBits();
    const auto stride = src.width();
    int deep = std::numeric_limits<int>::max();
    const auto val = p[x + y * stride];
    for (int j = -N; j <= N; j++) {
      for (int i = -N; i <= N; i++) {
        const auto px = x + i;
        const auto py = y + j;
        const auto k = tk.get(i,j);
        const auto cur = p[px + py * stride];
        deep = std::max(c
      }
    }
    return deep;
  };

  const auto stride = image.width();
  auto bits = image.bits();
  for (int j = 0; j < image.height(); j++) {
    for (int i = 0; i < image.width(); i++) {
      bits[i + j * stride] = eval(i, j);
    }
  }
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  BitBop bb;
  bb.show();
  return app.exec();
}

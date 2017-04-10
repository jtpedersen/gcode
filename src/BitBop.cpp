#include "BitBop.h"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QLabel>
#include <QStandardPaths>
#include <QToolBar>

#include <iostream>

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

  auto eval = [img = image.copy()](int x, int y) {
    const auto p = img.constBits();
    const auto stride = img.width();
    constexpr int N = 4;
    int sum = 0;
    const auto val = p[x + y * stride];
    for (int j = -N; j <= N; j++) {
      for (int i = -N; i <= N; i++) {
        const auto dist = (i * i) + (j * j);
        if (dist < N * N)
          continue;
        const auto px = x + i;
        const auto py = y + j;
        if (px < 0 || py < 0 || px >= img.width() || py >= img.height())
          continue;
        const auto cur = p[px + py * stride];
        if (cur > val)
          return 255;
        sum += val - cur;
      }
    }
    return sum / (4*N*N);
  };

  const auto stride = image.width();
  auto bits = image.bits();
  for (int j = 0; j < image.height(); j++) {
    for (int i = 0; i < image.width(); i++) {
      bits[i + j * stride] = eval(i,j);
    }
  }

  const auto pixels = QPixmap::fromImage(image);
  main->setPixmap(pixels);

}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  BitBop bb;
  bb.show();
  return app.exec();
}

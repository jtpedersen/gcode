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
  toolBar->addAction(startAction);

  resize(800, 800);

  connect(startAction, &QAction::triggered, this, [this]() {
    const auto folder =
        QStandardPaths::standardLocations(QStandardPaths::DownloadLocation)[0];
    const auto fn = QFileDialog::getOpenFileName(
        this, tr("Open heightmap"), folder,
        tr("Image Files (*.png *.jpg *.jpeg *.gif)"));
    loadFile(fn);
  });
}

void BitBop::loadFile(const QString &path) {
  QImage img(path);
  main->setPixmap(QPixmap::fromImage(img).scaled(main->size()));
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  BitBop bb;
  bb.show();
  return app.exec();
}

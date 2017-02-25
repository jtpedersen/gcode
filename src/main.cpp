#include "MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QVector3D>
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(3, 2);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  qRegisterMetaType<QList<QVector3D>>();

  MainWindow window;

  window.show();
  return app.exec();
}

#include "MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  if (argc < 2) {
    std::cout << "Usage " << argv[0] << " gcode-file" << std::endl;
    exit(-1);
  }

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(3, 2);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  MainWindow window;

  window.show();
  return app.exec();
}

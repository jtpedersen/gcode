#include "PathWindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>

// #include "OrbitTransformController.h"
#include "GCodeParser.h"
#include <iostream>

struct Observer : public SegmentObserver {
  Observer(PathWindow *p) : p(p){};

public:
  virtual void newSegment(Segment s) override {
    // root add new segment
    // for (const auto &v : s) {
    //   std::cout << v.x << std::endl;
    // }
    p->addSegment(std::move(s));
  }

private:
  PathWindow *p;
};

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

  auto pw = new PathWindow;
  GCodeParser gcp(argv[1]);
  gcp.addObserver(std::make_shared<Observer>(pw));

  gcp.run();

  QMainWindow window;
  window.setWindowTitle("Gcode-o-tron");

  window.setCentralWidget(pw);
  window.resize(800, 800);
  window.show();

  return app.exec();
}

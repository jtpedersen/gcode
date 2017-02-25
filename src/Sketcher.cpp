#include "Sketcher.h"

#include <QGraphicsScene>

Sketcher::Sketcher(QWidget *parent) : QGraphicsView(parent) {
  setScene(new QGraphicsScene(this));
  setTransformationAnchor(AnchorUnderMouse);
  setDragMode(ScrollHandDrag);
  setViewportUpdateMode(FullViewportUpdate);
}
Sketcher::~Sketcher() {}

void Sketcher::loadFile(const QString &fileName){};

#include "Sketcher.h"

#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>

Sketcher::Sketcher(QWidget *parent) : QGraphicsView(parent) {
  setScene(new QGraphicsScene(this));
  setTransformationAnchor(AnchorUnderMouse);
  setDragMode(ScrollHandDrag);
  setViewportUpdateMode(FullViewportUpdate);
}
Sketcher::~Sketcher() {}

void Sketcher::loadFile(const QString &fileName) {
  auto s = scene();

  QScopedPointer<QGraphicsSvgItem> svgItem(new QGraphicsSvgItem(fileName));
  if (!svgItem->renderer()->isValid())
    return;

  s->clear();
  resetTransform();

  sketchPath = svgItem.take();
  sketchPath->setFlags(QGraphicsItem::ItemClipsToShape);
  sketchPath->setCacheMode(QGraphicsItem::NoCache);
  sketchPath->setZValue(0);

  s->addItem(sketchPath);

  return;
}

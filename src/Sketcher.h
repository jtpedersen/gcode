#ifndef SKETCHER_H_
#define SKETCHER_H_
#include <QGraphicsView>

class Sketcher : public QGraphicsView {
  Q_OBJECT
public:
  Sketcher(QWidget *parent = nullptr);
  virtual ~Sketcher();

  void loadFile(const QString &fileName);
};

#endif /* !SKETCHER_H_ */

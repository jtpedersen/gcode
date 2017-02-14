#ifndef PATHWINDOW_H_
#define PATHWINDOW_H_

#include <QOpenGLWindow>

class PathWindow : public QOpenGLWidget {
public:
  PathWindow(QWindow *parent);
  ~PathWindow();
  
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
};


#endif /* !PATHWINDOW_H_ */

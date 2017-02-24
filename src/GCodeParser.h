#ifndef GCODEPARSER_H_
#define GCODEPARSER_H_
#include <QThread>
#include <memory>
#include <vector>

#include <QList>
#include <QVector3D>

enum Units { mm, inches };

class GCodeParser : public QThread {
  Q_OBJECT

public:
  GCodeParser(std::string file, QObject *parent =nullptr);

  /// start parsing the file
  void run() override;

signals:
  void newSegment(QList<QVector3D>);

private:
  struct gtoken {
    char type;
    union {
      int ival;
      double fval;
    };
    gtoken(char type, int ival);
    gtoken(char type, double fval);
    static bool integerType(char type);
    static bool floatType(char type);
    friend std::ostream &operator<<(std::ostream &os, const gtoken &g);
  };
  std::vector<gtoken> tokenize(std::string line);
  void handleTokens(std::vector<gtoken> tokens);
  void startNewSegment();

  std::string filename;

  QVector3D pos;
  bool absolute;
  Units units;
  QList<QVector3D> current;
};

#endif /* !GCODEPARSER_H_ */

#ifndef GCODEPARSER_H_
#define GCODEPARSER_H_
#include <memory>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>

enum Units { mm, inches };
using Segment = std::vector<glm::vec3>;

/// Called with new segments
struct SegmentObserver {
  virtual void newSegment(Segment s) = 0;
};

class GCodeParser {

public:
  GCodeParser(std::string file);

  /// start parsing the file
  void run();

  void addObserver(SegmentObserver *so);

private:
  /// notify observes on new Segment
  void newSegment(Segment s);

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
  void addPosToCurrentSegment();


  std::string filename;
  std::vector<SegmentObserver*> observers;

  glm::vec3 pos;
  bool absolute;
  Units units;
  Segment current;
};

#endif /* !GCODEPARSER_H_ */

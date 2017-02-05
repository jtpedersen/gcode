#ifndef GCODEPARSER_H_
#define GCODEPARSER_H_
#include <vector>
#include <memory>

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

  void addObserver(std::shared_ptr<SegmentObserver> so);

private:
  /// notify observes on new Segment
  void newSegment(Segment s);
  std::string filename;
  std::vector<std::shared_ptr<SegmentObserver>> observers;
};


#endif /* !GCODEPARSER_H_ */

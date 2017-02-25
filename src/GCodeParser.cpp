#include "GCodeParser.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <utility>

using namespace std;
using Token = GCodeParser::gtoken;

GCodeParser::gtoken::gtoken(char type, int ival) : type(type), ival(ival){};
GCodeParser::gtoken::gtoken(char type, double fval) : type(type), fval(fval){};

GCodeParser::gtoken::gtoken(const std::string &strToken) {
  type = strToken[0];
  const auto number = strToken.substr(1);

  if (GCodeParser::gtoken::integerType(type)) {
    ival = std::stoi(number);
    //    cout << strToken << " Integer as " << ival << " from " << number <<
    //    std::endl;
  } else if (GCodeParser::gtoken::floatType(type)) {
    fval = QString(number.c_str()).toFloat();
    //    cout << strToken << " Float " << fval << " from " << number <<
    //    std::endl;
  } else {
    cout << "ignored: " << strToken << endl;
  }
}
bool GCodeParser::gtoken::integerType(char type) {
  return 'G' == type || 'M' == type || 'T' == type;
}

bool GCodeParser::gtoken::floatType(char type) {
  return 'A' == type || 'X' == type || 'Y' == type || 'Z' == type ||
         'F' == type || 'E' == type;
}

// interpret a line of tokens
void GCodeParser::handleTokens(vector<gtoken> tokens) {
  auto nextPos(pos);
  bool extruding = false;
  for (auto t : tokens) {
    switch (t.type) {
    case 'M':
    case 'F':
    case 'T':
    case 'S':
      // ignore we are only interested in moves
      break;
    case 'G':
      // check for go home, set mm/inches etc for now just chuck along
      // if ( t.ival == 1 || t.ival == 0 ) {

      // } else {

      // }
      if (t.ival == 20) {
        units = inches;
      } else if (t.ival == 21) {
        units = mm;
      } else if (t.ival == 90) {
        absolute = true;
      } else if (t.ival == 91) {
        absolute = false;
      }
      break;
    case 'X':
      nextPos.setX(t.fval + (absolute ? 0.0f : pos.x()));
      break;
    case 'Y':
      nextPos.setY(t.fval + (absolute ? 0.0f : pos.y()));
      break;
    case 'Z':
      nextPos.setZ(t.fval + (absolute ? 0.0f : pos.z()));
      break;
    case 'E':
      extruding = t.fval > 0;
      break;
    default:
      cout << "unknown type:" << t.type << endl;
    }
    //      cout << t << endl;
  }

  pos = nextPos;
  if (extruding) {
    //    cout << nextPos.x() << ", " << nextPos.y() << ", " << nextPos.z() <<
    //    endl;
    current << pos;

  } else if (!current.isEmpty()) {
    current << pos;
    startNewSegment();
  }
}
// add another Segment iff the current is nonempty
void GCodeParser::startNewSegment() {
  newSegment(current);
  current.clear();
}

void removeComments(string &line) {
  auto sep = line.find(";");
  if (sep != string::npos)
    line.erase(sep);
}

vector<GCodeParser::gtoken> GCodeParser::tokenize(string line) {
  static regex ws_re("\\s+"); // whitespace
  vector<gtoken> tokens;
  if (line.empty()) {
    return tokens;
  }

  auto begin = sregex_token_iterator(line.begin(), line.end(), ws_re, -1);
  auto end = sregex_token_iterator();
  if (begin == end) {
    cout << "nothing on line:" << line << endl;
  }
  for (auto it = begin; it != end; it++) {
    tokens.emplace_back(*it);
  }
  return tokens;
}

GCodeParser::GCodeParser(std::string filename, QObject *parent)
    : QThread(parent), filename(filename), pos(0, 0, 0), absolute(false),
      units(mm) {}

void GCodeParser::run() {
  Q_ASSERT(GCodeParser::runTests());
  string line;
  fstream fs(filename);
  int lineCnt = 0;

  while (getline(fs, line)) {
    removeComments(line);
    auto tokens = tokenize(line);
    handleTokens(tokens);
    lineCnt++;
    if (0 == (lineCnt % 1000))
      cout << "Read :" << lineCnt << " Lines" << endl;
  }
  startNewSegment();
}

namespace {
bool testInt(const std::string &in, char type, int val, bool expect = true) {
  cout << "Test: " << in << " == " << type << "" << val << "     ";

  Token t(in);
  auto ok = (t.type == type) && (t.ival == val);
  ok = ok == expect;
  cout << (ok ? "OK" : "FAIL") << endl;
  return ok;
}

bool testFloat(const std::string &in, char type, float val,
               bool expect = true) {
  cout << "Test: " << in << " == " << type << "" << val << "     ";

  Token t(in);
  bool ok = true;
  if (t.type != type) {
    cout << "Expected:" << type << " Got " << t.type;
    ok = false;
  }

  if ((std::abs(t.fval - val) > .001f)) {
    cout << "Expected:" << val << " Got " << t.fval;
    ok = false;
  }

  ok = ok == expect;

  cout << "  " << (ok ? "OK" : "FAIL") << endl;
  return ok;
}
}

bool GCodeParser::runTests() {
  bool ok = true;
  ok &= testInt("G1", 'G', 1);
  ok &= testInt("M1", 'M', 1);

  ok &= testFloat("X1.0", 'X', 1.0f);
  ok &= testFloat("X1,0", 'X', 1.0f, false);
  ok &= testFloat("X-1,0", 'X', -1.0f, false);

  ok &= testFloat("X11.0", 'X', 11.0f);
  ok &= testFloat("X11,0", 'X', 11.0f, false);
  ok &= testFloat("X-11.0", 'X', -11.0f);

  ok &= testFloat("X1.23", 'X', 1.23f);
  ok &= testFloat("X1,23", 'X', 1.23f, false);
  ok &= testFloat("X-1.23", 'X', -1.23f);
  return ok;
}

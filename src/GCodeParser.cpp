#include "GCodeParser.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <utility>

using namespace std;

GCodeParser::gtoken::gtoken(char type, int ival) : type(type), ival(ival){};
GCodeParser::gtoken::gtoken(char type, double fval) : type(type), fval(fval){};
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
    }
    //      cout << t << endl;
  }
  if (nextPos != pos) {
    pos = nextPos;
    if (extruding) {
      //      cout << nextPos.x() << ", " << nextPos.y() << ", " << nextPos.z()
      //      <<
      //      endl;
      current << pos;

    } else {
      startNewSegment();
    }
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
  if (line.empty())
    return tokens;

  auto begin = sregex_token_iterator(line.begin(), line.end(), ws_re, -1);
  auto end = sregex_token_iterator();
  for (auto it = begin; it != end; it++) {
    string match = *it;
    char type = match[0];
    std::string number(match.substr(1));

    if (GCodeParser::gtoken::integerType(type)) {
      tokens.emplace_back(type, std::stoi(number));
      // cout << match <<" Integer as " << tokens.back().ival << std::endl;
    } else if (GCodeParser::gtoken::floatType(type)) {
      tokens.emplace_back(type, std::stod(number));
      // cout << match <<" Float " << tokens.back().fval << std::endl;
    } else {
      // cout << "ignored: " << match;
    }
  }
  return tokens;
}

GCodeParser::GCodeParser(std::string filename, QObject *parent)
  : QThread(parent), filename(filename), pos(0,0,0), absolute(false), units(mm) {}

void GCodeParser::run() {
  string line;
  fstream fs(filename);
  while (getline(fs, line)) {
    removeComments(line);
    auto tokens = tokenize(line);
    handleTokens(tokens);
  }
}

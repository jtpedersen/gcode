#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <iterator>

using namespace std;

struct gcode {
  char type;
  union {
    int ival;
    double fval;
  };
  gcode(char type, int ival)   : type(type), ival(ival) {};
  gcode(char type, double fval) : type(type), fval(fval) {};
static bool integerType(char type) {   return 'G' == type || 'M' == type || 'T' == type; }
  friend ostream& operator << (ostream& os, const gcode& g) {
    os << g.type << (integerType(g.type) ? g.ival : g.fval);
    return os;
  }
};

void removeComments(string& line) {
    auto sep = line.find(";");
    if (sep != string::npos)
      line.erase(sep);
}

int main(int argc, char *argv[]) {

  string line;
  regex ws_re("\\s+"); // whitespace
  
  while(getline(cin, line)) {
    removeComments(line);
    if (line.empty()) 
      continue;
    vector<gcode> tokens;
    auto begin = sregex_token_iterator(line.begin(), line.end(), ws_re, -1);
    auto end = sregex_token_iterator();
    for(auto it = begin; it != end; it++) {
      string match = *it;
      char type = match[0];
      if (gcode::integerType(type)) {
	tokens.emplace_back(type, atoi(match.c_str()+1));
      } else {
	tokens.emplace_back(type, atof(match.c_str()+1));
      }
    }
    for(auto t : tokens) 
      cout << t << endl;
    cout << endl;

  }
  return 0;
}

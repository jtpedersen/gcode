#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <iterator>

using namespace std;

struct gtoken {
  char type;
  union {
    int ival;
    double fval;
  };
  gtoken(char type, int ival)   : type(type), ival(ival) {};
  gtoken(char type, double fval) : type(type), fval(fval) {};
  static bool integerType(char type) {   return 'G' == type || 'M' == type || 'T' == type; }
  friend ostream& operator << (ostream& os, const gtoken& g) {
    os << g.type << (integerType(g.type) ? g.ival : g.fval);
    return os;
  }
};

struct State {
  double x,y,z,e;
  State() : x(0), y(0), z(0), e(0) { }
  State(const State& o) : x(o.x), y(o.y), z(o.z), e(0) {}
  const bool operator==(const State& other) const { 
    return other.x == x && other.y == y && other.z == z && other.e == e; 
  }
  const bool operator!=(const State& other) const { 
    return ! (other == *this);
  }
};

void removeComments(string& line) {
  auto sep = line.find(";");
  if (sep != string::npos)
    line.erase(sep);
}

vector<gtoken> tokenize(string line) {
  static regex ws_re("\\s+"); // whitespace
  vector<gtoken> tokens;
  if (line.empty()) 
    return tokens;

  auto begin = sregex_token_iterator(line.begin(), line.end(), ws_re, -1);
  auto end = sregex_token_iterator();
  for(auto it = begin; it != end; it++) {
    string match = *it;
    char type = match[0];
    if (gtoken::integerType(type)) {
      tokens.emplace_back(type, atoi(match.c_str()+1));
    } else {
      tokens.emplace_back(type, atof(match.c_str()+1));
    }
  }
  return tokens;
}

int main(int argc, char *argv[]) {
  string line;
  State state;
  while(getline(cin, line)) {
    removeComments(line);
    auto tokens = tokenize(line);
    auto nextState(state);
    for(auto t : tokens) {
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
	if (t.ival == 90) {
	  //absolute
	} else if (t.ival == 91) {
	  //relative
	} else if (t.ival == 92) {
	  // set cur as 0
	}
	break;
      case 'X':
	nextState.x += t.fval;
	break;
      case 'Y':
	nextState.y += t.fval;
	break;
      case 'Z':
	nextState.z += t.fval;
	break;
      case 'E':
	nextState.e += t.fval;
	break;

      }
      //      cout << t << endl;
    }
    if (nextState != state) {
      if (nextState.e  >0) {
	cout << nextState.x  << ", " << nextState.y << ", " <<  nextState.z << endl;
      }
      state = nextState;
    }


  }
  return 0;
}

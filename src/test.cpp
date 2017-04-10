#include "GCodeParser.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  return GCodeParser::runTests() ? EXIT_SUCCESS : EXIT_FAILURE;
}

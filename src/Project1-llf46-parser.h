#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <algorithm> 
#include <map>

#include "Project1-llf46-data.h"

void parseFile(const std::string &filename, std::map<int, std::string> &rules, std::vector<ClauseVariable> &clauses);
std::string trim(const std::string &str);
std::string removeCommas(const std::string &str);

#endif

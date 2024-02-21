// backwardChaining.h

#ifndef BACKWARDCHAINING_H
#define BACKWARDCHAINING_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <stack>
#include <set>
#include <map>
#include <iomanip> 
#include <chrono> 
#include <ctime> 

#include "data.h"
#include "parser.h"

// Prototypes for output functions
void testingOutput(const std::string text);
std::string getCurrentTime();
void printRules(const std::map<int, std::string>& rules);
void printClauses(std::vector<std::reference_wrapper<ClauseVariable>> &clauses);
void printBoolComparison(const std::vector<bool>& requiredClauseBools, const std::vector<bool>& actualClauseBools);
void printConclusions(std::vector<std::string> &conclusions);
void getUserInput(ClauseVariable &tempClause);
std::vector<bool> determineRealClauseBools(std::vector<std::reference_wrapper<ClauseVariable>> &currentRuleClauses);
std::vector<bool> determineRequiredClauseBools(std::vector<std::reference_wrapper<ClauseVariable>> &currentRuleClauses);
bool compareClauseBools(std::vector<bool> &requiredBools, std::vector<bool> &actualBools);
void printClauseNumbers(const std::vector<int> &clauseNumbers);

// Prototypes for backward chaining algorithm functions
AttackResult Attacks_BW();
std::vector<std::string> getConclusions(const std::map<int, std::string>& rules);
std::string trim(std::string &str);
int calcClauseNumber(int ruleNumber);

#endif // BACKWARDCHAININGFUNCTIONS_H

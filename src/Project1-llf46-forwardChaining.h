#ifndef FORWARD_CHAINING_
#define FORWARD_CHAINING_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Project1-llf46-data.h"

// Function prototypes
void initializeArr(std::string array[]);
void populateRules(Rules rule[], std::string array[]);
void populateVariables(Variables var[], std::string array[]);
void populateConclusions(Conclusions con[], std::string array[]);
int Search_CVL(Variables var[], std::string type);
int Clause_To_Rule(int clauseNum);
void identifyCheck(std::string identified, Variables var[]);
int validate_Ri(int ruleNum, Rules rules[], Variables var[], Conclusions conclusion[]);
Conclusions forwardChaining(AttackResult);

#endif // FORWARD_CHAINING_

#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include <iostream>
#include <optional>

struct ClauseVariable
{
    std::string name;
    std::vector<int> clauseNumbers; 
    std::string meaning;
    bool instantiated = false;
    std::optional<bool> response;
};

struct Rule
{
    int ruleNumber;
    std::string conclusion;
};

// values initially assume no attack was found
struct AttackResult
{
    std::string typeOfAttack = "None";
    bool attackWasIdentified = false;
};

// Structure for the rules
struct Rules {
    std::vector<std::string> conditions;
    std::vector<int> clauseNum;
    std::string conclusion;
};

// Structure for the variables
struct Variables {
    std::string variable;
    std::string type;
    std::string instantiated;
    int clauseNum;
};

// Structure for the conclusions
struct Conclusions {
    std::string conclusion;
    std::string description;
};

#endif
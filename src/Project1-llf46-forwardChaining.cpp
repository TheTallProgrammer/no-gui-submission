#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Project1-llf46-forwardChaining.h"

//puts all info into an array organized by chunk
void initializeArr(std::string array[]){
    std::ifstream inputFile("Project1-llf46-kbfc.txt"); // Open file
    if (!inputFile) {
        std::cout << "Error opening file." << std::endl;
        return;
    }
    std::string line;
    std::string paragraph;
    int counter = 0;

    while(std::getline(inputFile, line)){
        if(!line.empty()){
            paragraph += line + "\n";
        }
        else{
            array[counter] = paragraph;
            paragraph = "";
            counter++;
        }
    }

    // Add the last paragraph if it exists
    if (!paragraph.empty()) {
        array[counter] = paragraph;
    }

    inputFile.close();
}
//populated the rules structure array with each rule
void populateRules(Rules rule[], std::string array[]){
  for(int i = 0; i < 17; i++){
    std::string paragraph = array[i];
    // Parse the paragraph
    std::istringstream iss(paragraph);
    std::string line;
    
    while (std::getline(iss, line)) {
      if(line.find("Condition:") != std::string::npos){
        int pos1 = line.find("Condition: ") + 11;
        int length = line.find(',') - pos1;
        std::string condition = line.substr(pos1, length);
        rule[i].conditions.push_back(condition);
        if(i != 16){
          pos1 = line.find("Clause: ") + 8;
          length = line.find(',') - pos1;
          std::string clause = line.substr(pos1, length);
          int num = std::stoi(clause);
          rule[i].clauseNum.push_back(num);
        }
        }else if(line.find("Conclusion: ") != std::string::npos){
          int pos1 = line.find("Conclusion: ") + 12;
          int length = line.length() - pos1;
          std::string conclusion = line.substr(pos1, length);
          rule[i].conclusion = conclusion;
        }
    }
  }
}

//populates the variables structure array with each variable
void populateVariables(Variables var[], std::string array[]){
  //counter for var[] array
  int counter = 0;
  //loops through array starting at the first variable chunk to last
  for(int i = 16; i < 32; i++){
    std::string paragraph = array[i];
    // Parse the paragraph
    std::istringstream iss(paragraph);
    std::string line;
    while (std::getline(iss, line)) {
      if(line.find("Variable:") != std::string::npos){
        int pos1 = line.find("Variable: ") + 10;
        int length = line.length() - pos1;
        std::string variable = line.substr(pos1, length);
        var[counter].variable = variable;
      }else if(line.find("Type:") != std::string::npos){
        int pos1 = line.find("Type: ") + 6;
        int length = line.length() - pos1;
        std::string type = line.substr(pos1, length);
        var[counter].type = type;
      }else if(line.find("Instantiated:") != std::string::npos){
        int pos1 = line.find("Instantiated: ") + 13;
        int length = line.length() - pos1;
        std::string instantiated = line.substr(pos1, length);
        var[counter].instantiated = instantiated;
      }else if(line.find("Clause Number:") != std::string::npos){
        int pos1 = line.find("Clause Number: ") + 15;
        int length = line.length() - pos1;
        std::string clause = line.substr(pos1, length);
        int num = std::stoi(clause);
        var[counter].clauseNum = num;
      }
    }
    counter++;
  }
}

void populateConclusions(Conclusions con[], std::string array[]){
    int counter = 0;

    for(int i = 32; i < 49; i++){  // Assuming 17 rules in total
        std::string paragraph = array[i];
        std::istringstream iss(paragraph);
        std::string line;
        bool foundConclusion = false;

        // Reset description for each new conclusion
        con[counter].description = "";

        while (std::getline(iss, line)){
            if(line.find("Conclusion:") != std::string::npos){
                int pos1 = line.find("Conclusion: ") + 12;
                con[counter].conclusion = line.substr(pos1);
                foundConclusion = true;
            } else if (foundConclusion) {
                // Append to description only after finding the conclusion
                con[counter].description += line + "\n";
            }
        }

        if (!foundConclusion) {
            // Handle case where no conclusion is found in the paragraph
            con[counter].conclusion = "No conclusion found";
        }

        counter++;
    }
}


//searches for the clause number, returns clause number or -1 if not found
int Search_CVL(Variables var[], std::string type){
  //std::cout << type << std::endl;
  for(int i = 0; i < 17; i++){
    if(type == var[i].variable){
      var[i].instantiated = "true";
      return var[i].clauseNum;
    }
  }
  return 4;
}

int Clause_To_Rule(int clauseNum){
  int ruleNum = clauseNum / 4;
  
  return ruleNum;
}

void identifyCheck(bool identified, Variables var[]){
  if(identified){
    var[0].instantiated = "true"; // attack_was_identified is true if identified is passed in
  }
}

int validate_Ri(int ruleNum, Rules rules[], Variables var[], Conclusions conclusion[], AttackResult result){
 
  int i = ruleNum - 1;
  int size = rules[i].conditions.size();
  int validated = true;
    for(int i = 0; i < 17; i++){
      if( conclusion[i].conclusion  == result.typeOfAttack){
        return i;
      }
    }
  return -1;
}

Conclusions forwardChaining(AttackResult result){
  //initialize variables for testing
  std::string type = "attack_was_" + result.typeOfAttack;
  bool identified = result.attackWasIdentified;
  std::string array[48];
  initializeArr(array);
  
  Rules rules[17];
  populateRules(rules, array);

  Variables variables[16];
  populateVariables(variables, array);
  
  Conclusions conclusion[16];
  populateConclusions(conclusion, array);

  int Ci = Search_CVL(variables, type);

  int Ri = Clause_To_Rule(Ci);

  //check if attack was identified
  identifyCheck(identified, variables);

  //returns the index of the conclution
  int conclusionIndex = validate_Ri(Ri, rules, variables, conclusion, result);

  if(conclusionIndex == -1){
    std::cout << "couldn't find conclusion index in validate_Ri" << std::endl;
  }
  
  return conclusion[conclusionIndex];
}



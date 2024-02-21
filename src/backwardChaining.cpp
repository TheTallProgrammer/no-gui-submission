#include "backwardChaining.h"

// test output functions
std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* ptm = std::localtime(&currentTime);
    char buffer[32];
    // Format: YYYY-MM-DD HH:mm:ss
    std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm); 
    return std::string(buffer);
    // this is a test
}
void testingOutput(const std::string text) {
    // The symbols for the border can be changed to anything you like
    const std::string timeStr = getCurrentTime();
    std::cout << "* [" << timeStr << "] - " << text << std::endl;
    std::cout << "" << std::endl;
}
void printClauses(std::vector<std::reference_wrapper<ClauseVariable>>& clauses) {
    std::cout << "----------------- Clauses -----------------\n";
    for (const auto& clauseRef : clauses) {
        const ClauseVariable& clause = clauseRef.get();
        std::cout << "Clause Name: " << clause.name << std::endl;
    }
    std::cout << "" << std::endl;
}
void printConclusions(const std::vector<std::string>& conclusions) {
    std::cout << "--------------- Conclusions ---------------\n";
    for (const auto& conclusion : conclusions) {
        std::cout << "- " << conclusion << "\n";
    }
    std::cout << "------------------------------------------\n";
}
void printClauseNumbers(const std::vector<int>& clauseNumbers) {
    std::cout << "----------- Clause Numbers -----------\n";
    for (const auto& num : clauseNumbers) {
        std::cout << num << " ";
    }
    std::cout << "\n-------------------------------------\n";
}
void printRules(const std::map<int, std::string>& rules) {
    std::cout << "----------------- Rules -----------------\n";
    for (const auto& rulePair : rules) {
        std::cout << "Rule Number: " << std::left << std::setw(4) << rulePair.first 
                  << "Conclusion: " << rulePair.second << "\n";
    }
    std::cout << "----------------------------------------\n";
}
void printBoolComparison(const std::vector<bool>& requiredClauseBools, const std::vector<bool>& actualClauseBools) {
    if (requiredClauseBools.size() != actualClauseBools.size()) {
        std::cerr << "Error: Vectors are not the same size.\n";
        return;
    }

    std::cout << "------- Clause Boolean Comparison -------\n"
              << std::left << std::setw(10) << "Index" 
              << std::setw(20) << "Required Clause" 
              << "Actual Clause\n"
              << "----------------------------------------\n";

    for (size_t i = 0; i < requiredClauseBools.size(); ++i) {
        std::cout << std::left << std::setw(10) << i
                  << std::setw(20) << (requiredClauseBools[i] ? "True" : "False")
                  << (actualClauseBools[i] ? "True" : "False") << "\n";
    }
    std::cout << "----------------------------------------\n\n";
}

// main
AttackResult Attacks_BW(){ // Parses knowledge base and extracts the data, keeping it seperate from inference engine
    std::map<int, std::string> rules; // hashing 
    std::vector<ClauseVariable> clauses;
    std::vector<std::string> conclusions;
    parseFile("kbbc.txt", rules, clauses);
    conclusions = getConclusions(rules); // this contains all conclusion variables used for the engine
    AttackResult attack;
    std::stack<std::string> conclusionStack;
    std::set<int> checkedRules;
    // loop that will go through all conclusions (goal) until one is found for the user (type of attack)
    testingOutput("Beginning Backward Chaining procedure...");
    for (auto &conclusion : conclusions){
        conclusionStack.push(conclusion);
        // parse through rules until it finds a rule with the same conclusion as the conclusion we're using as goal
        for (const auto &rulePair : rules) { // iterates through the rules hash map
            if (checkedRules.find(rulePair.first) != checkedRules.end()) {continue;} // Rule already checked
            if (trim(rulePair.second) == trim(conclusion)){ // rule we want that has the conclusion were using for the goal
                testingOutput("Current Goal: " + conclusion + ", located at rule number: " + std::to_string(rulePair.first));
                int clauseNumber = calcClauseNumber(rulePair.first);
                std::vector<std::reference_wrapper<ClauseVariable>> currentRuleClauses;
                bool clauseFound = false;
                for (auto &clause : clauses){ // Find clauses for the current rule
                    // Check if the clause contains the current clauseNumber
                    if (std::find(clause.clauseNumbers.begin(), clause.clauseNumbers.end(), clauseNumber) != clause.clauseNumbers.end()){
                        currentRuleClauses.push_back(clause); // Add reference to the clause
                        clauseFound = true;
                    }
                    else{if (clauseFound) {break;}}
                    if (clauseFound){ // Increment clauseNumber if a matching clause was found
                        clauseNumber++;
                        clauseFound = false; // Reset clauseFound for the next iteration
                    }
                }
                std::reverse(currentRuleClauses.begin(), currentRuleClauses.end());
                // Skip to the next rule as no clauses were found for the current rule
                if (currentRuleClauses.empty()) {continue;} 
                testingOutput("Clauses for rule " + std::to_string(rulePair.first));
                printClauses(currentRuleClauses);
                // iterating through the currentRuleClauses to instantiate and determine clause responses(yes this is the attack/no not this one) based on user input
                for (auto &clauseRef : currentRuleClauses){
                    ClauseVariable &clause = clauseRef.get(); // Get the original clause reference
                    bool clauseIsMet = false;
                    if (!clause.instantiated){
                        getUserInput(clause);
                        clause.instantiated = true;
                    }
                    else {continue;} // if the clause is instaniated, go to next clause
                }
                std::vector<bool> requiredClauseBools = determineRequiredClauseBools(currentRuleClauses); // bools required for conclusion of rule to be used
                std::vector<bool> actualClauseBools = determineRealClauseBools(currentRuleClauses); // bools of users responses (yes=t,no=f)
                // compare the bools to see if what the user gave matches what is needed for the rule to provide the conclusion, if it doesn't keep going through the rules until it's found
                testingOutput("Required bools vs. Users answers for conclusion to be met:");
                printBoolComparison(requiredClauseBools, actualClauseBools);
                if (compareClauseBools(requiredClauseBools, actualClauseBools)){ // attack is identified, return attack
                    attack.attackWasIdentified = true;
                    attack.typeOfAttack = rulePair.second;
                    return attack;
                }
                checkedRules.insert(rulePair.first);
            } // after loop, system is out of clauses for that rule.
        } conclusionStack.pop();
    } 
    return attack;
}
std::vector<std::string> getConclusions(const std::map<int, std::string>& rules) {
    std::vector<std::string> conclusions;
    conclusions.reserve(rules.size()); // Reserving memory for known map size
    for (const auto& rulePair : rules) {
        conclusions.push_back(rulePair.second); // Adding the conclusion from each rule
    }
    return conclusions;
}
std::string trim(std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    return (start < end) ? std::string(start, end) : "";
}
int calcClauseNumber(int ruleNumber){
    return static_cast<int>((static_cast<double>(ruleNumber) / 10.0 - 1) * 4) + 1;
}
void getUserInput(ClauseVariable &tempClause){
    std::string userResponse;
    while (true){
        std::cout << tempClause.meaning << "\n: ";
        getline(std::cin, userResponse);
        for (auto &c : userResponse) {c = std::tolower(c);}
        if (userResponse == "yes"){
            tempClause.response = true;
            break;
        }
        else if (userResponse == "no"){
            tempClause.response = false;
            break;
        }
        else {std::cout << "Invalid response. Please answer with 'yes' or 'no'.\n";}
    }
}
std::vector<bool> determineRequiredClauseBools(std::vector<std::reference_wrapper<ClauseVariable>> &currentRuleClauses){
    std::vector<bool> requiredBools;
    for (auto &clauseRef : currentRuleClauses){
        ClauseVariable &clause = clauseRef.get(); // Get the actual ClauseVariable
        requiredBools.push_back(clause.name[0] != '!');
    }
    return requiredBools;
}
std::vector<bool> determineRealClauseBools(std::vector<std::reference_wrapper<ClauseVariable>> &currentRuleClauses){
    std::vector<bool> realBools;
    for (auto &clauseRef : currentRuleClauses){
        ClauseVariable &clause = clauseRef.get(); // Get the actual ClauseVariable
        realBools.push_back(clause.response.value_or(false));
    }
    return realBools;
}
bool compareClauseBools(std::vector<bool> &requiredClauseBools, std::vector<bool> &actualClauseBools){
    if (requiredClauseBools.size() != actualClauseBools.size()) {return false;}
    return std::equal(requiredClauseBools.begin(), requiredClauseBools.end(), actualClauseBools.begin());
}
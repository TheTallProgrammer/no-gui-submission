#include "Project1-llf46-parser.h"

// Trims whitespace from both ends of a given string
std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(' ');
    // Return original string if no non-space character is found
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    // Extracts a substring from 'first' to 'last', inclusive
    return str.substr(first, (last - first + 1));
}

// Removes all commas from a given string
std::string removeCommas(const std::string &str) {
    std::string result;
    // Reserve space upfront to avoid multiple reallocations
    result.reserve(str.size());
    for (char ch : str) {
        if (ch != ',') {
            result += ch; // Append character if it's not a comma
        }
    }
    return result;
}
// Parses a file to extract rules and clause variables
void parseFile(const std::string &filename, std::map<int, std::string> &rules, std::vector<ClauseVariable> &clauses) {
    std::ifstream file(filename);
    std::string line;
    int lastRuleNumber = -1;  // To keep track of the last rule number

    while (std::getline(file, line)) {
        if (line.find("Rule ") != std::string::npos) {
            std::istringstream iss(line);
            std::string tmp;
            iss >> tmp >> tmp; // Extracting and skipping the "Rule" part, getting the rule number
            try {
                int ruleNumber = std::stoi(tmp);
                rules[ruleNumber] = ""; // Adding a new rule with the extracted number
                lastRuleNumber = ruleNumber; // Updating last rule number
            } catch (const std::invalid_argument &ia) {
                std::cerr << "Invalid argument: " << ia.what() << '\n';
                continue;
            }
        } else if (line.find("Clause: ") != std::string::npos) {
            size_t clausePos = line.find("ClauseNumber: ");
            size_t meaningPos = line.find(", clauseMeaning: ");
            std::string clauseName = removeCommas(trim(line.substr(8, clausePos - 9)));
            std::string clauseNumStr = trim(line.substr(clausePos + 14, meaningPos - (clausePos + 14)));
            std::string clauseMeaning = "";
            if (meaningPos != std::string::npos) {
                clauseMeaning = trim(line.substr(meaningPos + 17));
            }
            try {
                int clauseNumber = std::stoi(clauseNumStr);
                auto foundClause = std::find_if(clauses.begin(), clauses.end(),
                                                [&](const ClauseVariable &c) { return c.name == clauseName; });

                if (foundClause != clauses.end()) {
                    // Clause already exists, so add this clause number to its vector
                    foundClause->clauseNumbers.push_back(clauseNumber);
                } else {
                    // New clause: determine if it's negated and whether it's instantiated
                    bool isNegated = !clauseName.empty() && clauseName[0] == '!';
                    bool instantiated = isNegated;
                    std::optional<bool> response = isNegated ? std::optional<bool>(false) : std::nullopt;

                    clauses.push_back({clauseName, {clauseNumber}, clauseMeaning, instantiated, response});
                }
            } catch (const std::invalid_argument &ia) {
                std::cerr << "Invalid argument: " << ia.what() << '\n';
                continue;
            }
        } else if (line.find("Conclusion: ") != std::string::npos) {
            // Sets the conclusion of the last rule in the vector
            if (!rules.empty()) {
                rules[lastRuleNumber] = line.substr(12); // Sets the conclusion of the last rule
            }
        }
    }
}
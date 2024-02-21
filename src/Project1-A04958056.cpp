#include <iostream>
#include "Project1-A04958056.h"




int main() {
    // starts the backward chaining algo
    AttackResult result = Attacks_BW();
    if(result.attackWasIdentified){
        std::cout << "Attack identified is: " << result.typeOfAttack << std::endl;
    } else {
        std::cout << "No attack found." << std::endl;
    }

    Conclusions finalConclusion = forwardChaining(result);

    // Output the conclusion and description
    std::cout << "Conclusion: " << finalConclusion.conclusion << std::endl;
    std::cout << "Description: " << finalConclusion.description << std::endl;

    return 0;
}

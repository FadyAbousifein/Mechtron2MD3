#include "assignment1.h"
#include <iostream>

float BuyOnePotato(double price, int numOfPotatoes) {
    if (numOfPotatoes > 0) {
        int * pNumOfPotatoes = &numOfPotatoes; 
        --(*pNumOfPotatoes);  
        return price * 1.13;
    } 
    else {
        std::cout << "No more potatoes available\n";
        return 0; 
    }
}

float BuyNumPotatoes(double price, int numOfPotatoes, int numOfPurchases) {
    if (numOfPotatoes > 0) {
        int * pNumOfPotatoes = &numOfPotatoes; 
        (*pNumOfPotatoes) -= numOfPurchases; 
        return price * numOfPurchases * 1.13; 
    } else {
        std::cout << "No more potatoes available\n";
        return 0; 
    }
}

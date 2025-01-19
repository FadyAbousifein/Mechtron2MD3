#include "assignment1.h"
#include <iostream>

double BuyOnePotato(double price, int numOfPotatoes) {
    if (numOfPotatoes > 0) {
        --numOfPotatoes;  
        return price * 1.13;
    } 
    else {
        std::cout << "No more potatoes available\n";
        return 0; 
    }
}

double BuyNumPotatoes(double price, int numOfPotatoes, int numOfPurchases) {
    if (numOfPotatoes > 0) {
        numOfPotatoes -= numOfPurchases; 
        return price * numOfPurchases * 1.13; 
    } else {
        std::cout << "No more potatoes available\n";
        return 0; 
    }
}

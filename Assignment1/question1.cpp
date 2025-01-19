#include "assignment1.h"
#include <stdio.h>

double BuyOnePotato(double price, int numOfPotatoes) {
    if (numOfPotatoes > 0) { 
        return price * 1.13;
    } 
    else {
        printf("No more potatoes available");
        return 0; 
    }
}

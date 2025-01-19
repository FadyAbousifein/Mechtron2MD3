#include <iostream> 
#include "assignment1.h"

// main function will contain and run test cases 
int main() {
    // printf("%lf", BuyOnePotato(353.0, 90)); 
    //printf("%lf", BuyNumPotatoes(30, 0, 5));
    int potatoes_available = 20;
    double potato_price = 30.0;
    std::cout << "$" << BuyOnePotato(potato_price, potatoes_available) << std::endl;
    std::cout << potatoes_available << std::endl << std::endl;
    std::cout << "$" << BuyNumPotatoes(potato_price, potatoes_available, 3) << std::endl;
    std::cout << potatoes_available << std::endl;

    Birthday kid1;
    int * day; 
    int * month; 
    int * year; 
    kid1.randomBirthday(day, month, year); 
    std::cout << day <<  "/" << month << "/" << year << std::endl; 
}

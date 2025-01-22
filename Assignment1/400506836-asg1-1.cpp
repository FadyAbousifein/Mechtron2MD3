#include <iostream> 

// forward declerations
float BuyNumPotatoes(float potatoPrice, int * availablePotatoes, int potatoesPurchased);  
float BuyOnePotato(float potatoPrice, int * availablePotatoes); 

// test cases 
int main() {
    int potatoes_available = 20;
    float potato_price = 30.0;
    std::cout << "$" << BuyOnePotato(potato_price, &potatoes_available)
        << std::endl;
    std::cout << potatoes_available << std::endl << std::endl;
    std::cout << "$" << BuyNumPotatoes(potato_price, &potatoes_available, 3)
        << std::endl;
    std::cout << potatoes_available << std::endl;
    return EXIT_SUCCESS;
}

// returns the price of one potato with tax, and decrements the number of available potatoes by 1
float BuyOnePotato(float potatoPrice, int * availablePotatoes) {
    if (*availablePotatoes > 0) {
        (*availablePotatoes)--; 
        return 1.13 * potatoPrice; 
    } else {
        std::cout << "No Potatoes Available"; 
        return 0; 
    }
}

// returns the price of N potatoes with tax, and decrements the number of available potatoes by N 
float  BuyNumPotatoes(float potatoPrice, int * availablePotatoes, int potatoesPurchased) {
    if (*availablePotatoes > 0) {
        (*availablePotatoes) -= potatoesPurchased; 
        return (potatoPrice * potatoesPurchased) * 1.13; 
    } else {
        std::cout << "No Potatoes Available"; 
        return 0; 
    }
}

#include <cstdlib>
#include <iostream>

class AbsProgression { 
    public:

        int first; 
        int second;

        // default constructor 
        AbsProgression() {
            first = 2; 
            second = 200; 
            printProgression(10);
        }

        // custom constructor 
        AbsProgression(int first, int second) {
            this->first = first;
            this->second = second; 
            printProgression(10);
        }

        // finds the next number in the sequence and shifts the first and second values 
        int nextNum() {
            int nextNum = abs(second - first); 
            first = second; 
            second = nextNum; 
            return nextNum; 
        }

        // prints the sequence progression for the first 10 numbers
        void printProgression(int n) {
            std::cout << first << " " << second << " "; 
            for (int i = 0; i < 8; i++) {
                std::cout << nextNum() << " "; 
            }
        }


}; 

int main() {
    std::cout << "Absolute progression with default constructor:" << std::endl;
    AbsProgression defaultProgression;  // Should output 10 values starting from 2 and 200

    std::cout << std::endl << std::endl << "Absolute progression with custom constructor:" << std::endl;
    AbsProgression customProgression(300, 297);

    return EXIT_SUCCESS; 
}

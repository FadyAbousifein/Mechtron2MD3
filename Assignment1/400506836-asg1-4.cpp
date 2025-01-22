#include <cstdlib>
#include <iostream>

class AbsProgression { 
    public:

        int first; 
        int second;

        AbsProgression() {
            first = 2; 
            second = 200; 

            for (int i = 0; i < 8; i++) {
                
            }
        }

        AbsProgression(int first, int second) {
            this->first = first;
            this->second = second; 
        }

        int nextNum(int first, int second) {
            return abs(second - first); 
        }


}; 

int main() {
    return EXIT_SUCCESS; 
}

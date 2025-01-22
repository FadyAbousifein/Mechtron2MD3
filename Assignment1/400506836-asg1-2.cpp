#include <cstdlib>
#include <iostream> 
#include <ctime>

class Birthday {

    public: 
        int day; 
        int month; 

        // generates random birthday 
        void randomDate() {
            day = (rand() % 31) + 1; // 1 - 31
            month = (rand() % 12) + 1; // 1 - 12 
        }

        // overloads == operator to check whether birthdays are equal
        bool operator == (Birthday * birthday) {
            if (day == birthday->day && month == birthday-> month ) {
                return true;  
            } else return false; 
        }
};

class BirthdayParadox {
    public:
        Birthday *birthdays; // Array for holding birthdays
        const int num_people; // Number of birthdays to generate
        
        // constructor creates n birthdays with a random date
        BirthdayParadox(int n) : num_people(n) {
            birthdays = new Birthday[num_people]; 
            
            for (int i = 0; i < num_people; i++) {
                birthdays[i] = Birthday(); 
                birthdays[i].randomDate(); 
            }
        } 

        ~BirthdayParadox() {
            delete [] birthdays; 
        } 
        
        // checks whether any two of the n birthdays are the same
        int CheckDuplicates(Birthday * birthdays, int n) {
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (birthdays[i] == &(birthdays[j])) {
                        return 1;  
                    } 
                }
            }
            return 0; 
        }
};

int main() {
    srand(time(0)); 

    // runs test cases for n = 5, 10, 15, .. , 100
    for (int n = 5; n <= 100; n += 5) {
        int dupes = 0; 

        // for each n, 100 experiments are ran in which new random birthdays are generated 
        for (int i = 0; i < 100; i++) {
            BirthdayParadox bParadox = BirthdayParadox(n); 
            
            // checks for duplicate birthdays
            if (bParadox.CheckDuplicates(bParadox.birthdays, n)) {
                dupes++; 
            }
        }
        
        std::cout << n << "," << dupes << "," <<  dupes/100. << std::endl; 
    }
    return EXIT_SUCCESS; 
}



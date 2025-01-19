#include "assignment1.h"
#include <iostream>
#include <cstdlib> 

class Birthday {
    public: 
        int month; 
        int day; 
        int year; 

        void randomBirthday(int * day, int * month, int * year) {
            *day = (rand() % 31) + 1; 
            *month = (rand() % 12) + 1; 
            *year = (rand() % 2026);  
        } 

};

class BirthdayParadox {
    public: 
        Birthday * birthdays; 
        const int num_people; 
        BirthdayParadox(int n); 
        ~BirthdayParadox(); 

        int checkDuplicates() {

        }
};


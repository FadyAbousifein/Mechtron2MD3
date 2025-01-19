#pragma once 

float BuyOnePotato(double price, int numOfPotatoes);
float BuyNumPotatoes(double price, int numOfPotatoes, int numOfPurchases);

class Birthday {
    public: 
        int month; 
        int day; 
        int year; 

        void randomBirthday(int * day, int * month, int * year); 
};

class BirthdayParadox {

    public: 
        Birthday * birthdays; 
        const int num_people; 
        BirthdayParadox(int n); 
        ~BirthdayParadox(); 

        int checkDuplicates();
};

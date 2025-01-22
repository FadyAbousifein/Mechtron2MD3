#include <cstdlib>
#include <iostream>

template <typename T1, typename T2> 
class Pair {
    public:
        T1 type1; 
        T2 type2; 

        void print() {
            std::cout << "<" << type1 << ", " << type2 << ">" << std::endl; 
        }

        Pair(T1 type1, T2 type2) {
            this->type1 = type1;
            this->type2 = type2; 
        }
}; 

int main () {
    Pair<int, double> p1(1, 7.3);
    p1.print();
    Pair<std::string, double> p2("hello", 7.7);
    p2.print();
    Pair<float, long> p3(1.2, 777777773);
    p3.print(); 
    return EXIT_SUCCESS;
}

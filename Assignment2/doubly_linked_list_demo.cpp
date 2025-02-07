#include <cstdlib>
#include <iostream>

class Node {
    private: 
        std::string name; 
        int score; 
        Node * next; 
        Node * prev; 
        friend class DLinkedList; 
}; 

class DLinkedList {  // doubly linked list
    public:
        DLinkedList();                 // constructor
        ~DLinkedList();                // destructor
        bool empty() const;            // is list empty?
        
        // part A
        int Size();
        int recursiveSize(Node * current); 
        
        // part B
        void AddScoreInOrder(std::string name, int score); 

        // part C 
        void RemoveScore(int index); 

        // part D 
        bool UpdateScore(std::string name, int score); 

        // part E
        void Print(); 
        void recursivePrint(Node * current); 

        // part F 
        DLinkedList(const DLinkedList& oldList); 
        void addBackWithScore(const std::string& name, int score);

        // part G 
        DLinkedList& operator = (const DLinkedList & oldList); 

        // part H 
        void OrderByName(); 

        // Part I 
        void OrderByScore(); 

        // Part J 
        void Append(DLinkedList& L); 

        // Part K 
        void Reverse(); 

    private:                        
        Node* header;                 
        Node* trailer;
};

DLinkedList::DLinkedList() {  // constructor
   header = new Node; 
   trailer = new Node; 
   header->next = trailer; 
   trailer->prev = header; 
   header->prev = NULL; 
   trailer->next = NULL; 
}

DLinkedList::~DLinkedList() {  // destructor
    Node * temp = header->next; 
    while (temp != trailer) {
        Node * next = temp->next;  
        delete temp; 
        temp = next; 
    }
    delete header; 
    delete trailer; 
}

int DLinkedList::Size() {  // part A
    return recursiveSize(header->next); 
}

int DLinkedList::recursiveSize(Node * current) {  // part A
    if (current == trailer) return 0;
    if (current->score == 0) return recursiveSize(current->next); 
    return 1 + recursiveSize(current->next); 
}

void DLinkedList::AddScoreInOrder(std::string name, int score) {  // part B
    Node * newNode = new Node; 
    newNode->name = name; 
    newNode->score = score; 

    Node * current = header->next; 

    while (current != trailer && current->score > score) {
        current = current->next; 
    }

    newNode->next = current; 
    newNode->prev = current->prev; 
    current->prev->next = newNode; 
    current->prev = newNode; 
}

void DLinkedList::RemoveScore(int index) {  // part C
    if (index < 0 || index >= Size()) return; 

    Node * current = header->next; 

    for (int i = 0; i < index; i++) {
        current = current->next; 
    }
    
    Node * temp = current; 
    current->prev->next = current->next; 
    current->next->prev = current->prev; 

    delete temp; 
}

bool DLinkedList::UpdateScore(std::string name, int score) {  // part D
    Node * current = header->next; 

    while (current != trailer) {
        if (current->name == name) {
            current->score = score; 
            return true;
        }
        current = current->next; 
    }

    return false; 
}

void DLinkedList::Print() {  // part E
    recursivePrint(header->next);
}

void DLinkedList::recursivePrint(Node * current) {  // part E
    if (current == trailer) return; 

    std::cout << "{" << current->name << "," << current->score << "}";

    if (current->next != trailer) std::cout << "->"; 

    recursivePrint(current->next); 
}

DLinkedList::DLinkedList(const DLinkedList& oldList) {  // part F
    header = new Node; 
    trailer = new Node; 
    header->next = trailer; 
    trailer->prev = header; 

    Node * current = oldList.header->next; 
    while (current != oldList.trailer) {
        addBackWithScore(current->name, current->score); 
        current = current->next; 
    }
}

void DLinkedList::addBackWithScore(const std::string& name, int score) {  // part F
    Node* newNode = new Node;
    newNode->name = name;
    newNode->score = score;

    newNode->next = trailer;
    newNode->prev = trailer->prev;
    trailer->prev->next = newNode;
    trailer->prev = newNode;
}

DLinkedList& DLinkedList::operator=(const DLinkedList& oldList) {  // part G
    if (this == &oldList) return *this; 

    while (!empty()) removeFront(); 

    Node * current = oldList.header->next; 

    while (current != oldList.trailer) {
        AddScoreInOrder(current->name, current->score); 
        current = current->next; 
    }
    return *this; 
} 

void DLinkedList::OrderByName() {  // part H
    if (header->next == trailer || header->next->next == trailer) return;

    Node * current = header->next->next; 

    while (current != trailer) {
        Node * temp = current; 
        current = current->next; 

        Node * iteration = header->next; 
        while (iteration != trailer && iteration->name < temp->name) {
            iteration = iteration->next; 
        }

        if (iteration != temp) {
            temp->prev->next = temp->next; 
            if (temp->next != trailer) {
                temp->next->prev = temp->prev; 
            }

            temp->next = iteration; 
            temp->prev = iteration->prev; 
            iteration->prev->next = temp; 
            iteration->prev = temp; 
        }
    }
}

void DLinkedList::OrderByScore() {  // part I
    if (header->next == trailer || header->next->next == trailer) return;

    Node * current = header->next; 
    Node * temp = header;

    while (current != trailer) {
        Node * temp = current->next; 

        current->prev->next = current->next; 
        current->next->prev = current->prev; 

        Node * position = header->next; 

        while (position != trailer && position->score > current->score) position = position->next; 

        current->next = position; 
        current->prev = position->prev; 
        position->prev->next = current; 
        position->prev = current; 

        current = temp; 
    }
}

void DLinkedList::Append(DLinkedList & L) {  // part J
    if (L.empty()) return; 
    
    Node * lastCurrent = trailer->prev; 
    Node * firstL = L.header->next; 
    Node * lastL = L.trailer->prev; 

    lastCurrent->next = firstL; 
    firstL->prev = lastCurrent; 

    lastL->next = trailer; 
    trailer->prev = lastL; 

    L.header->next = L.trailer; 
    L.trailer->prev = L.header; 
}

void DLinkedList::Reverse() {  // part K
    DLinkedList temporary = *this;

    Node* current = header->next;
    while (current != trailer) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    header->next = trailer;
    trailer->prev = header;

    Node* currentNew = temporary.trailer->prev;
    Node* thisPos = header;

    while (currentNew != temporary.header) {
        Node* newNode = new Node;
        newNode->name = currentNew->name;
        newNode->score = currentNew->score;

        newNode->next = trailer;
        newNode->prev = thisPos;
        thisPos->next = newNode;
        trailer->prev = newNode;

        currentNew = currentNew->prev;
        thisPos = newNode;
    }
}

bool DLinkedList::empty() const {  // is list empty?
  return (header->next == trailer);
}


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
        const std::string& front() const;     // get front element
        const std::string& back() const;      // get back element
        void addFront(const std::string& e);  // add to front of list
        void addBack(const std::string& e);   // add to back of list
        void removeFront();            // remove from front
        void removeBack();           
        
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
        void add(Node* v, const std::string& value);  
        void remove(Node* v); 
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

// recursivley count nodes with score
int DLinkedList::Size() {  // part A
    return recursiveSize(header->next); 
}

int DLinkedList::recursiveSize(Node * current) {  // part A
    if (current == trailer) return 0;
    if (current->score == 0) return recursiveSize(current->next); 
    return 1 + recursiveSize(current->next); 
}

// insert node in sorted order by score 
void DLinkedList::AddScoreInOrder(std::string name, int score) {  // part B
    Node * newNode = new Node; 
    newNode->name = name; 
    newNode->score = score; 

    Node * current = header->next; 

    // find insertion point
    while (current != trailer && current->score > score) {
        current = current->next; 
    }

    // insert node before 'current'
    newNode->next = current; 
    newNode->prev = current->prev; 
    current->prev->next = newNode; 
    current->prev = newNode; 
}

// remove node at index 
void DLinkedList::RemoveScore(int index) {  // part C
    if (index < 0 || index >= Size()) return; 

    Node * current = header->next; 

    // find node 
    for (int i = 0; i < index; i++) {
        current = current->next; 
    }
    
    // alter pointers and free memory
    Node * temp = current; 
    current->prev->next = current->next; 
    current->next->prev = current->prev; 

    delete temp; 
}


// update score by name
bool DLinkedList::UpdateScore(std::string name, int score) {  // part D
    Node * current = header->next; 

    // find name and update score
    while (current != trailer) {
        if (current->name == name) {
            current->score = score; 
            return true;
        }
        current = current->next; 
    }

    return false; // name not found 
}

// recursivley print list 
void DLinkedList::Print() {  // part E
    recursivePrint(header->next);
}

void DLinkedList::recursivePrint(Node * current) {  // part E
    if (current == trailer) return; 

    std::cout << "{" << current->name << "," << current->score << "}";

    if (current->next != trailer) std::cout << "->"; 

    recursivePrint(current->next); 
}

// copy constructor 
DLinkedList::DLinkedList(const DLinkedList& oldList) {  // part F
    // initialize new list properties 
    header = new Node; 
    trailer = new Node; 
    header->next = trailer; 
    trailer->prev = header; 

    // copy oldList to new list 
    Node * current = oldList.header->next; 
    while (current != oldList.trailer) {
        addBackWithScore(current->name, current->score); 
        current = current->next; 
    }
}

// same as addBack I just made it also take in a score 
void DLinkedList::addBackWithScore(const std::string& name, int score) {  // part F
    Node* newNode = new Node;
    newNode->name = name;
    newNode->score = score;

    newNode->next = trailer;
    newNode->prev = trailer->prev;
    trailer->prev->next = newNode;
    trailer->prev = newNode;
}

// overload = operator 
DLinkedList& DLinkedList::operator=(const DLinkedList& oldList) {  // part G
    if (this == &oldList) return *this; // same list case

    while (!empty()) removeFront(); 

    Node * current = oldList.header->next; 

    // oldList elements coppied to newList
    while (current != oldList.trailer) {
        AddScoreInOrder(current->name, current->score); 
        current = current->next; 
    }
    return *this; 
} 

// sort by alphabetical order
void DLinkedList::OrderByName() {  // part H
    // one or zero nodes need not to be sorted
    if (header->next == trailer || header->next->next == trailer) return;

    Node * current = header->next->next; 

    // traverse list
    while (current != trailer) {
        Node * temp = current; 
        current = current->next; 

        Node * iteration = header->next; 
        // swap order depending on alphebtical presedence
        while (iteration != trailer && iteration->name < temp->name) {
            iteration = iteration->next; 
        }

        // rearrange pointers 
        if (iteration != temp) {
            temp->prev->next = temp->next; 
            if (temp->next != trailer) {
                temp->next->prev = temp->prev; 
            }

            // set pointers up for next iteration 
            temp->next = iteration; 
            temp->prev = iteration->prev; 
            iteration->prev->next = temp; 
            iteration->prev = temp; 
        }
    }
}


// order by score low - high
void DLinkedList::OrderByScore() {  // part I
    // 0 or 1 node list need not be ordered 
    if (header->next == trailer || header->next->next == trailer) return;

    Node * current = header->next; 
    Node * temp = header;

    // traverse list
    while (current != trailer) {
        // insertion sort 
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

// append list to list 
void DLinkedList::Append(DLinkedList & L) {  // part J
    if (L.empty()) return; // empty list 
    
    // L refers to the passed in list parameters
    Node * lastCurrent = trailer->prev; 
    Node * firstL = L.header->next; 
    Node * lastL = L.trailer->prev; 

    // the list being appended to the larger list will become empty 
    lastCurrent->next = firstL; 
    firstL->prev = lastCurrent; 

    lastL->next = trailer; 
    trailer->prev = lastL; 

    L.header->next = L.trailer; 
    L.trailer->prev = L.header; 
}

// reverse list nodes 
void DLinkedList::Reverse() {  // part K
    DLinkedList temporary = *this; // copy of list 

    // empty current list 
    Node* current = header->next;
    while (current != trailer) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    header->next = trailer;
    trailer->prev = header;

    // copy values to current list 
    Node* currentNew = temporary.trailer->prev;
    Node* position = header;

    // insert the new nodes in proper order into the new list 
    while (currentNew != temporary.header) {
        Node* newNode = new Node;
        newNode->name = currentNew->name;
        newNode->score = currentNew->score;

        newNode->next = trailer;
        newNode->prev = position;
        position->next = newNode;
        trailer->prev = newNode;

        currentNew = currentNew->prev;
        position= newNode;
    }
}

bool DLinkedList::empty() const {  // is list empty?
  return (header->next == trailer);
}



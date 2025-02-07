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

int DLinkedList::Size() {
    return recursiveSize(header->next); 
}

int DLinkedList::recursiveSize(Node * current) {
    if (current == trailer) return 0;
    if (current->score == 0) return recursiveSize(current->next); 
    return 1 + recursiveSize(current->next); 
}

void DLinkedList::AddScoreInOrder(std::string name, int score) {
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

void DLinkedList::RemoveScore(int index) {
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

bool DLinkedList::UpdateScore(std::string name, int score) {
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

void DLinkedList::Print() {
    recursivePrint(header->next);
}

void DLinkedList::recursivePrint(Node * current) {
    if (current == trailer) return; 

    std::cout << "{" << current->name << "," << current->score << "}";

    if (current->next != trailer) std::cout << "->"; 

    recursivePrint(current->next); 
}

DLinkedList::DLinkedList(const DLinkedList& oldList) {
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


void DLinkedList::addBackWithScore(const std::string& name, int score) {
    Node* newNode = new Node;
    newNode->name = name;
    newNode->score = score;

    // Insert newNode at the end (before trailer)
    newNode->next = trailer;
    newNode->prev = trailer->prev;
    trailer->prev->next = newNode;
    trailer->prev = newNode;
}

DLinkedList& DLinkedList::operator=(const DLinkedList& oldList) {
    if (this == &oldList) return *this; 

    while (!empty()) removeFront(); 

    Node * current = oldList.header->next; 

    while (current != oldList.trailer) {
        AddScoreInOrder(current->name, current->score); 
        current = current->next; 
    }
    return *this; 
} 

void DLinkedList::OrderByName() {
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

void DLinkedList::OrderByScore() {
     
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

// the list being appended will be empty after the process 
void DLinkedList::Append(DLinkedList & L) {
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

void DLinkedList::Reverse() {
}




bool DLinkedList::empty() const {  // is list empty?
  return (header->next == trailer);
}

const std::string& DLinkedList::front() const {  // get front element
  return header->next->name;
}

const std::string& DLinkedList::back() const {  // get back element
  return trailer->prev->name;
}

void DLinkedList::add(Node* v, const std::string& value) {
  Node* newNode = new Node;
  newNode->name = value;
  newNode->next = v->next;
  newNode->prev = v;
  v->next->prev = newNode;
  v->next = newNode;
}

void DLinkedList::addFront(const std::string& e) {  // add to front of list
  add(header, e);
}

void DLinkedList::addBack(const std::string& e) {  // add to back of list
  add(trailer->prev, e);
}

void DLinkedList::remove(Node* v) {  // remove node v
  Node* u = v->prev;                 // predecessor
  Node* w = v->next;                 // successor
  u->next = w;                        // unlink v from list
  w->prev = u;
  delete v;
}

void DLinkedList::removeFront() {  // remove from front
  remove(header->next);
}

void DLinkedList::removeBack() {  // remove from back
  remove(trailer->prev);
}

void listReverse(DLinkedList& L) {  // reverse a list
  DLinkedList T;                    // temporary list
  while (!L.empty()) {              // reverse L into T
    std::string s = L.front();
    L.removeFront();
    T.addFront(s);
  }
  while (!T.empty()) {  // copy T back to L
    std::string s = T.front();
    T.removeFront();
    L.addBack(s);
  }
}
int main() {
    DLinkedList dll;

    // Add some scores in order
    dll.AddScoreInOrder("Alice", 90);
    dll.AddScoreInOrder("Bob", 85);
    dll.AddScoreInOrder("Charlie", 95);
    dll.AddScoreInOrder("David", 80);

    std::cout << "Original List (sorted by score): ";
    dll.Print();  // Print the list before reversing

    // Reverse the list (this will reverse the order of scores and names)
    dll.Reverse();
    
    std::cout << "\nReversed List (sorted by score): ";
    dll.Print();  // Print the list after reversing

    return 0;
}

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
        

        void test(const std::string& name, int score); 
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
    traiiler = new Node; 
    header->next = trailer; 
    trailer->prev = header; 
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

void DLinkedList::test(const std::string& name, int score) {
    Node* newNode = new Node;
    newNode->name = name;
    newNode->score = score;

    // Insert newNode at the end (before trailer)
    newNode->next = trailer;
    newNode->prev = trailer->prev;
    trailer->prev->next = newNode;
    trailer->prev = newNode;
}

int main() {
    DLinkedList dll;

    // Adding nodes in order
    dll.AddScoreInOrder("Alice", 90);
    dll.AddScoreInOrder("Bob", 85);
    dll.AddScoreInOrder("Charlie", 95);

    // Print the list recursively
    std::cout << "List in order: ";
    dll.Print();  // Will print the list with names and scores in the recursive format
}


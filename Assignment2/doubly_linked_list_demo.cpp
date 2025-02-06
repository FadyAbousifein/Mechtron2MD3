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
        void addFront(const Elem& e);  // add to front of list
        void addBack(const Elem& e);   // add to back of list
        void removeFront();            // remove from front
        void removeBack();           
        int Size();
        int recursiveSize(Node * current); 

    private:                        // local type definitions
        Node* header;                 // list sentinels
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
    while (temp->next != NULL) {
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
    return 1 + recursiveSize(current->next); 
}


bool DLinkedList::empty() const {  // is list empty?
  return (header->next == trailer);
}

const Elem& DLinkedList::front() const {  // get front element
  return header->next->elem;
}

const Elem& DLinkedList::back() const {  // get back element
  return trailer->prev->elem;
}

void DLinkedList::add(DNode* v, const Elem& value) {
  DNode* newNode = new DNode;
  newNode->elem = value;
  newNode->next = v->next;
  newNode->prev = v;
  v->next->prev = newNode;
  v->next = newNode;
}

void DLinkedList::addFront(const Elem& e) {  // add to front of list
  add(header, e);
}

void DLinkedList::addBack(const Elem& e) {  // add to back of list
  add(trailer->prev, e);
}

void DLinkedList::remove(DNode* v) {  // remove node v
  DNode* u = v->prev;                 // predecessor
  DNode* w = v->next;                 // successor
  u->next = w;                        // unlink v from list
  w->prev = u;
  delete v;
}

void DLinkedList::removeFront() {  // remove from font
  remove(header->next);
}

void DLinkedList::removeBack() {  // remove from back
  remove(trailer->prev);
}

void listReverse(DLinkedList& L) {  // reverse a list
  DLinkedList T;                    // temporary list
  while (!L.empty()) {              // reverse L into T
    Elem s = L.front();
    L.removeFront();
    T.addFront(s);
  }
  while (!T.empty()) {  // copy T back to L
    Elem s = T.front();
    T.removeFront();
    L.addBack(s);
  }
}

int main() {
  DLinkedList dll;
  dll.addBack("one");
  dll.addBack("two");
  dll.addBack("three");
  listReverse(dll);
  std::cout << dll.front() << std::endl;
}

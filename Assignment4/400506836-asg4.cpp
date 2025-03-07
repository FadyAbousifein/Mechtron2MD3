#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <cstdlib>
#include <algorithm>
#include <list> 

using namespace std;

typedef string Elem;

// class will manage a binary tree with a root, left and right children
class LinkedBinaryTree {
protected:
    // node structure 
    struct Node {
        Elem elt; // value in the node
        Node* par; // pointer to parent
        Node* left; // pointer to left child
        Node* right; // pointer to right child
        Node() : elt(""), par(nullptr), left(nullptr), right(nullptr) {} // default constructor
    };

public:
    // position wihtin the tree
    class Position {
    private:
        Node* v; // the node represented by the position 

    public:
        Position(Node* _v = nullptr) : v(_v) {} // position constructor
        Elem& operator*() { return v->elt; } // dereference operator to access an element stored at the position 
        Position left() const { return Position(v->left); } // left child position
        Position right() const { return Position(v->right); } // right child position
        Position parent() const { return Position(v->par); } // parent position
        bool isRoot() const { return v->par == nullptr; } // check if the position is the root 
        bool isExternal() const { return v->left == nullptr && v->right == nullptr; } // check if position is a leaf node

        friend class LinkedBinaryTree; // the linked binary tree class can access the private members of position
    };

    typedef list<Position> PositionList; // define list of positions 

    // public methods for the tree operations 
    LinkedBinaryTree();
    LinkedBinaryTree(const LinkedBinaryTree &other);
    LinkedBinaryTree& operator=(const LinkedBinaryTree &other);
    ~LinkedBinaryTree();

    int size() const; // size of tree
    bool empty() const; // check if tree is empty
    Position root() const; // return root of tree
    PositionList positions() const; // return list of all positions in tree
    void addRoot(); // add a root to the tree
    void expandExternal(const Position &p); // add children 
    Position removeAboveExternal(const Position &p); // remove child and parent 

    void printExpression() const; // print expression in tree
    double evaluateExpression(double a, double b) const; // evaluate expression in tree
    double getScore() const; // getter for score
    void setScore(double s); // setter for score
    bool operator<(const LinkedBinaryTree &other) const; // compares two trees by score

    friend LinkedBinaryTree createExpressionTree(const string& postfix); // creates an expression tree

protected:
    // helper functions for the deep copy and traversal of tree
    void preorder(Node* v, PositionList &pl) const;
    void printExpression(Node* v) const;
    double evaluateExpression(Node* v, double a, double b) const;

private:
    Node* _root; // points to root
    int n; // # of nodes in tree
    double score; // score of tree

    // helper functions for tree management
    Node* clone(Node* v) const; // clones a sub tree rooted at v
    void destroy(Node* v); // destroys a sub tree rooted at v
    int countNodes(Node* v) const; // counts the number of nodes in the subtree rooted at v 
};

// Constructor & Basic Methods
LinkedBinaryTree::LinkedBinaryTree() : _root(nullptr), n(0), score(0.0) {} // default constructor, empty tree and score of 0

int LinkedBinaryTree::size() const { return n; } // returns number of nodes in tree

bool LinkedBinaryTree::empty() const { return size() == 0; } // check if tree is empty

LinkedBinaryTree::Position LinkedBinaryTree::root() const { return Position(_root); } // returns root position of the tree

// adds a root to an empty tree 
void LinkedBinaryTree::addRoot() {
    _root = new Node;
    n = 1;
}

// function expands an external node by creating two child nodes 
void LinkedBinaryTree::expandExternal(const Position &p) {
    // get node from position, create left child, set parent of left child
    // create right child, set parent of right child, increase size of tree 
    Node* v = p.v;
    v->left = new Node;
    v->left->par = v;
    v->right = new Node;
    v->right->par = v;
    n += 2;
}

// function removes the external node (p) and its parent node then returns the position of the sibling of p 
LinkedBinaryTree::Position LinkedBinaryTree::removeAboveExternal(const Position &p) {
    // define node to be removed, parent of external node, and sibling of external node
    Node* w = p.v;
    Node* v = w->par;
    Node* sib = (w == v->left ? v->right : v->left);

    // if parent is the root, make sibling new root, and paret -> null 
    if (v == _root) {
        _root = sib;
        sib->par = nullptr;
    } 
    // parent is not root, get grandparents and if parent is the left child of the grand parent -> sibling = new left child 
    else {
        Node* grand_parent = v->par;
        if (v == grand_parent->left)
            grand_parent->left = sib;
        // otherwise set sibling as new right child and siblings parent = grandparent
        else
            grand_parent->right = sib;
        sib->par = grand_parent;
    }
    // delete external and parent node, decrease size of tree, return position of the sibling
    delete w;
    delete v;
    n -= 2;
    return Position(sib);
}

// function returns a list of all the position in the tree in the preorder traversal order 
LinkedBinaryTree::PositionList LinkedBinaryTree::positions() const {
    PositionList pl;
    preorder(_root, pl);
    return PositionList(pl);
}

// function implements the preorder traversal ov the tree, storing th enodes within PositionList
void LinkedBinaryTree::preorder(Node* v, PositionList &pl) const {
    if (v == nullptr) return; // null node -> return 

    // add current node to list and recur on the left or right child if they exist
    pl.push_back(Position(v));
    if (v->left != nullptr) preorder(v->left, pl);
    if (v->right != nullptr) preorder(v->right, pl);
}

// function prints the expression that is stored within the tree 
void LinkedBinaryTree::printExpression(Node* v) const {
    if (v == nullptr) return; // node null -> return 

    // leaf node -> print it 
    if (v->left == nullptr && v->right == nullptr) {
        cout << v->elt;
    } 
    // handling of "abs" operator 
    else {
        if (v->elt == "abs") {
            cout << "abs(";
            printExpression(v->left);
            cout << ")";
        }
        // handling of the remaining operators 
        else {
            cout << "(";
            printExpression(v->left);
            cout << v->elt;
            printExpression(v->right);
            cout << ")";
        }
    }
}

// function prints the expression the tree represents, begining from the root node
void LinkedBinaryTree::printExpression() const {
    printExpression(_root);
}

// Helper function that recursivley evalutes the expression tree with the given values 'a' and 'b' starting from a given node
double LinkedBinaryTree::evaluateExpression(Node* v, double a, double b) const {
    if (v == nullptr) return 0; // null node -> return 0 
    
    // leaf node -> return a or b depedning on what the node contains 
    if (v->left == nullptr && v->right == nullptr) {
        if (v->elt == "a") return a;
        else if (v->elt == "b") return b;
        else return std::stod(v->elt); // not a leaf -> convert to double and return it 
    } 
    // operator node 
    else {
        // "abs" calculate the absolute value 
        if (v->elt == "abs") {
            double val = evaluateExpression(v->left, a, b);
            return (val < 0) ? -val : val; // ternary conditional representation of |val|
        }
        // evaluate sub trees 
        double left_value = evaluateExpression(v->left, a, b);
        double right_value = evaluateExpression(v->right, a, b);
        
        // perform the appropriate operation at the current node 
        if (v->elt == "+") return left_value + right_value;
        if (v->elt == "-") return left_value - right_value;
        if (v->elt == "*") return left_value * right_value;
        if (v->elt == "/") return left_value / right_value;
        if (v->elt == ">") return (left_value > right_value) ? 1 : -1; // 1 if left > right -1 otherwise 
        return 0;
    }
}

// public method that evalutes the expression tree with the given values for 'a' and 'b' starting from root
double LinkedBinaryTree::evaluateExpression(double a, double b) const {
    return evaluateExpression(_root, a, b);
}

// getter function for the score variable of the tree 
double LinkedBinaryTree::getScore() const { return score; }

// setter function for the score variable of the three 
void LinkedBinaryTree::setScore(double s) { score = s; }

// comparison operator overlaoding to check if the score of a tree is less than another's 
bool LinkedBinaryTree::operator<(const LinkedBinaryTree &other) const {
    return this->score < other.score;
}

// Helper method which recursivley creates a copy of a tree starting from a node 'v'
LinkedBinaryTree::Node* LinkedBinaryTree::clone(Node* v) const {
    if (v == nullptr) return nullptr; // null node -> return null pointer
    
    // create node, copy element from original node, clone the left sub tree
    // set parent pointer to the left child, clone the right subtree 
    // set parent to right child and return the node
    Node* newNode = new Node;
    newNode->elt = v->elt;
    newNode->left = clone(v->left);
    if(newNode->left) newNode->left->par = newNode;
    newNode->right = clone(v->right);
    if(newNode->right) newNode->right->par = newNode;
    return newNode;
}

// helper function which recursivley destroys the nodes of a tree
void LinkedBinaryTree::destroy(Node* v) {
    if (v != nullptr) {
        destroy(v->left);
        destroy(v->right);
        delete v;
    }
}

// helper function which counts the nodes in a subtree which is rooted at 'v'
int LinkedBinaryTree::countNodes(Node* v) const {
    if (v == nullptr) return 0;
    return 1 + countNodes(v->left) + countNodes(v->right);
}

// copy constructor - willcreate a new tree as a copy of an already existing tree 'other'
LinkedBinaryTree::LinkedBinaryTree(const LinkedBinaryTree &other) : score(other.score) {
    _root = clone(other._root);
    n = countNodes(_root);
}

// Assignment operator overloading to copy the content of 'other' into the current object
LinkedBinaryTree& LinkedBinaryTree::operator=(const LinkedBinaryTree &other) {
    if (this != &other) {
        destroy(_root);
        _root = clone(other._root);
        n = countNodes(_root);
        score = other.score;
    }
    return *this;
}

// destructor - will destroy the tree releasing memory 
LinkedBinaryTree::~LinkedBinaryTree() {
    destroy(_root);
}

// Helper Function to Create Expression Tree
LinkedBinaryTree createExpressionTree(const string& postfix) {
    stack<LinkedBinaryTree> subtree_stack; // stack holding subtrees
    istringstream iss(postfix); // stream will read the postfix expression
    string token;

    // process each token that is in the postfix expression 
    while (iss >> token) {
        // check if the current token is a operator or is a function 
        if (token == "abs" || token == "+" || token == "-" || token == "*" || token == "/" || token == ">") {
            // handling of the unary "abs" operator 
            if (token == "abs") {
                // sanity check 
                if (subtree_stack.empty()) { cerr << "Invalid postfix expression: not enough operands for abs" << endl; exit(1); }
                
                // pop the operand 
                LinkedBinaryTree operandTree = subtree_stack.top();
                subtree_stack.pop();
                
                // new tree with the root as the operator, and push to stack  
                LinkedBinaryTree temp_tree;
                temp_tree.addRoot(); 
                *temp_tree.root() = token;
                temp_tree._root->left = operandTree._root;
                if (temp_tree._root->left) temp_tree._root->left->par = temp_tree._root;
                temp_tree.n = operandTree.n + 1;
                operandTree._root = nullptr;
                operandTree.n = 0;
                subtree_stack.push(temp_tree);
            } 
            // handling of binary operators 
            else {
                // sanity check again 
                if (subtree_stack.size() < 2) { cerr << "Invalid postfix expression: not enough operands for " << token << endl; exit(1); }
                
                // pop right and left operands
                LinkedBinaryTree rightTree = subtree_stack.top(); subtree_stack.pop();
                LinkedBinaryTree leftTree = subtree_stack.top(); subtree_stack.pop();
                
                // new tree with root as the operator, and push to stack
                LinkedBinaryTree temp_tree;
                temp_tree.addRoot();
                *temp_tree.root() = token;
                temp_tree._root->left = leftTree._root;
                if (temp_tree._root->left) temp_tree._root->left->par = temp_tree._root;
                temp_tree._root->right = rightTree._root;
                if (temp_tree._root->right) temp_tree._root->right->par = temp_tree._root;
                temp_tree.n = leftTree.n + rightTree.n + 1;
                leftTree._root = nullptr; leftTree.n = 0;
                rightTree._root = nullptr; rightTree.n = 0;
                subtree_stack.push(temp_tree);
            }
        } 
        // handling of operands (either numbers or variables)
        else {
            // new tree with root for operand and push to stack 
            LinkedBinaryTree temp_tree;
            temp_tree.addRoot();
            *temp_tree.root() = token;
            temp_tree.n = 1;
            subtree_stack.push(temp_tree);
        }
    }
    // invalid expression if there is not exactly one tree remaining in the stack
    if (subtree_stack.size() != 1) { cerr << "Invalid postfix expression: remaining trees in stack" << endl; exit(1); }
    return subtree_stack.top(); // return the final expression tree 
}

// main function to parse files from assignment and output the desired result
int main() {
    // read the postfix expressions supplied in the given sample "expressions.txt" file, storing the contents in a vector
    vector<LinkedBinaryTree> trees;
    ifstream expressions_file("expressions.txt");
    string line;

    while (getline(expressions_file, line)) {
        if(line.empty()) continue; // skip blank lines
        trees.push_back(createExpressionTree(line)); // add tree to vector
    }
    expressions_file.close();

    // read the input values a and b from the given sample "input.txt" file, storing the contents in a vector
    vector<vector<double>> inputs;
    ifstream input_file("input.txt");
    while (getline(input_file, line)) {
        if(line.empty()) continue; // skip blank lines again
        vector<double> data_input;
        stringstream ss(line); // split the line into a and b values
        string str;


        while (getline(ss, str, ' ')) {
            if (!str.empty()) data_input.push_back(stod(str)); // convert to double and store
        }
        if(!data_input.empty()) inputs.push_back(data_input); // add input pair a and b to vector
    }
    input_file.close();

    // iterate through and evalute the tree with each input pair
    for (auto& t : trees) {
        double sum = 0;
        for (auto& i : inputs) {
            sum += t.evaluateExpression(i[0], i[1]);
        }
        t.setScore(sum / inputs.size()); // average score
    }

    sort(trees.begin(), trees.end()); // sort by score

    // Output the results in desired format 
    for (auto& t : trees) {
        cout << "Exp ";
        t.printExpression();
        cout << " Score " << t.getScore() << endl;
    }

    return 0;
}


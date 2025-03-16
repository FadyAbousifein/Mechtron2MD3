#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stack>
#include <vector>
#include <fstream>

#include "cartCentering.h"

using namespace std;


// return a double unifomrly sampled in (0,1)
double randDouble(mt19937& rng) {
    return std::uniform_real_distribution<>{0, 1}(rng);
}
// return uniformly sampled 0 or 1
bool randChoice(mt19937& rng) {
    return std::uniform_int_distribution<>{0, 1}(rng);
}
// return a random integer uniformly sampled in (min, max)
int randInt(mt19937& rng, const int& min, const int& max) {
    return std::uniform_int_distribution<>{min, max}(rng);
}

// return true if op is a suported operation, otherwise return false
bool isOp(string op) {
    if (op == "+")
        return true;
    else if (op == "-")
        return true;
    else if (op == "*")
        return true;
    else if (op == "/")
        return true;
    else if (op == ">")
        return true;
    else if (op == "abs")
        return true;
    else
        return false;
}

int arity(string op) {
    if (op == "abs")
        return 1;
    else
        return 2;
}

typedef string Elem;

class LinkedBinaryTree {
    public:
        struct Node {
            Elem elt;
            string name;
            Node* par;
            Node* left;
            Node* right;
            Node() : elt(), par(NULL), name(""), left(NULL), right(NULL) {}
            int depth() {
                if (par == NULL) return 0;
                return par->depth() + 1;
            }
        };

        class Position {
            private:

            public:
                Node* v;
                Position(Node* _v = NULL) : v(_v) {}
                Elem& operator*() { return v->elt; }
                Position left() const { return Position(v->left); }
                void setLeft(Node* n) { v->left = n; }
                Position right() const { return Position(v->right); }
                void setRight(Node* n) { v->right = n; }
                Position parent() const  // get parent
                {
                    return Position(v->par);
                }
                bool isRoot() const  // root of the tree?
                {
                    return v->par == NULL;
                }
                bool isExternal() const  // an external node?
                {
                    return v->left == NULL && v->right == NULL;
                }
                friend class LinkedBinaryTree;  // give tree access
        };
        typedef vector<Position> PositionList;

    public:
        LinkedBinaryTree() : _root(NULL), score(0), steps(0), generation(0) {}

        // copy constructor
        LinkedBinaryTree(const LinkedBinaryTree& t) {
            _root = copyPreOrder(t.root());
            score = t.getScore();
            steps = t.getSteps();
            generation = t.getGeneration();
        }

        // copy assignment operator
        LinkedBinaryTree& operator=(const LinkedBinaryTree& t) {
            if (this != &t) {
                // if tree already contains data, delete it
                if (_root != NULL) {
                    PositionList pl = positions();
                    for (auto& p : pl) delete p.v;
                }
                _root = copyPreOrder(t.root());
                score = t.getScore();
                steps = t.getSteps();
                generation = t.getGeneration();
            }
            return *this;
        }

        // destructor
        ~LinkedBinaryTree() {
            if (_root != NULL) {
                PositionList pl = positions();
                for (auto& p : pl) delete p.v;
            }
        }

        int size() const { return size(_root); }
        int size(Node* root) const;
        int depth() const;
        bool empty() const { return size() == 0; };
        Node* root() const { return _root; }
        PositionList positions() const;
        void addRoot() { _root = new Node; }
        void addRoot(Elem e) {
            _root = new Node;
            _root->elt = e;
        }
        void nameRoot(string name) { _root->name = name; }
        void addLeftChild(const Position& p, const Node* n);
        void addLeftChild(const Position& p);
        void addRightChild(const Position& p, const Node* n);
        void addRightChild(const Position& p);
        void printExpression() { printExpression(_root); }
        void printExpression(Node* v);
        double evaluateExpression(double a, double b) {
            return evaluateExpression(Position(_root), a, b);
        };
        double evaluateExpression(const Position& p, double a, double b);
        long getGeneration() const { return generation; }
        void setGeneration(int g) { generation = g; }
        double getScore() const { return score; }
        void setScore(double s) { score = s; }
        double getSteps() const { return steps; }
        void setSteps(double s) { steps = s; }
        void randomExpressionTree(Node* p, const int& maxDepth, mt19937& rng);
        void randomExpressionTree(const int& maxDepth, mt19937& rng) {
            randomExpressionTree(_root, maxDepth, rng);
        }
        void deleteSubtreeMutator(mt19937& rng);
        void addSubtreeMutator(mt19937& rng, const int maxDepth);

        Node* _root; 

    protected:                                        // local utilities
        void preorder(Node* v, PositionList& pl) const;  // preorder utility
        Node* copyPreOrder(const Node* root);
        double score;     // mean reward over 20 episodes
        double steps;     // mean steps-per-episode over 20 episodes
        long generation;  // which generation was tree "born"
};

// add the tree rooted at node child as this tree's left child
void LinkedBinaryTree::addLeftChild(const Position& p, const Node* child) {
    Node* v = p.v;
    v->left = copyPreOrder(child);  // deep copy child
    v->left->par = v;
}

// add the tree rooted at node child as this tree's right child
void LinkedBinaryTree::addRightChild(const Position& p, const Node* child) {
    Node* v = p.v;
    v->right = copyPreOrder(child);  // deep copy child
    v->right->par = v;
}

void LinkedBinaryTree::addLeftChild(const Position& p) {
    Node* v = p.v;
    v->left = new Node;
    v->left->par = v;
}

void LinkedBinaryTree::addRightChild(const Position& p) {
    Node* v = p.v;
    v->right = new Node;
    v->right->par = v;
}

// return a list of all nodes
LinkedBinaryTree::PositionList LinkedBinaryTree::positions() const {
    PositionList pl;
    preorder(_root, pl);
    return PositionList(pl);
}

void LinkedBinaryTree::preorder(Node* v, PositionList& pl) const {
    pl.push_back(Position(v));
    if (v->left != NULL) preorder(v->left, pl);
    if (v->right != NULL) preorder(v->right, pl);
}

int LinkedBinaryTree::size(Node* v) const {
    int lsize = 0;
    int rsize = 0;
    if (v->left != NULL) lsize = size(v->left);
    if (v->right != NULL) rsize = size(v->right);
    return 1 + lsize + rsize;
}

int LinkedBinaryTree::depth() const {
    PositionList pl = positions();
    int depth = 0;
    for (auto& p : pl) depth = std::max(depth, p.v->depth());
    return depth;
}

LinkedBinaryTree::Node* LinkedBinaryTree::copyPreOrder(const Node* root) {
    if (root == NULL) return NULL;
    Node* nn = new Node;
    nn->elt = root->elt;
    nn->left = copyPreOrder(root->left);
    if (nn->left != NULL) nn->left->par = nn;
    nn->right = copyPreOrder(root->right);
    if (nn->right != NULL) nn->right->par = nn;
    return nn;
}

// Part 1 - Question 1, Function recursivley prints the expression tree 
void LinkedBinaryTree::printExpression(Node* v) {
    if (v == nullptr) return;

    // if the node is a leaf node, simply print the element
    if (v->left == nullptr && v->right == nullptr) {
        cout << v->elt;
        return;
    }

    // Handling for the unary operator
    if (v->elt == "abs") {
        cout << "abs(";
        printExpression(v->left);
        cout << ")";
    } 
    // handling for the binary operators 
    else {
        cout << "(";
        printExpression(v->left);
        cout << " " << v->elt << " ";
        printExpression(v->right);
        cout << ")";
    }
}

double evalOp(string op, double x, double y = 0) {
    double result;
    if (op == "+")
        result = x + y;
    else if (op == "-")
        result = x - y;
    else if (op == "*")
        result = x * y;
    else if (op == "/") {
        result = x / y;
    } else if (op == ">") {
        result = x > y ? 1 : -1;
    } else if (op == "abs") {
        result = abs(x);
    } else
        result = 0;
    return isnan(result) || !isfinite(result) ? 0 : result;
}

double LinkedBinaryTree::evaluateExpression(const Position& p, double a,
        double b) {
    if (!p.isExternal()) {
        auto x = evaluateExpression(p.left(), a, b);
        if (arity(p.v->elt) > 1) {
            auto y = evaluateExpression(p.right(), a, b);
            return evalOp(p.v->elt, x, y);
        } else {
            return evalOp(p.v->elt, x);
        }
    } else {
        if (p.v->elt == "a")
            return a;
        else if (p.v->elt == "b")
            return b;
        else
            return stod(p.v->elt);
    }
}

// Part 1 - Question 3, Fuction randomly delets a subtree from the tree 
void LinkedBinaryTree::deleteSubtreeMutator(mt19937& rng) {
    // handling of empty tree (return) 
    if (empty()) return; 

    // only if there is more than just 1 operator and 2 terminals, collect all the nodes 
    PositionList list_of_nodes = positions();
    if (list_of_nodes.size() <= 3) return;

    // only delete nodes that arent the root, and is an operator instead 
    Node* node_of_interest = nullptr;
    do {
        int random_position = randInt(rng, 0, list_of_nodes.size() - 1);
        node_of_interest = list_of_nodes[random_position].v;
    } while (!isOp(node_of_interest->elt) || node_of_interest == _root);

    // find parent of node, and find if its a left or right child 
    Node* parent_node = node_of_interest->par;
    bool is_left_child = (parent_node->left == node_of_interest);

    // now unlik the node of interest from its parent 
    if (is_left_child) {
        parent_node->left = nullptr;
    } else {
        parent_node->right = nullptr;
    }

    // iteratively delete subtree that is rooted at the node of interest 
    Node* temporary_stack[1000]; 
    int top_of_temp_stack = -1;
    temporary_stack[++top_of_temp_stack] = node_of_interest;

    while (top_of_temp_stack >= 0) {
        Node* currently_selected_node = temporary_stack[top_of_temp_stack--];
        if (currently_selected_node->left) temporary_stack[++top_of_temp_stack] = currently_selected_node->left;
        if (currently_selected_node->right) temporary_stack[++top_of_temp_stack] = currently_selected_node->right;
        delete currently_selected_node; 
    }

    // handling of operators that now lack a parent after the deletion process 
    if (isOp(parent_node->elt)) {
        vector<string> term_vec = {"a", "b"}; 

        if (arity(parent_node->elt) == 2) {
            // binary operator handling: missing child replaced with a random terminal 
            if (is_left_child&& parent_node->right != nullptr) {
                // replace the deleted left child with a random terminal 
                Node* temp_new_term = new Node;
                temp_new_term->elt = term_vec[randInt(rng, 0, term_vec.size() - 1)];
                temp_new_term->par = parent_node;
                parent_node->left = temp_new_term;
            } 
            else if (!is_left_child && parent_node->left != nullptr) {
                // replace the deleted right child with a random terminal 
                Node* temp_new_term = new Node;
                temp_new_term->elt = term_vec[randInt(rng, 0, term_vec.size() - 1)];
                temp_new_term->par = parent_node;
                parent_node->right = temp_new_term;
            } 
            else {
                // replace both deleted children with 2 random terminals 
                Node* temp_left_term= new Node;
                temp_left_term->elt = term_vec[randInt(rng, 0, term_vec.size() - 1)];
                temp_left_term->par = parent_node;
                parent_node->left = temp_left_term;

                Node* temp_right_term = new Node;
                temp_right_term->elt = term_vec[randInt(rng, 0, term_vec.size() - 1)];
                temp_right_term->par = parent_node;
                parent_node->right = temp_right_term;
            }
        } 
        else if (arity(parent_node->elt) == 1 && parent_node->left == nullptr) {
            // unary operator handlin - no chlild, replace with random terminal 
            Node* temp_new_term = new Node;
            temp_new_term->elt = term_vec[randInt(rng, 0, term_vec.size() - 1)];
            temp_new_term->par = parent_node;
            parent_node->left = temp_new_term;
        }
    }
}

// Part 1 - Question 4, function should add a randomly reacted subtree to the tree 
void LinkedBinaryTree::addSubtreeMutator(mt19937& rng, const int maxDepth) {
    if (empty()) return; 

    PositionList list_of_nodes = positions();

    // remove nodes that have no children 
    vector<Node*> no_children_nodes;
    for (auto& position : list_of_nodes) {
        Node* temp_node = position.v;
        if (temp_node->left == nullptr && temp_node->right == nullptr) { 
            no_children_nodes.push_back(temp_node);
        }
    }
    if (no_children_nodes.empty()) return; 

    // define a random node that will be replaced 
    Node* random_node_to_replace = no_children_nodes[randInt(rng, 0, no_children_nodes.size() - 1)];

    static vector<string> operator_vec = {"+", "-", "*", "/", ">", "abs"};
    static vector<string> term_vec = {"a", "b"};

    // Stack to iteratively build the subtree
    struct StackNode {
        Node* node;
        int depth;
    };

    // stack initialized with the root of the new sub tree 
    Node* root_of_subtree = new Node;
    root_of_subtree->elt = randChoice(rng) ? operator_vec[randInt(rng, 0, operator_vec.size() - 1)] : term_vec[randInt(rng, 0, term_vec.size() - 1)]; 
    root_of_subtree->par = random_node_to_replace->par;

    // leaf -> sub tree 
    Node* parent_node = random_node_to_replace->par;
    if (parent_node == nullptr) {
        _root = root_of_subtree; // entire tree replacement root = subtree 
    } 
    else {
        // find if leaf is left or right child, and replace leaf with new sub tree, deleting existing leaf in the process 
        bool leaf_is_left_child = (parent_node->left == random_node_to_replace);

        delete random_node_to_replace; 
        if (leaf_is_left_child) {
            parent_node->left = root_of_subtree;
        } else {
            parent_node->right = root_of_subtree;
        }
    }

    // same logic as in question 3 of part 1 to delete 
    
    StackNode temp_deletion_stack[1000];
    int top_of_deletion_stack = -1;
    int depth_of_leaf = random_node_to_replace->depth(); 

    // Push the root of the new subtree onto the stack
    temp_deletion_stack[++top_of_deletion_stack] = {root_of_subtree, depth_of_leaf + 1};

    while (top_of_deletion_stack >= 0) {
        StackNode current = temp_deletion_stack[top_of_deletion_stack--];
        Node* currentNode = current.node;
        int currentDepth = current.depth;

        // If the current node is an operator and we haven't reached max depth
        if (isOp(currentNode->elt) && currentDepth < maxDepth) {
            // Generate left child
            Node* leftChild = new Node;
            leftChild->elt = randChoice(rng) || currentDepth == maxDepth - 1
                ? term_vec[randInt(rng, 0, term_vec.size() - 1)]
                : operator_vec[randInt(rng, 0, operator_vec.size() - 1)];
            leftChild->par = currentNode;
            currentNode->left = leftChild;

            // Generate right child if the operator is binary
            if (arity(currentNode->elt) > 1) {
                Node* rightChild = new Node;
                rightChild->elt = randChoice(rng) || currentDepth == maxDepth - 1
                    ? term_vec[randInt(rng, 0, term_vec.size() - 1)]
                    : operator_vec[randInt(rng, 0, operator_vec.size() - 1)];
                rightChild->par = currentNode;
                currentNode->right = rightChild;

                // Push right child onto the stack
                temp_deletion_stack[++top_of_deletion_stack] = {rightChild, currentDepth + 1};
            }

            // Push left child onto the stack
            temp_deletion_stack[++top_of_deletion_stack] = {leftChild, currentDepth + 1};
        }
    }
}

bool operator<(const LinkedBinaryTree& x, const LinkedBinaryTree& y) {
    return x.getScore() < y.getScore();
}

LinkedBinaryTree createExpressionTree(string postfix) {
    stack<LinkedBinaryTree> tree_stack;
    stringstream ss(postfix);
    // Split each line into words
    string token;
    while (getline(ss, token, ' ')) {
        LinkedBinaryTree t;
        if (!isOp(token)) {
            t.addRoot(token);
            tree_stack.push(t);
        } else {
            t.addRoot(token);
            if (arity(token) > 1) {
                LinkedBinaryTree r = tree_stack.top();
                tree_stack.pop();
                t.addRightChild(t.root(), r.root());
            }
            LinkedBinaryTree l = tree_stack.top();
            tree_stack.pop();
            t.addLeftChild(t.root(), l.root());
            tree_stack.push(t);
        }
    }
    return tree_stack.top();
}

// Part 1 - Question 2, Function creates and returns a randomly generated expression tree (depth < max_depth)
LinkedBinaryTree createRandExpressionTree(int max_depth, mt19937& rng) {

    static vector<string> ops_vec = {"+", "-", "*", "/", ">", "abs"};
    static vector<string> term_vec = {"a", "b"};

    LinkedBinaryTree tree;

    // Add a random terminal if the base case is zero
    if (max_depth <= 0) {
        tree.addRoot(term_vec[randInt(rng, 0, term_vec.size() - 1)]);
        return tree;
    }
    
    // choose a terminal or operator randomly, only if the depth is 1, whill we choose a terminal directly
    if (max_depth == 1) {
        tree.addRoot(term_vec[randInt(rng, 0, term_vec.size() - 1)]);
    } else {
        string operator_string = ops_vec[randInt(rng, 0, ops_vec.size() - 1)];
        tree.addRoot(operator_string);

        // handling of unary operator 
        if (operator_string == "abs") {
            tree.addLeftChild(tree.root(), createRandExpressionTree(max_depth - 1, rng).root());
        } 
        // hanndling of binary operators 
        else {
            tree.addLeftChild(tree.root(), createRandExpressionTree(max_depth - 1, rng).root());
            tree.addRightChild(tree.root(), createRandExpressionTree(max_depth - 1, rng).root());
        }
    }
    return tree;
}

// evaluate tree t in the cart centering task
void evaluate(mt19937& rng, LinkedBinaryTree& t, const int& num_episode,
        bool animate, bool partially_observable = false) {
    cartCentering env;
    double mean_score = 0.0;
    double mean_steps = 0.0;
    for (int i = 0; i < num_episode; i++) {
        double episode_score = 0.0;
        int episode_steps = 0;
        env.reset(rng);
        while (!env.terminal()) {
            int action;
            if (partially_observable) {
                action = t.evaluateExpression(env.getCartXPos(), 0.0);
            } else {
                action = t.evaluateExpression(env.getCartXPos(), env.getCartXVel());
            }
            episode_score += env.update(action, animate);
            episode_steps++;
        }
        mean_score += episode_score;
        mean_steps += episode_steps;
    }
    t.setScore(mean_score / num_episode);
    t.setSteps(mean_steps / num_episode);
}

// Part 2: Question 1
struct LexLessThan {
    bool operator()(const LinkedBinaryTree& x, const LinkedBinaryTree& y) const {
        double scoreDiff = fabs(x.getScore() - y.getScore());
        if (scoreDiff < 0.01) {
            // Favor simpler trees (fewer nodes) when scores are similar
            return x.size() > y.size();
        } else {
            // Otherwise, compare by score
            return x.getScore() < y.getScore();
        }
    }
};

// Part 3: Question 1
// Perform crossover by swapping subtrees between two parent trees
std::pair<LinkedBinaryTree, LinkedBinaryTree> crossover(mt19937& rng, const LinkedBinaryTree& parent1, const LinkedBinaryTree& parent2) {
    // Create deep copies of the parent trees
    LinkedBinaryTree child1(parent1); // Copy constructor creates a deep copy
    LinkedBinaryTree child2(parent2);

    // Collect all nodes in both child trees
    LinkedBinaryTree::PositionList pl1 = child1.positions();
    LinkedBinaryTree::PositionList pl2 = child2.positions();

    // Exit early if either tree only has a root or only 2 leaf nodes
    if (pl1.size() <= 3 || pl2.size() <= 3) {
        return {child1, child2}; // Return unmodified copies if crossover is not feasible
    }

    // Randomly select a subtree from each child
    int randomIndex1 = randInt(rng, 1, pl1.size() - 1);
    int randomIndex2 = randInt(rng, 1, pl2.size() - 1);

    LinkedBinaryTree::Node* subtree1 = pl1[randomIndex1].v;
    LinkedBinaryTree::Node* subtree2 = pl2[randomIndex2].v;

    // Find the parent nodes of the selected subtrees
    LinkedBinaryTree::Node* parentOfSubtree1 = subtree1->par;
    LinkedBinaryTree::Node* parentOfSubtree2 = subtree2->par;

    // Determine if the subtrees are left or right children of their parents
    bool isSubtree1LeftChild = (parentOfSubtree1 && parentOfSubtree1->left == subtree1);
    bool isSubtree2LeftChild = (parentOfSubtree2 && parentOfSubtree2->left == subtree2);

    // Swap the subtrees
    if (parentOfSubtree1) {
        if (isSubtree1LeftChild) {
            parentOfSubtree1->left = subtree2;
        } else {
            parentOfSubtree1->right = subtree2;
        }
    } else {
        // If subtree1 has no parent, it must be the root of child1
        child1._root = subtree2; // Update the root pointer
    }

    if (parentOfSubtree2) {
        if (isSubtree2LeftChild) {
            parentOfSubtree2->left = subtree1;
        } else {
            parentOfSubtree2->right = subtree1;
        }
    } else {
        // If subtree2 has no parent, it must be the root of child2
        child2._root = subtree1; // Update the root pointer
    }

    // Update the parent pointers of the swapped subtrees
    subtree1->par = parentOfSubtree2;
    subtree2->par = parentOfSubtree1;

    // Return the modified child trees
    return {child1, child2};
}

int main() {
    mt19937 rng(42);
    // Experiment parameters
    const int NUM_TREE = 50;
    const int MAX_DEPTH_INITIAL = 2;
    const int MAX_DEPTH = 20;
    const int NUM_EPISODE = 20;
    const int MAX_GENERATIONS = 100;
    const bool PARTIALLY_OBSERVABLE = false;

    // Write to a csv file
    ofstream file("part3.csv");
    file << "generation,fitness,steps,size,depth" << endl;

    // Create an initial "population" of expression trees
    vector<LinkedBinaryTree> trees;
    for (int i = 0; i < NUM_TREE; i++) {
        LinkedBinaryTree t = createRandExpressionTree(MAX_DEPTH_INITIAL, rng);
        trees.push_back(t);
    }

    // Genetic Algorithm loop
    LinkedBinaryTree best_tree;
    std::cout << "generation,fitness,steps,size,depth" << std::endl;
    for (int g = 1; g <= MAX_GENERATIONS; g++) {

        // Fitness evaluation
        for (auto& t : trees) {
            if (t.getGeneration() < g - 1) continue;  // skip if not new
            evaluate(rng, t, NUM_EPISODE, false, PARTIALLY_OBSERVABLE);
        }

        // sort trees using overloaded "<" op (worst->best)
        std::sort(trees.begin(), trees.end());

        // // sort trees using comparaor class (worst->best)
        /*std::sort(trees.begin(), trees.end(), LexLessThan());*/

        // erase worst 50% of trees (first half of vector)
        trees.erase(trees.begin(), trees.begin() + NUM_TREE / 2);

        // Print stats for best tree
        best_tree = trees[trees.size() - 1];
        std::cout << g << ",";
        std::cout << best_tree.getScore() << ",";
        std::cout << best_tree.getSteps() << ",";
        std::cout << best_tree.size() << ",";
        std::cout << best_tree.depth() << std::endl;

        // Selection and mutation
        /*while (trees.size() < NUM_TREE) {*/
        /*	// Selected random "parent" tree from survivors*/
        /*	LinkedBinaryTree parent = trees[randInt(rng, 0, (NUM_TREE / 2) - 1)];*/
        /**/
        /*	// Create child tree with copy constructor*/
        /*	LinkedBinaryTree child(parent);*/
        /*	child.setGeneration(g);*/
        /**/
        /*	// Mutation*/
        /*	// Delete a randomly selected part of the child's tree*/
        /*	child.deleteSubtreeMutator(rng);*/
        /*	// Add a random subtree to the child*/
        /*	child.addSubtreeMutator(rng, MAX_DEPTH);*/
        /**/
        /*	trees.push_back(child);*/
        /*}*/

        // Selection, crossover, and mutation
        while (trees.size() < NUM_TREE) {
            // Select two random parents
            int index1 = randInt(rng, 0, (NUM_TREE / 2) - 1);
            int index2 = randInt(rng, 0, (NUM_TREE / 2) - 1);

            auto [child1, child2] = crossover(rng, trees[index1], trees[index2]);
            child1.setGeneration(g);
            child2.setGeneration(g);

            // Mutation
            child1.addSubtreeMutator(rng, MAX_DEPTH);
            child2.addSubtreeMutator(rng, MAX_DEPTH);
            child1.deleteSubtreeMutator(rng);
            child2.deleteSubtreeMutator(rng);

            trees.push_back(child1);
            trees.push_back(child2);
        }

        // Write to a csv file
        file << g << ",";
        file << best_tree.getScore() << ",";
        file << best_tree.getSteps() << ",";
        file << best_tree.size() << ",";
        file << best_tree.depth() << endl;
    }

    // // Evaluate best tree with animation
    /*const int num_episode = 3;*/
    /*evaluate(rng, best_tree, num_episode, true, PARTIALLY_OBSERVABLE);*/

    // Close the file
    file.close();

    // Print best tree info
    std::cout << std::endl << "Best tree:" << std::endl;
    best_tree.printExpression();
    std::cout << endl;
    std::cout << "Generation: " << best_tree.getGeneration() << endl;
    std::cout << "Size: " << best_tree.size() << std::endl;
    std::cout << "Depth: " << best_tree.depth() << std::endl;
    std::cout << "Fitness: " << best_tree.getScore() << std::endl << std::endl;

    /*vector<LinkedBinaryTree> trees;*/
    /*mt19937 rng(91);*/
    /*LinkedBinaryTree t;*/
    /*t = createRandExpressionTree(5, rng);*/
    /*t.printExpression();*/
    /*cout << endl;*/
    /*t.deleteSubtreeMutator(rng);*/
    /*t.printExpression();*/
    /*cout << endl;*/
    /*t.addSubtreeMutator(rng, 5);*/
    /*t.printExpression();*/
    /*cout << endl;*/
    /*t.deleteSubtreeMutator(rng);*/
    /*t.printExpression();*/
    /*cout << endl;*/
    /*t.addSubtreeMutator(rng, 5);*/
    /*t.printExpression();*/
    /*cout << endl;*/
    /*trees.push_back(t);*/
    /*LinkedBinaryTree copy(t);*/
    /*copy.printExpression();*/
    /*cout << endl;*/
    /*copy.addSubtreeMutator(rng, 5);*/
    /*copy.printExpression();*/
    /*cout << endl;*/
    /*copy.deleteSubtreeMutator(rng);*/
    /*copy.printExpression();*/
    /*cout << endl;*/
    /*trees.push_back(copy);*/
}

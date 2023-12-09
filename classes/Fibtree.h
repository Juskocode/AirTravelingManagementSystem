//
// Created by freitas on 09-12-2023.
//

#ifndef AIRBUSMANAGEMENTSYSTEM_FIBTREE_H
#define AIRBUSMANAGEMENTSYSTEM_FIBTREE_H

#include <iostream>
#include <cmath>
#include <unordered_map>
#include <vector>

using namespace std;

/**
 * @brief Fibonacci Heap data structure implementation.
 *
 * This class implements a Fibonacci heap, a priority queue data structure
 * that supports efficient insertion, extraction of minimum key, and key decrease operations.
 *
 * @tparam T Type of elements to be stored in the heap.
 */
template <typename T>
class FibTree {
private:
    /**
     * @brief Node structure representing a node in the Fibonacci heap.
     */
    struct Node {
        T key; ///< Key value of the node.
        int degree; ///< Degree of the node.
        Node* parent; ///< Pointer to the parent node.
        Node* child; ///< Pointer to the child node.
        Node* left; ///< Pointer to the left sibling node.
        Node* right; ///< Pointer to the right sibling node.
        bool marked; ///< Flag to mark if the node has lost a child.

        /**
         * @brief Constructor for the Node structure.
         * @param k Key value of the node.
         */
        Node(T k) : key(k), degree(0), parent(nullptr), child(nullptr), left(this), right(this), marked(false) {}
    };

    Node* minNode; ///< Pointer to the minimum node in the Fibonacci heap.
    int numNodes; ///< Number of nodes in the Fibonacci heap.
    std::unordered_map<T, Node*> nodeMap; ///< Map to store nodes using their keys.

    /**
     * @brief Consolidates the trees in the Fibonacci heap to ensure optimal structure.
     */
    void consolidate();

    /**
     * @brief Links two nodes together in the Fibonacci heap.
     * @param node1 First node to be linked.
     * @param node2 Second node to be linked.
     */
    void link(Node* node1, Node* node2);

    /**
     * @brief Cuts a node from its parent in the Fibonacci heap.
     * @param node Node to be cut.
     * @param parent Parent node of the node to be cut.
     */
    void cut(Node* node, Node* parent);

    /**
     * @brief Performs cascading cut operation in the Fibonacci heap.
     * @param node Node on which cascading cut is performed.
     */
    void cascadingCut(Node* node);

    /**
     * @brief Removes a node from the Fibonacci heap.
     * @param node Node to be removed.
     */
    void removeNode(Node* node);

public:
    /**
     * @brief Default constructor for FibTree class.
     */
    FibTree();

    /**
     * @brief Destructor for FibTree class.
     */
    ~FibTree();

    /**
     * @brief Checks if the Fibonacci heap is empty.
     * @return True if the heap is empty, otherwise false.
     */
    bool empty() const;

    /**
     * @brief Returns the number of elements in the Fibonacci heap.
     * @return Number of elements in the heap.
     */
    int size() const;

    /**
     * @brief Inserts a new element into the Fibonacci heap.
     * @param key Key value of the element to be inserted.
     */
    void insert(T key);

    /**
     * @brief Extracts the minimum element from the Fibonacci heap.
     * @return The minimum element of the heap.
     */
    T extractMin();

    /**
     * @brief Decreases the key of an element in the Fibonacci heap.
     * @param oldKey Old key value of the element.
     * @param newKey New key value to be set for the element.
     */
    void decreaseKey(T oldKey, T newKey);

    /**
     * @brief Checks if a key exists in the Fibonacci heap.
     * @param key Key value to be checked.
     * @return True if the key exists, otherwise false.
     */
    bool contains(T key);

    /**
     * @brief Erases an element with the specified key from the Fibonacci heap.
     * @param key Key value of the element to be erased.
     */
    void erase(T key);

    /**
     * @brief Clears the Fibonacci heap, removing all elements.
     */
    void clear();
};

template <typename T>
FibTree<T>::FibTree() : minNode(nullptr), numNodes(0) {}

template <typename T>
FibTree<T>::~FibTree() {
    clear();
}

template <typename T>
bool FibTree<T>::empty() const {
    return numNodes == 0;
}

template <typename T>
int FibTree<T>::size() const {
    return numNodes;
}

template <typename T>
void FibTree<T>::insert(T key) {

    if (nodeMap.find(key) != nodeMap.end()) {
        // Key already exists, handle accordingly
        return;
    }

    Node* newNode = new Node(key);
    if (minNode == nullptr) {
        minNode = newNode;
    } else {
        newNode->left = minNode;
        newNode->right = minNode->right;
        minNode->right = newNode;
        newNode->right->left = newNode;
        if (newNode->key < minNode->key) {
            minNode = newNode;
        }
    }

    nodeMap[key] = newNode;
    ++numNodes;
}


template <typename T>
void FibTree<T>::consolidate() {
    int maxDegree = static_cast<int>(log2(numNodes)) + 1;
    std::vector<Node*> degreeRoots(maxDegree, nullptr);
    Node* current = minNode;
    int numRoots = 0;

    do {
        ++numRoots;
        Node* next = current->right;
        int degree = current->degree;
        while (degreeRoots[degree] != nullptr) {
            Node* other = degreeRoots[degree];
            if (current->key > other->key) {
                std::swap(current, other);
            }
            link(other, current);
            degreeRoots[degree] = nullptr;
            degree++;
        }
        degreeRoots[degree] = current;
        current = next;
    } while (current != minNode);

    minNode = nullptr;
    for (int i = 0; i < maxDegree; ++i) {
        if (degreeRoots[i] != nullptr) {
            if (minNode == nullptr) {
                minNode = degreeRoots[i];
            } else {
                degreeRoots[i]->left->right = degreeRoots[i]->right;
                degreeRoots[i]->right->left = degreeRoots[i]->left;
                degreeRoots[i]->left = minNode;
                degreeRoots[i]->right = minNode->right;
                minNode->right = degreeRoots[i];
                degreeRoots[i]->right->left = degreeRoots[i];
                if (degreeRoots[i]->key < minNode->key) {
                    minNode = degreeRoots[i];
                }
            }
        }
    }
}

template <typename T>
void FibTree<T>::link(Node* node1, Node* node2) {
    node1->left->right = node1->right;
    node1->right->left = node1->left;
    node1->parent = node2;
    if (node2->child == nullptr) {
        node2->child = node1;
        node1->left = node1;
        node1->right = node1;
    } else {
        node1->left = node2->child;
        node1->right = node2->child->right;
        node2->child->right = node1;
        node1->right->left = node1;
    }
    node2->degree++;
    node1->marked = false;
}

template <typename T>
void FibTree<T>::cut(Node* node, Node* parent) {
    if (node == node->right) {
        parent->child = nullptr;
    } else {
        node->left->right = node->right;
        node->right->left = node->left;
        if (node == parent->child) {
            parent->child = node->right;
        }
    }
    parent->degree--;
    node->left = minNode;
    node->right = minNode->right;
    minNode->right = node;
    node->right->left = node;
    node->parent = nullptr;
    node->marked = false;
}

template <typename T>
void FibTree<T>::cascadingCut(Node* node) {
    Node* parent = node->parent;
    if (parent != nullptr) {
        if (!node->marked) {
            node->marked = true;
        } else {
            cut(node, parent);
            cascadingCut(parent);
        }
    }
}

template <typename T>
void FibTree<T>::removeNode(Node* node) {
    decreaseKey(node->key, std::numeric_limits<T>::min());
    extractMin();
}

template <typename T>
T FibTree<T>::extractMin() {
    if (minNode == nullptr) {
        throw std::logic_error("Heap is empty");
    }

    Node* extractedMin = minNode;
    Node* child = extractedMin->child;
    Node* tempChild;

    if (child != nullptr) {
        tempChild = child;
        do {
            tempChild->parent = nullptr;
            tempChild = tempChild->right;
        } while (tempChild != child);
    }

    if (extractedMin->right == extractedMin) {
        minNode = nullptr;
    } else {
        extractedMin->left->right = extractedMin->right;
        extractedMin->right->left = extractedMin->left;
        minNode = extractedMin->right;
        consolidate();
    }

    T minKey = extractedMin->key;
    nodeMap.erase(minKey);
    delete extractedMin;
    --numNodes;

    return minKey;
}

template <typename T>
void FibTree<T>::decreaseKey(T oldKey, T newKey) {
    if (nodeMap.find(oldKey) == nodeMap.end()) {
        // Key does not exist in the heap
        return;
    }

    Node* node = nodeMap[oldKey];
    if (newKey > oldKey) {
        // New key is greater than old key
        return;
    }

    node->key = newKey;
    Node* parent = node->parent;
    if (parent != nullptr && node->key < parent->key) {
        cut(node, parent);
        cascadingCut(parent);
    }

    if (node->key < minNode->key)
        minNode = node;

}

template <typename T>
bool FibTree<T>::contains(T key) {
    return nodeMap.find(key) != nodeMap.end();
}

template <typename T>
void FibTree<T>::erase(T key) {
    if (nodeMap.find(key) != nodeMap.end()) {
        Node* node = nodeMap[key];
        removeNode(node);
    }
}

template <typename T>
void FibTree<T>::clear() {
    while (minNode != nullptr)
        extractMin();
}


#endif //AIRBUSMANAGEMENTSYSTEM_FIBTREE_H

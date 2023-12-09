#ifndef AIRBUSMANAGEMENTSYSTEM_MINHEAP_H
#define AIRBUSMANAGEMENTSYSTEM_MINHEAP_H

#include <vector>
#include <unordered_map>

#define LEFT(i) (2 * (i))
#define RIGHT(i) (2 * (i) + 1)
#define PARENT(i) ((i) / 2)

// Binary min-heap to represent integer keys of type K with values (priorities) of type V
template <class Key, class Value>
class MinHeap {
    struct Node { // An element of the heap: a pair (key, value)
        Key key;
        Value value;
    };

    int heapSize;                  // Number of elements in the heap
    int maxHeapSize;               // Maximum number of elements in heap
    std::vector<Node> heapArray;  // The heap array
    std::unordered_map<Key, int> positions; // Maps a key into its position in the heapArray
    const Key KEY_NOT_FOUND;

    void bubbleUp(int index);      // Time Complexity: O(log N)
    void bubbleDown(int index);    // Time Complexity: O(log N) O(1) in fibTree case
    void swapNodes(int index1, int index2);

public:
    MinHeap(int n, const Key& notFound); // Create a min-heap for a maximum of n pairs (Key, Value) with notFound returned when empty
    int getSize();              // Return number of elements in the heap
    bool contains(const Key& key);  // Checks if heap contains a key
    void insert(const Key& key, const Value& value);      // Insert (key, value) into the heap. Time Complexity: O(log N)
    void decreaseKey(const Key& key, const Value& value); // Decrease value of key. Time Complexity: O(log N)
    Key extractMin(); // Remove and return key with the smallest value. Time Complexity: O(log N)
    /**
     * @brief Checks if the Fibonacci heap is empty.
     * @return True if the heap is empty, otherwise false.
     */
    bool empty() const;
};

template<class Key, class Value>
bool MinHeap<Key, Value>::empty() const {
    return heapSize == 0;
}

// ----------------------------------------------

// Move a value up the tree until it reaches its position
template <class Key, class Value>
void MinHeap<Key, Value>::bubbleUp(int index) {
    while (index > 1 && heapArray[index].value < heapArray[PARENT(index)].value) {
        swapNodes(index, PARENT(index));
        index = PARENT(index);
    }
}

// Move a value down the tree until it reaches its position
template <class Key, class Value>
void MinHeap<Key, Value>::bubbleDown(int index) {
    while (LEFT(index) <= heapSize) {
        int childIndex = LEFT(index);
        if (RIGHT(index) <= heapSize && heapArray[RIGHT(index)].value < heapArray[childIndex].value) {
            childIndex = RIGHT(index);
        }
        if (heapArray[index].value < heapArray[childIndex].value) {
            break;
        }
        swapNodes(index, childIndex);
        index = childIndex;
    }
}

// Swap two nodes within the heap and update their positions
template <class Key, class Value>
void MinHeap<Key, Value>::swapNodes(int index1, int index2) {
    Node tmp = heapArray[index1];
    heapArray[index1] = heapArray[index2];
    heapArray[index2] = tmp;
    positions[heapArray[index1].key] = index1;
    positions[heapArray[index2].key] = index2;
}

// ----------------------------------------------

// Create a min-heap for a maximum of n pairs (Key, Value) with notFound returned when empty
template <class Key, class Value>
MinHeap<Key, Value>::MinHeap(int n, const Key& notFound) : KEY_NOT_FOUND(notFound), heapSize(0), maxHeapSize(n), heapArray(n + 1) {
}

// Return number of elements in the heap
template <class Key, class Value>
int MinHeap<Key, Value>::getSize() {
    return heapSize;
}

// Checks if heap contains a key
template <class Key, class Value>
bool MinHeap<Key, Value>::contains(const Key& key) {
    return positions.find(key) != positions.end();
}

// Insert (key, value) into the heap
template <class Key, class Value>
void MinHeap<Key, Value>::insert(const Key& key, const Value& value) {
    if (heapSize == maxHeapSize) return; // The heap is full, do nothing
    if (contains(key)) return;           // The key already exists, do nothing
    heapArray[++heapSize] = {key, value};
    positions[key] = heapSize;
    bubbleUp(heapSize);
}

// Decrease value of key to the indicated value
template <class Key, class Value>
void MinHeap<Key, Value>::decreaseKey(const Key& key, const Value& value) {
    if (!contains(key)) return; // The key does not exist, do nothing
    int index = positions[key];
    if (value > heapArray[index].value) return; // The value would increase, do nothing
    heapArray[index].value = value;
    bubbleUp(index);
}

// Remove and return key with the smallest value
template <class Key, class Value>
Key MinHeap<Key, Value>::extractMin() {
    if (heapSize == 0) return KEY_NOT_FOUND;
    Key minKey = heapArray[1].key;
    positions.erase(minKey);
    heapArray[1] = heapArray[heapSize--];
    bubbleDown(1);
    return minKey;
}

#endif

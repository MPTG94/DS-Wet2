//
// Created by Mor on 14/06/2020.
//

#ifndef WET2_RANKTREESINGLE_H
#define WET2_RANKTREESINGLE_H

#include <cmath>
#include <algorithm>
#include <iostream>

using std::max;

/**
 * Generic Template Class for a RankTreeSingleNode
 * Made Specifically for use as a balanced AVL tree node meant to store dynamic
 * user data
 */
template<class K>
class RankTreeSingleNode {
private:
    K key;
    int height = 1;
    int rank = 1;
    RankTreeSingleNode<K> *left = nullptr;
    RankTreeSingleNode<K> *right = nullptr;

    static RankTreeSingleNode<K> *Rebalance(RankTreeSingleNode<K> *node);

    int getBalanceFactor();

    static RankTreeSingleNode<K> *StaticLeftRotate(RankTreeSingleNode<K> *node);

    static RankTreeSingleNode<K> *StaticRightRotate(RankTreeSingleNode<K> *node);

public:
    RankTreeSingleNode(K key, RankTreeSingleNode *left = nullptr, RankTreeSingleNode *right = nullptr);

    K getKey();

    int getHeight();

    int getRank();

    static K FindNodeByRank(RankTreeSingleNode<K> *root, int searchRank);

    void PrintTreeInOrderWithRanks();

    int getNodeBalanceFactor();

    static int getBalance(RankTreeSingleNode<K> *node);

    int getLeftChildRank();

    int getRightChildRank();

    int getLeftChildHeight();

    int getRightChildHeight();

    static RankTreeSingleNode<K> *findMin(RankTreeSingleNode<K> *node);

    static RankTreeSingleNode<K> *Insert(RankTreeSingleNode<K> *tempRoot, K nodeKey);

    RankTreeSingleNode<K> *Remove(K nodeKey);

    static RankTreeSingleNode<K> *DeleteNode(RankTreeSingleNode<K> *tempRoot, K nodeKey);

    void updateHeightRecurse();

    void updateNodeHeight();

    void updateNodeRank();

    RankTreeSingleNode<K> *Find(K searchKey);

    RankTreeSingleNode<K> *getNext();

    RankTreeSingleNode<K> *getLeft();

    RankTreeSingleNode<K> *getRight();

    RankTreeSingleNode<K> *findMaxNoRank();

    RankTreeSingleNode<K> *findMax();

    void DeleteTreeData();

    ~RankTreeSingleNode();

    void RankSanity();
};

/**
 * =============================================================================
 * Tree Node Functions Implementation
 * =============================================================================
 */

/**
 * Checks the balance factor of the tree and rebalances it using the AVL tree
 * rotation algorithms, returns the root of the new subtree at the end
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The new root of the subtree after rebalancing
 */
template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::Rebalance(RankTreeSingleNode<K> *node) {
    // Getting the balance factor of the current node
    int balance = node->getBalanceFactor();

    if (balance > 1) {
        int leftSubtreeBalance = node->left->getBalanceFactor();
        if (leftSubtreeBalance > 0) {
            node = StaticLeftRotate(node);
        } else {
            node = StaticLeftRightRotate(node);
        }
    } else if (balance < -1) {
        int rightSubtreeBalance = node->right->getBalanceFactor();
        if (rightSubtreeBalance > 0) {
            node = StaticRightLeftRotate(node);
        } else {
            node = StaticRightRotate(node);
        }
    }
    return node;
}

template<class K>
void RankTreeSingleNode<K>::RankSanity() {
    int leftRank = 0;
    int rightRank = 0;

    if (left) {
        left->RankSanity();
        leftRank = left->rank;
    }
    if (right) {
        right->RankSanity();
        rightRank = right->rank;
    }
    if (rank != 1 + leftRank + rightRank) {
        std::cout << "ERROR IN TREE RANK" << std::endl;
    }
}

/**
 * Calculates the balance factor of the current node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The balance factor of the current node
 */
template<class K>
int RankTreeSingleNode<K>::getBalanceFactor() {
    updateNodeHeight();
    updateNodeRank();
    int leftHeight = 0;
    int rightHeight = 0;
    if (right) {
        rightHeight = right->height;
    }
    if (left) {
        leftHeight = left->height;
    }
    return leftHeight - rightHeight;
}

/**
 * Updates the height of the node based on it's children
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K>
void RankTreeSingleNode<K>::updateNodeHeight() {
    int leftHeight = getLeftChildHeight();
    int rightHeight = getRightChildHeight();
    height = 1 + max(leftHeight, rightHeight);
}

/**
 * Updates the rank of hte node based on it's children
 * @tparam K Class for node key comparisons
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K>
void RankTreeSingleNode<K>::updateNodeRank() {
    int leftRank = getLeftChildRank();
    int rightRank = getRightChildRank();
    rank = 1 + leftRank + rightRank;
}

/**
 * Returns the rank of the left child
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The rank of the left child
 */
template<class K>
int RankTreeSingleNode<K>::getLeftChildRank() {
    if (left) {
        return left->rank;
    }
    return 0;
}

/**
 * Returns the rank of the right child
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The rank of the right child
 */
template<class K>
int RankTreeSingleNode<K>::getRightChildRank() {
    if (right) {
        return right->rank;
    }
    return 0;
}

/**
 * Returns the height of the left child
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The height of the left child
 */
template<class K>
int RankTreeSingleNode<K>::getLeftChildHeight() {
    if (left) {
        return left->height;
    }
    return 0;
}

/**
 * Returns the height of the right child
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The height of the right child
 */
template<class K>
int RankTreeSingleNode<K>::getRightChildHeight() {
    if (right) {
        return right->height;
    }
    return 0;
}

/**
 * Insert a new node to the subtree tree and returns the new root of the subtree
 * after rotations and rebalance
 * @tparam T Pointer to dynamically allocated object of type T
 * @param nodeKey The key of the new node
 * @param nodeData The data object of the new node
 * @param result A pointer to save a reference to the new node in
 * @return The new root of the rebalanced tree after insert
 */
template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::Insert(RankTreeSingleNode<K> *tempRoot, K nodeKey) {
    if (!tempRoot) {
        tempRoot = new RankTreeSingleNode(nodeKey);
        return tempRoot;
    } else if (tempRoot->key > nodeKey) {
        tempRoot->left = Insert(tempRoot->left, nodeKey);
//        tempRoot = Rebalance(tempRoot);
    } else if (tempRoot->key < nodeKey) {
        tempRoot->right = Insert(tempRoot->right, nodeKey);
//        tempRoot = Rebalance(tempRoot);
    }
    tempRoot->updateNodeHeight();
    tempRoot->updateNodeRank();
    int balance = getBalance(tempRoot);
//    int balance = tempRoot->getBalanceFactor();

    if (balance > 1 && nodeKey < tempRoot->left->key) {
        return StaticRightRotate(tempRoot);
    }

    if (balance < -1 && nodeKey > tempRoot->right->key) {
        return StaticLeftRotate(tempRoot);
    }

    if (balance > 1 && nodeKey > tempRoot->left->key) {
        tempRoot->left = StaticLeftRotate(tempRoot->left);
        return StaticRightRotate(tempRoot);
    }

    if (balance < -1 && nodeKey < tempRoot->right->key) {
        tempRoot->right = StaticRightRotate(tempRoot->right);
        return StaticLeftRotate(tempRoot);
    }
    return tempRoot;
}

template<class K>
int RankTreeSingleNode<K>::getBalance(RankTreeSingleNode<K> *tempRoot) {
    if (tempRoot == NULL)
        return 0;
    return tempRoot->getLeftChildHeight() - tempRoot->getRightChildHeight();
}

/**
 * Insert a new node to the subtree tree and returns the new root of the subtree
 * after rotations and rebalance
 * @tparam T Pointer to dynamically allocated object of type T
 * @param nodeKey The key of the new node
 * @param nodeData The data object of the new node
 * @param result A pointer to save a reference to the new node in
 * @return The new root of the rebalanced tree after insert
 */
template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::DeleteNode(RankTreeSingleNode<K> *tempRoot, K nodeKey) {
    if (!tempRoot) {
        return tempRoot;
    }

    if (nodeKey < tempRoot->key) {
        tempRoot->left = DeleteNode(tempRoot->left, nodeKey);
    } else if (nodeKey > tempRoot->key) {
        tempRoot->right = DeleteNode(tempRoot->right, nodeKey);
    } else {
        if (tempRoot->left == nullptr || tempRoot->right == nullptr) {
            RankTreeSingleNode<K> *temp = tempRoot->left ? tempRoot->left : tempRoot->right;

            if (temp == nullptr) {
                temp = tempRoot;
                tempRoot = nullptr;
            } else {
                tempRoot->key = temp->key;
            }
            delete temp;
        } else {
            RankTreeSingleNode<K> *temp = findMin(tempRoot->right);

            tempRoot->key = temp->key;

            tempRoot->right = DeleteNode(tempRoot->right, temp->key);
        }
    }

    if (tempRoot == nullptr) {
        return tempRoot;
    }

    tempRoot->updateNodeHeight();
    tempRoot->updateNodeRank();

    int balance = getBalance(tempRoot);
//    int balance = tempRoot->getBalanceFactor();
    int leftBalance = getBalance(tempRoot->left);
    int rightBalance = getBalance(tempRoot->right);

    if (balance > 1 && leftBalance >= 0) {
        // LL case
        return StaticRightRotate(tempRoot);
    }

    if (balance > 1 && leftBalance < 0) {
        // LR case
        tempRoot->left = StaticLeftRotate(tempRoot->left);
        return StaticRightRotate(tempRoot);
    }

    if (balance < -1 && rightBalance <= 0) {
        // RR case
        return StaticLeftRotate(tempRoot);
    }

    if (balance < -1 && rightBalance > 0) {
        // RL case
        tempRoot->right = StaticRightRotate(tempRoot->right);
        return StaticLeftRotate(tempRoot);
    }
    return tempRoot;
}

template<class K>
RankTreeSingleNode<K>::RankTreeSingleNode(K key, RankTreeSingleNode *left, RankTreeSingleNode *right):
        key(key) {
    height = 1;
    rank = 1;
    this->left = left;
    this->right = right;
};

/**
 * Returns the key of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The key of the tree node
 */
template<class K>
K RankTreeSingleNode<K>::getKey() {
    return key;
}

/**
 * Returns the height of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The height of the tree node
 */
template<class K>
int RankTreeSingleNode<K>::getHeight() {
    return height;
}

/**
 * Completely deletes the tree from the given node downward, including all dynamically
 * allocated data in each node
 * also marks every deleted node as nullptr
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K>
void RankTreeSingleNode<K>::DeleteTreeData() {
    if (left) {
        left->DeleteTreeData();
        delete left;
        left = nullptr;
    }
    if (right) {
        right->DeleteTreeData();
        delete right;
        right = nullptr;
    }
}

/**
 * Returns the balance factor of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The balance factor of the tree node
 */
template<class K>
int RankTreeSingleNode<K>::getNodeBalanceFactor() {
    return getBalanceFactor();
}

/**
 * Deletes the data stored in this node,
 * marks the data as deleted and deletes the node itself
 * (does not delete child nodes)
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K>
RankTreeSingleNode<K>::~RankTreeSingleNode() = default;

/**
 * Searches for a tree node with the input key and returns a pointer to it
 * @tparam T Pointer to dynamically allocated object of type T
 * @param searchKey The key of the node to search for
 * @return A pointer to the tree node if it is found, nullptr otherwise
 */
template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::Find(K searchKey) {
    if (getKey() > searchKey) {
        // The node we are looking for is a left child of this node
        if (left) {
            return left->Find(searchKey);
        } else {
            return nullptr;
        }
    } else if (getKey() < searchKey) {
        // The node we are looking for is a right child of this node
        if (right) {
            return right->Find(searchKey);
        } else {
            return nullptr;
        }
    } else {
        // We found the node we were looking for
        return this;
    }
}

/**
 * Removes a node with input key from the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @param nodeKey The key of the node to remove
 * @return The new root of the subtree after removal of the node
 */
template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::Remove(K nodeKey) {
    // Searching for the node to remove
    if (nodeKey > getKey()) {
        // The delete target is a right child of the current node
        if (right) {
            right->Remove(nodeKey);
        } else {
            return this;
        }
    } else if (nodeKey < getKey()) {
        // The delete target is a left child of the current node
        if (left) {
            left->Remove(nodeKey);
        } else {
            return this;
        }
    } else {
        // Found the node we need to delete
        // Using delete handler to remove the node from the tree
        return this->DeleteNode();
    }
    // Rebalancing tree after removal
    return Rebalance();
}

/**
 * Finds the minimal node from this point downward in the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The minimal node
 */
template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::findMin(RankTreeSingleNode<K> *node) {
    RankTreeSingleNode<K> *current = node;

    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

/**
 * Finds the maximal node from this point downward in the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The maximal node
 */
template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::findMaxNoRank() {
    if (right) {
        return right->findMaxNoRank();
    }
    return this;
}

template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::findMax() {
    if (right) {
        RankTreeSingleNode<K> *temp = right->findMax();
        this->updateNodeRank();
        this->updateNodeHeight();
        return temp;
    }
    this->rank = this->rank - 1;
    this->height = 0;
    return this;
}

template<class K>
int RankTreeSingleNode<K>::getRank() {
    return rank;
}

template<class K>
K RankTreeSingleNode<K>::FindNodeByRank(RankTreeSingleNode<K> *root, int searchRank) {
    if (root->right) {
        if (root->right->rank == searchRank - 1) {
            return root->key;
        } else if (root->right->rank > searchRank - 1) {
            return FindNodeByRank(root->right, searchRank);
        } else {
            return FindNodeByRank(root->left, searchRank - root->right->rank - 1);
        }
    } else if (root->left) {
        if (searchRank == 1) {
            return root->key;
        } else {
            return root->left->key;
        }
    } else {
        return root->key;
    }
}

template<class K>
void RankTreeSingleNode<K>::PrintTreeInOrderWithRanks() {
    if (left) {
        left->PrintTreeInOrderWithRanks();
    }
    std::cout << "Key is: " << key << " Rank is: " << rank << std::endl;

    if (right) {
        right->PrintTreeInOrderWithRanks();
    }
}

template<class K>
void RankTreeSingleNode<K>::updateHeightRecurse() {
    if (left) {
        left->updateHeightRecurse();
    }
    if (right) {
        right->updateHeightRecurse();
    }
    int leftHeight = getLeftChildHeight();
    int rightHeight = getRightChildHeight();
    height = 1 + max(leftHeight, rightHeight);
}

template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::StaticLeftRotate(RankTreeSingleNode<K> *node) {
    RankTreeSingleNode<K> *y = node->right;
    RankTreeSingleNode<K> *T2 = y->left;

    y->left = node;
    node->right = T2;

    node->updateNodeHeight();
    node->updateNodeRank();
    y->updateNodeHeight();
    y->updateNodeRank();
    return y;
}

template<class K>
RankTreeSingleNode<K> *RankTreeSingleNode<K>::StaticRightRotate(RankTreeSingleNode<K> *node) {
    RankTreeSingleNode<K> *x = node->left;
    RankTreeSingleNode<K> *T2 = x->right;

    x->right = node;
    node->left = T2;

    node->updateNodeHeight();
    node->updateNodeRank();

    x->updateNodeHeight();
    x->updateNodeRank();
    return x;
}


/**
 * Generic Template Class for an AVL Tree
 * Stores and organizes AVL tree nodes meant to store dynamic user data
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K>
class RankTreeSingle {
    RankTreeSingleNode<K> *root;

public:
    RankTreeSingle();

    RankTreeSingleNode<K> *GetRoot();

    void MarkRootAsNullptr();

    void MarkRootDataAsNullptr();

    RankTreeSingleNode<K> *Find(K key);

    K FindByRank(int searchRank);

    void Insert(K key);

    void PrintTreeWithRanks();

    void Remove(K key);

    bool IsRootNull();

    ~RankTreeSingle();

    void RankSanityCheck();
};

/**
 * =============================================================================
 * Tree Functions Implementation
 * =============================================================================
 */

/**
 * Creates an empty AVL Tree to store nodes with type T
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K>
RankTreeSingle<K>::RankTreeSingle(): root(nullptr) {};


/**
 * Returns a pointer to the root of the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @return A pointer to the root of the tree
 */
template<class K>
RankTreeSingleNode<K> *RankTreeSingle<K>::GetRoot() {
    if (root) {
        return root;
    }
    return nullptr;
}

/**
 * Searches for a node by key in the tree and returns it if it exists
 * @tparam T Pointer to dynamically allocated object of type T
 * @param key The key of the node to find
 * @return A pointer to the node if it's found in the tree, nullptr otherwise
 */
template<class K>
RankTreeSingleNode<K> *RankTreeSingle<K>::Find(K key) {
    if (!root) {
        return nullptr;
    }

    return root->Find(key);
}

/**
 * Inserts a new node with the given key and data to the tree
 * If the key already exists in the tree, nothing will be done
 * (This also means the dynamic data sent to the function will not
 * be freed)
 * @tparam T Pointer to dynamically allocated object of type T
 * @param key The key of the new node
 * @param data The data of the new node
 */
template<class K>
void RankTreeSingle<K>::Insert(K key) {
    root = RankTreeSingleNode<K>::Insert(root, key);
//    if (!root) {
//        // The tree is empty, inserting the new node as the root
//        root = new RankTreeSingleNode<K>(key);
//    } else {
//        root = root->Insert(key);
//    }
}

/**
 * Removes a node with the given key from the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @param key The key of the node to remove
 */
template<class K>
void RankTreeSingle<K>::Remove(K key) {
    root = RankTreeSingleNode<K>::DeleteNode(root, key);
//    if (root) {
//        root = root->DeleteNode(key);
//    }
}

/**
 * Checks if the tree is empty
 * @tparam T Pointer to dynamically allocated object of type T
 * @return True if the root of the tree is a nullptr, False otherwiese
 */
template<class K>
bool RankTreeSingle<K>::IsRootNull() {
    return (root == nullptr);
}

/**
 * Marks the root of the tree as nullptr
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K>
void RankTreeSingle<K>::MarkRootAsNullptr() {
    root = nullptr;
}

/**
 * Marks the data of the root of the tree as nullptr
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K>
void RankTreeSingle<K>::MarkRootDataAsNullptr() {
    root->removeDataPointer();
}

/**
 * Deletes all nodes stored in the tree, including the dynamic data stored in them
 * and then sets the root of the tree to nullptr
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K>
RankTreeSingle<K>::~RankTreeSingle() {
    if (root) {
        root->DeleteTreeData();
        delete root;
        root = nullptr;
    }
}

template<class K>
K RankTreeSingle<K>::FindByRank(int searchRank) {
    return RankTreeSingleNode<K>::FindNodeByRank(root, searchRank);
}

template<class K>
void RankTreeSingle<K>::PrintTreeWithRanks() {
    if (root) {
        root->PrintTreeInOrderWithRanks();
    }
}

template<class K>
void RankTreeSingle<K>::RankSanityCheck() {
    if (root) {
        root->RankSanity();
    }
}

#endif //WET2_RANKTREESINGLE_H

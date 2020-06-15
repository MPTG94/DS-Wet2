//
// Created by Mor on 14/06/2020.
//

#ifndef WET2_RANKTREESINGLE_H
#define WET2_RANKTREESINGLE_H

#include <iostream>

#include "AVLExceptions.h"

template<class K>
class RankTreeSingle {
private:
    struct Node {
        K key;
        int height;
        int rank;
        Node *left;
        Node *right;
        Node *parent;

        /**
         * Calculates the balance factor of a node
         * @return The balance factor.
         */
        int GetBalanceFactor() {
            int leftHeight = 0, rightHeight = 0;
            if (left) {
                leftHeight = left->height;
            }
            if (right) {
                rightHeight = right->height;
            }
            return leftHeight - rightHeight;
        }

        Node() : key(K(1, 1, 1)), height(1), rank(1) {

        }

        /**
         * Searches for a node with a given rank in the rank tree
         * @param searchRank The rank of the node to find
         * @return The node
         */
        Node *getNodeByRank(int searchRank) {
            if (this->right) {
                if (this->right->rank == searchRank - 1) {
                    return this;
                } else if (this->right->rank > searchRank - 1) {
                    return this->right->getNodeByRank(searchRank);
                } else {
                    return this->left->getNodeByRank(searchRank - this->right->rank - 1);
                }
            } else if (this->left) {
                if (searchRank == 1) {
                    return this;
                } else {
                    return this->left;
                }
            } else {
                return this;
            }
        }
    };
    Node *root;

    void RemoveNode(Node *node);

    void UpdateHeightsGoingUp(Node *node);

    void UpdateRanksGoingUp(Node *node);

    void balance(Node *node);

    void llRotation(Node *node);

    void rrRotation(Node *node);

    void rlRotation(Node *node);

    void lrRotation(Node *node);

    Node *getNode(K key);

    Node *getNodeByRank(int searchRank);

    void ParentPointTo(Node *child, Node *newChild);

    void DeleteTree(Node *node);

public:
    RankTreeSingle();

    virtual ~RankTreeSingle();

    void Insert(K key);

    void Remove(K key);

    K FindByRank(int searchRank);

    K getByKey(K key);

    bool DoesExist(K key);

};

/********************************* Public Functions *******************************/

template<class K>
RankTreeSingle<K>::RankTreeSingle() {
    root = nullptr;
}

template<class K>
RankTreeSingle<K>::~RankTreeSingle() {
    // release allocated memory
    DeleteTree(root);
    root = nullptr;
}

// Time complexity: O(log(n))
template<class K>
void RankTreeSingle<K>::Insert(K key) {
    if (DoesExist(key)) {
        throw KeyAlreadyExist();
    }

    // Create the node
    Node *new_node = new Node();
    new_node->height = 1;
    new_node->rank = 1;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = nullptr;
    new_node->key = key;

    // choose where to add the node and add it.
    if (root == nullptr) {
        // tree is empty
        root = new_node;
        return;
    } else { // tree is not empty.
        Node *current = root;
        Node *parent = nullptr;

        while ((current != nullptr) && (current->key != key)) {
            parent = current;
            if (key < current->key) {
                // left subtree
                current = current->left;
            } else {
                // right subtree
                current = current->right;
            }
        }

        // add to tree
        if (key < parent->key) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
        new_node->parent = parent;

        UpdateHeightsGoingUp(new_node);
        UpdateRanksGoingUp(new_node);
        this->balance(new_node);

        return;
    }
}

// Time complexity: O(log(n))
template<class K>
void RankTreeSingle<K>::Remove(K key) {
    Node *node = getNode(key);
    if (node)
        RemoveNode(node);
}

// Time complexity: log(n)
template<class K>
K RankTreeSingle<K>::getByKey(K key) {
    Node *node = getNode(key);
    if (!node) {
        return K(-1, -1, -1);
    }
    return node->key;
}

// Time complexity: log(n)
template<class K>
bool RankTreeSingle<K>::DoesExist(K key) {
    return (getNode(key) != nullptr);
}

/********************************* Private Functions *******************************/
template<class K>
void RankTreeSingle<K>::RemoveNode(Node *node) {
    if (!(node->left) && !(node->right)) {
        // if leaf
        ParentPointTo(node, nullptr);
        if (node->parent) {
            UpdateHeightsGoingUp(node->parent);
            UpdateRanksGoingUp(node->parent);
            this->balance(node->parent);
        }
        delete node;
        return;
    } else if (!(node->left) && (node->right)) {
        // if only right child
        ParentPointTo(node, node->right);
        node->right->parent = node->parent;

        if (node->parent) {
            UpdateHeightsGoingUp(node->parent);
            UpdateRanksGoingUp(node->parent);
            this->balance(node->parent);
        }
        delete node;
        return;
    } else if ((node->left) && !(node->right)) {
        // if only left child
        ParentPointTo(node, node->left);
        node->left->parent = node->parent;

        if (node->parent) {
            UpdateHeightsGoingUp(node->parent);
            UpdateRanksGoingUp(node->parent);
            this->balance(node->parent);
        }
        delete node;
        return;
    } else {
        // if node has 2 childrens
        // find new root for subtree
        Node *current = node->right;
        while (current->left)
            current = current->left;

        // switch current and node.
        K backupK = current->key;
        current->key = node->key;
        node->key = backupK;

        RemoveNode(current); // recursive call. will happen only once, because now node doesn't have 2 children.
    }
}

/**
 * Updates the height of the current node and it's parents.
 * @tparam K Key object of type T
 * @param node The node to start updating heights from
 */
template<class K>
void RankTreeSingle<K>::UpdateHeightsGoingUp(Node *node) {
    Node *current = node;
    while (current != nullptr) {
        int leftHeight = 0, rightHeight = 0;
        if (current->left) {
            leftHeight = current->left->height;
        }
        if (current->right) {
            rightHeight = current->right->height;
        }
        current->height =
                ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
        current = current->parent;
    }
}

/**
 * Updates the rank of the current node and it's parents.
 * @tparam K Key object of type T
 * @param node The node to start updating ranks from
 */
template<class K>
void RankTreeSingle<K>::UpdateRanksGoingUp(Node *node) {
    Node *current = node;
    while (current != nullptr) {
        int leftRank = 0, rightRank = 0;
        if (current->left) {
            leftRank = current->left->rank;
        }
        if (current->right) {
            rightRank = current->right->rank;
        }
        current->rank = leftRank + rightRank + 1;
        current = current->parent;
    }
}

template<class K>
void RankTreeSingle<K>::balance(Node *node) {
    int balanceFactor = node->GetBalanceFactor();
    if (balanceFactor >= 2) {
        // left Heavy
        if (node->left->GetBalanceFactor() >= 0) {
            llRotation(node);
        } else {
            lrRotation(node);
        }
    } else if (balanceFactor <= -2) {
        // left Heavy
        if (node->right->GetBalanceFactor() > 0) {
            rlRotation(node);
        } else {
            rrRotation(node);
        }
    }

    if (node->parent) {
        balance(node->parent);
    }
}

// node: the node with the |balance|>=2
template<class K>
void RankTreeSingle<K>::llRotation(Node *node) {
    Node *parent = node->parent;
    Node *lChild = node->left;

    // parent-node relation
    ParentPointTo(node, lChild);
    lChild->parent = parent;

    Node *lrChild = node->left->right;
    // node-child relation
    lChild->right = node;
    node->parent = lChild;
    // restore lost chain
    node->left = lrChild;
    if (lrChild) {
        lrChild->parent = node;
    }

    UpdateHeightsGoingUp(node);
    UpdateRanksGoingUp(node);
}

// node: the node with the |balance|>=2
template<class K>
void RankTreeSingle<K>::rrRotation(Node *node) {
    Node *parent = node->parent;
    Node *rChild = node->right;

    // parent-node relation
    ParentPointTo(node, rChild);
    rChild->parent = parent;

    Node *rlChild = node->right->left;
    // node-child relation
    rChild->left = node;
    node->parent = rChild;
    // restore lost chain
    node->right = rlChild;
    if (rlChild) {
        rlChild->parent = node;
    }

    UpdateHeightsGoingUp(node);
    UpdateRanksGoingUp(node);
}

// node: the node with the |balance|>=2
template<class K>
void RankTreeSingle<K>::lrRotation(Node *node) {
    rrRotation(node->left);
    llRotation(node);
}

// node: the node with the |balance|>=2
template<class K>
void RankTreeSingle<K>::rlRotation(Node *node) {
    llRotation(node->right);
    rrRotation(node);
}

template<class K>
typename RankTreeSingle<K>::Node *RankTreeSingle<K>::getNode(K key) {
    Node *current = root;
    while ((current != nullptr) && (current->key != key)) { // while wasn't placed yet
        if (key < current->key) { // left subtree
            current = current->left;
        } else { // right subtree
            current = current->right;
        }
    }
    return current;
}

template<class K>
typename RankTreeSingle<K>::Node *RankTreeSingle<K>::getNodeByRank(int searchRank) {
    return root->getNodeByRank(searchRank);
}

template<class K>
void RankTreeSingle<K>::ParentPointTo(Node *child, Node *newChild) {
    if (child->parent == nullptr)
        root = newChild;
    else {
        if (child->parent->left == child) {
            child->parent->left = newChild;
        } else {
            child->parent->right = newChild;
        }

    }
}

/************************************* Recursions ***********************************/

// destroying the array, post order
template<class K>
void RankTreeSingle<K>::DeleteTree(Node *node) {
    if (node == nullptr)
        return;
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

template<class K>
K RankTreeSingle<K>::FindByRank(int searchRank) {
    Node *temp = getNodeByRank(searchRank);
    if (!temp) {
        throw KeyDoesNotExist();
    }
    return temp->key;
}

#endif //WET2_RANKTREESINGLE_H

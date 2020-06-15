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

        /**
        * Finds The successor node in the subtree
        * @return A pointer to the node
        */
        Node *GetNextNode() {
            Node *current = this->right;
            while (current->left) {
                current = current->left;
            }
            return current;
        }

        Node() : key(K(1, 1, 1)), height(1), rank(1) {

        }

        /**
         * Searches for a node with a given rank in the rank tree
         * @param searchRank The rank of the node to find
         * @return The node
         */
        Node *GetNodeByRank(int searchRank) {
            if (this->right) {
                if (this->right->rank == searchRank - 1) {
                    return this;
                } else if (this->right->rank > searchRank - 1) {
                    return this->right->GetNodeByRank(searchRank);
                } else {
                    return this->left->GetNodeByRank(searchRank - this->right->rank - 1);
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

    void Rebalance(Node *node);

    void LeftLeftRotate(Node *node);

    void RightRightRotate(Node *node);

    void RightLeftRotate(Node *node);

    void LeftRightRotate(Node *node);

    Node *GetNode(K key);

    Node *GetNodeByRank(int searchRank);

    void ParentPointTo(Node *child, Node *newChild);

    void DeleteTree(Node *node);

public:
    RankTreeSingle();

    virtual ~RankTreeSingle();

    void Insert(K key);

    void Remove(K key);

    K FindByRank(int searchRank);

    bool Find(K key);

};

/**
 * Creates a new rank tree
 * @tparam K Key object of type T
 */
template<class K>
RankTreeSingle<K>::RankTreeSingle() {
    root = nullptr;
}

/**
 * Deletes the tree
 * @tparam K Key object of type T
 */
template<class K>
RankTreeSingle<K>::~RankTreeSingle() {
    // call recursive tree deletion
    DeleteTree(root);
    root = nullptr;
}

/**
 * Insert a new node with the input key to the tree
 * @tparam K Key object of type T
 * @param key The key of the new node
 */
template<class K>
void RankTreeSingle<K>::Insert(K key) {
    if (Find(key)) {
        throw KeyAlreadyExist();
    }

    // Creates a new node
    Node *new_node = new Node();
    new_node->height = 1;
    new_node->rank = 1;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = nullptr;
    new_node->key = key;

    if (root == nullptr) {
        // The new node is the first node in the tree
        root = new_node;
        return;
    } else {
        // There are already nodes in the tree.
        Node *current = root;
        Node *parent = nullptr;

        while ((current != nullptr) && (current->key != key)) {
            parent = current;
            if (key < current->key) {
                // Node needs to be inserted on the left.
                current = current->left;
            } else {
                // Node needs to be inserted on the right.
                current = current->right;
            }
        }

        // Adding the node
        if (key < parent->key) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
        new_node->parent = parent;

        UpdateHeightsGoingUp(new_node);
        UpdateRanksGoingUp(new_node);
        this->Rebalance(new_node);

        return;
    }
}

/**
 * Finds a node to remove by key and then deletes it
 * @tparam K Key object of type T
 * @param key The key of the node to remove
 */
template<class K>
void RankTreeSingle<K>::Remove(K key) {
    Node *node = GetNode(key);
    if (node)
        RemoveNode(node);
}

/**
 * Searches for a node with a given key and returns true if found
 * @tparam K Key object of type T
 * @param key THe key of the node to find
 * @return True if found, false otherwise
 */
template<class K>
bool RankTreeSingle<K>::Find(K key) {
    return (GetNode(key) != nullptr);
}

/**
 * Deletes a given node from the tree
 * @tparam K Key object of type T
 * @param node The node to delete
 */
template<class K>
void RankTreeSingle<K>::RemoveNode(Node *node) {
    if (!(node->left) && !(node->right)) {
        // Node to remove is a leaf, removing it from it's parent and rebalancing
        ParentPointTo(node, nullptr);
        if (node->parent) {
            UpdateHeightsGoingUp(node->parent);
            UpdateRanksGoingUp(node->parent);
            this->Rebalance(node->parent);
        }
        delete node;
        return;
    } else if (!(node->left) && (node->right)) {
        // Node only has a right child
        ParentPointTo(node, node->right);
        node->right->parent = node->parent;

        if (node->parent) {
            UpdateHeightsGoingUp(node->parent);
            UpdateRanksGoingUp(node->parent);
            this->Rebalance(node->parent);
        }
        delete node;
        return;
    } else if ((node->left) && !(node->right)) {
        // Node only has a left child
        ParentPointTo(node, node->left);
        node->left->parent = node->parent;

        if (node->parent) {
            UpdateHeightsGoingUp(node->parent);
            UpdateRanksGoingUp(node->parent);
            this->Rebalance(node->parent);
        }
        delete node;
        return;
    } else {
        // Node has 2 children
        // Finding the new root of the subtree
        Node *current = node->GetNextNode();

        // switch current and node.
        K backupK = current->key;
        current->key = node->key;
        node->key = backupK;

        // Calling remove on the old node of the new subtree root.
        RemoveNode(current);
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

/**
 * Rebalances the tree from a given node upward
 * @tparam K Key object of type T
 * @param node The node to check balance for
 */
template<class K>
void RankTreeSingle<K>::Rebalance(Node *node) {
    int balanceFactor = node->GetBalanceFactor();
    if (balanceFactor >= 2) {
        // Node is left heavy
        if (node->left->GetBalanceFactor() >= 0) {
            LeftLeftRotate(node);
        } else {
            LeftRightRotate(node);
        }
    } else if (balanceFactor <= -2) {
        // Node is right heavy
        if (node->right->GetBalanceFactor() > 0) {
            RightLeftRotate(node);
        } else {
            RightRightRotate(node);
        }
    }

    if (node->parent) {
        Rebalance(node->parent);
    }
}

/**
 * Performs left left rotation
 * @tparam K Key object of type T
 * @param node The unbalanced node
 */
template<class K>
void RankTreeSingle<K>::LeftLeftRotate(Node *node) {
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

/**
 * Performs right right rotation
 * @tparam K Key object of type T
 * @param node The unbalanced node
 */
template<class K>
void RankTreeSingle<K>::RightRightRotate(Node *node) {
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

/**
 * Performs left right rotation
 * @tparam K Key object of type T
 * @param node The unbalanced node
 */
template<class K>
void RankTreeSingle<K>::LeftRightRotate(Node *node) {
    RightRightRotate(node->left);
    LeftLeftRotate(node);
}

/**
 * Performs right left rotation
 * @tparam K Key object of type T
 * @param node The unbalanced node
 */
template<class K>
void RankTreeSingle<K>::RightLeftRotate(Node *node) {
    LeftLeftRotate(node->right);
    RightRightRotate(node);
}

/**
 * Finds a node with a given key
 * @tparam K Key object of type T
 * @param key The key of the node to find
 * @return A pointer to the node
 */
template<class K>
typename RankTreeSingle<K>::Node *RankTreeSingle<K>::GetNode(K key) {
    Node *current = root;
    while ((current != nullptr) && (current->key != key)) {
        // Iterate until node is found
        if (key < current->key) {
            // Node is in the left subtree
            current = current->left;
        } else {
            // Node is in the right subtree
            current = current->right;
        }
    }
    return current;
}

/**
 * Returns a node with the given rank
 * @tparam K Key object of type T
 * @param searchRank The rank of the node
 * @return A node with the given rank
 */
template<class K>
typename RankTreeSingle<K>::Node *RankTreeSingle<K>::GetNodeByRank(int searchRank) {
    return root->GetNodeByRank(searchRank);
}

/**
 * Switches the child of a parent node for rotation
 * @tparam K Key object of type T
 * @param child The old child of the node
 * @param newChild The new child of the node
 */
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

/**
 * Deletes the tree in post order traversal
 * @tparam K Key object of type T
 * @param node The node to start deleting from
 */
template<class K>
void RankTreeSingle<K>::DeleteTree(Node *node) {
    if (node == nullptr)
        return;
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

/**
 * Returns the key of a node with a given rank
 * @tparam K Key object of type T
 * @param searchRank The rank of the node
 * @return The key of the node
 */
template<class K>
K RankTreeSingle<K>::FindByRank(int searchRank) {
    Node *temp = GetNodeByRank(searchRank);
    if (!temp) {
        throw KeyDoesNotExist();
    }
    return temp->key;
}

#endif //WET2_RANKTREESINGLE_H

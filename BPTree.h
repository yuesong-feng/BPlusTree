#pragma once
#include <queue>
#include <iostream>
#define order 4
#define LEAF true

template<typename KeyT, typename ValT>
struct Node{
    bool leaf;
    int size;
    KeyT *key;
    Node *parent;
    Node **ptr2node;
    ValT **ptr2val;
    
    Node(bool _leaf = false);
};


template<typename KeyT, typename ValT>
class BPTree{
private:
    Node<KeyT, ValT> *root;
    std::pair<Node<KeyT, ValT>*, int> location(KeyT _key);
    void insertInternal(Node<KeyT, ValT>* _new_node);

public:

    BPTree();

    void insert(KeyT _key, ValT _val);
    void remove(KeyT _key);
    ValT* find(KeyT _key);
    void display();
};

template<typename KeyT, typename ValT>
Node<KeyT, ValT>::Node(bool _leaf) : leaf(_leaf), size(0), key(nullptr), parent(nullptr), ptr2node(nullptr), ptr2val(nullptr) {
    key = new KeyT[order];
    if(leaf){
        ptr2node = new Node*[1];
        ptr2val = new ValT*[order];
    } else{
        ptr2node = new Node*[order + 1];
    }
}

template<typename KeyT, typename ValT>
BPTree<KeyT, ValT>::BPTree() : root(nullptr) {
    
}


template<typename KeyT, typename ValT>
void BPTree<KeyT, ValT>::insert(KeyT _key, ValT _val){
    if(root == nullptr){
        root = new Node<KeyT, ValT>(LEAF);
        root->key[0] = _key;
        root->ptr2val[0] = new ValT(_val);
        root->size++;
        return;
    }
    std::pair<Node<KeyT, ValT>*, int> pair = location(_key);
    Node<KeyT, ValT> *leaf = pair.first;
    int loc = pair.second;
    if(leaf->key[loc] == _key){
        std::cout << "Key " << _key << " with value " << *(leaf->ptr2val[loc]) << " is already in B+ tree, overwrite it with new value " << _val << std::endl;
        *(leaf->ptr2val[loc]) = _val;
    } else if(leaf->size < order){
        for(int i = leaf->size; i > loc + 1; --i){
            leaf->key[i] = leaf->key[i - 1];
            leaf->ptr2val[i] = leaf->ptr2val[i - 1];
        }
        leaf->key[loc + 1] = _key;
        leaf->ptr2val[loc + 1] = new ValT(_val);
        leaf->size++;
    } else{
        KeyT key_tmp[order + 1];
        ValT *val_tmp[order + 1];
        for(int i = 0, offset = 0; i < order + 1; ++i){
            if(i == loc + 1){
                key_tmp[i] = _key;
                val_tmp[i] = new ValT(_val);
                offset++;
            } else{
                key_tmp[i] = leaf->key[i - offset];
                val_tmp[i] = leaf->ptr2val[i - offset];
            }
        }
        Node<KeyT, ValT> *new_leaf = new Node<KeyT, ValT>(LEAF);
        leaf->ptr2node[0] = new_leaf;
        new_leaf->parent = leaf->parent;
        new_leaf->size = leaf->size / 2 + 1;
        leaf->size /= 2;
        for(int i = 0; i < new_leaf->size; ++i){
            new_leaf->key[i] = key_tmp[leaf->size + i];
            new_leaf->ptr2val[i] = val_tmp[leaf->size + i];
        }
        for(int i = order - 1; i >= leaf->size; --i){
            // leaf->key[i] = 0;
            leaf->ptr2val[i] = nullptr;
        }
        if(leaf == root){
            Node<KeyT, ValT> *new_root = new Node<KeyT, ValT>();
            new_root->key[0] = new_leaf->key[0];
            new_root->ptr2node[0] = leaf;
            new_root->ptr2node[1] = new_leaf;
            new_root->size = 1;
            root = new_root;
            leaf->parent = root;
            new_leaf->parent = root;
        } else{
            insertInternal(new_leaf);
        }
    }
}

template<typename KeyT, typename ValT>
void BPTree<KeyT, ValT>::insertInternal(Node<KeyT, ValT>* _new_node){
    Node<KeyT, ValT> *node = _new_node->parent;
    int loc = 0;
    while(_new_node->key[0] >= node->key[loc]){
        loc++;
        if(loc == node->size) break;
    }
    loc--;
    if(node->size < order){
        for(int i = node->size; i >= 0; --i){
            if(i == loc + 1){
                node->key[i] = _new_node->key[0];
                node->ptr2node[i + 1] = _new_node;
                node->size++;
                break;
            }else{
                node->key[i] = node->key[i - 1];
                node->ptr2node[i + 1] = node->ptr2node[i];
            }
        }
    } else{
        KeyT key_tmp[order + 1];
        Node<KeyT, ValT> *node_tmp[order + 2];
        node_tmp[0] = node->ptr2node[0];
        for(int i = 0, offset = 0; i < order + 1; ++i){
            if(i == loc + 1){
                key_tmp[i] = _new_node->key[0];
                node_tmp[i + 1] = _new_node;
                offset++;
            } else{
                key_tmp[i] = node->key[i - offset];
                node_tmp[i + 1] = node->ptr2node[i + 1 - offset];
            }
        }
        Node<KeyT, ValT> *new_node = new Node<KeyT, ValT>();
        new_node->parent = node->parent;
        new_node->size = node->size / 2 + 1;
        node->size /= 2;
        for(int i = 0; i < new_node->size; ++i){
            new_node->key[i] = key_tmp[node->size + i];
            new_node->ptr2node[i] = node_tmp[node->size + i];
            new_node->ptr2node[i]->parent = new_node;
        }
        new_node->ptr2node[new_node->size] = node_tmp[node->size + new_node->size];
        new_node->ptr2node[new_node->size]->parent = new_node;
        for(int i = order - 1; i >= node->size; --i){
            // leaf->key[i] = 0;
            node->ptr2node[i + 1] = nullptr;
        }
        node->ptr2node[node->size] = nullptr;
        if(node == root){
            Node<KeyT, ValT> *new_root = new Node<KeyT, ValT>();
            new_root->key[0] = new_node->key[0];
            new_root->ptr2node[0] = node;
            new_root->ptr2node[1] = new_node;
            new_root->size = 1;
            root = new_root;
            node->parent = root;
            new_node->parent = root;
        } else{
            insertInternal(new_node);
        }
    }
}

template<typename KeyT, typename ValT>
std::pair<Node<KeyT, ValT>*, int> BPTree<KeyT, ValT>::location(KeyT _key){
    if(root == nullptr){
        return std::make_pair(nullptr, 0);
    }
    Node<KeyT, ValT> *node = root;
    while(true){
        int loc = 0;
        while(_key >= node->key[loc]){
            loc++;
            if(loc == node->size) break;
        }
        if(node->leaf == false){
            node = node->ptr2node[loc];
        } else{
            return std::make_pair(node, loc - 1);
        }
    }
}

template<typename KeyT, typename ValT>
ValT* BPTree<KeyT, ValT>::find(KeyT _key){
    std::pair<Node<KeyT, ValT>*, int> pair = location(_key);
    Node<KeyT, ValT> *node = pair.first;
    int loc = pair.second;
    if(node == nullptr){
        return nullptr;
    } else if(node->key[loc] != _key){
        return nullptr;
    } else{
        return node->ptr2val[loc];
    }
}

template<typename KeyT, typename ValT>
void BPTree<KeyT, ValT>::display(){
    if(root == nullptr){
        printf("B+ tree is empty!\n");
        return;
    }
    std::queue<Node<KeyT, ValT>*> queue;
    queue.push(root);
    while(!queue.empty()){
        int size = queue.size();
        while(size--){
            Node<KeyT, ValT> *node = queue.front();
            queue.pop();
            for(int i = 0; i < node->size; ++i){
                if(node->leaf){
                    std::cout << *(node->ptr2val[i]) << " ";
                } else{
                    std::cout << node->key[i] << " ";
                    queue.push(node->ptr2node[i]);
                    if(i + 1 == node->size && node->ptr2node[i + 1] != nullptr)
                        queue.push(node->ptr2node[i + 1]);
                }
            }
            std::cout << "| ";
        }
        std::cout << std::endl;
    }
}

template<typename KeyT, typename ValT>
void BPTree<KeyT, ValT>::remove(KeyT _key){
    std::pair<Node<KeyT, ValT>*, int> pair = location(_key);
    Node<KeyT, ValT> *node = pair.first;
    int loc = pair.second;
    if(node == nullptr){
        std::cout << "Remove error, B+ tree is empty" << std::endl;
        return;
    } else if(node->key[loc] != _key){
        std::cout << "Remove error, key doesn't exist in B+ tree" << std::endl;
        return;
    }

}
#pragma once
#define order 4
#define LEAF true
#include <iostream>
#include <vector>
#include <queue>
#include <assert.h>

template<typename KeyT, typename ValT>
class Node{
public:
    bool leaf;
    Node *parent;
    std::vector<KeyT> key;
    std::vector<Node*> ptr2node;
    std::vector<ValT*> ptr2val;
    Node(bool _leaf = false);
};

template<typename KeyT, typename ValT>
class BPTree{
private:
    Node<KeyT, ValT> *root;
    void makeIndex(Node<KeyT, ValT>* _new_node, KeyT _index);
    inline int keyIndex(Node<KeyT, ValT> *_node, KeyT _key);
    inline std::pair<Node<KeyT, ValT>*, int> keyIndexInLeaf(KeyT _key);
    Node<KeyT, ValT>* splitLeaf(Node<KeyT, ValT>* _leaf);
    std::pair<Node<KeyT, ValT>*, KeyT> splitNode(Node<KeyT, ValT>* _node);
public:
    BPTree();
    void insert(KeyT _key, ValT _val);
    void display();
};

template<typename KeyT, typename ValT>
Node<KeyT, ValT>::Node(bool _leaf) : leaf(_leaf), parent(nullptr) {}

template<typename KeyT, typename ValT>
BPTree<KeyT, ValT>::BPTree() : root(nullptr) {}

template<typename KeyT, typename ValT>
Node<KeyT, ValT>* BPTree<KeyT, ValT>::splitLeaf(Node<KeyT, ValT>* _leaf){
    Node<KeyT, ValT> *new_leaf = new Node<KeyT, ValT>(LEAF);
    new_leaf->ptr2node.push_back(_leaf->ptr2node[0]);
    _leaf->ptr2node.clear();
    _leaf->ptr2node.push_back(new_leaf);
    new_leaf->parent = _leaf->parent;
    int mid = (_leaf->key.size() + 1) / 2 - 1;
    new_leaf->key.assign(_leaf->key.begin() + mid, _leaf->key.end());
    new_leaf->ptr2val.assign(_leaf->ptr2val.begin() + mid, _leaf->ptr2val.end());
    _leaf->key.erase(_leaf->key.begin() + mid, _leaf->key.end());
    _leaf->ptr2val.erase(_leaf->ptr2val.begin() + mid, _leaf->ptr2val.end());
    return new_leaf;
}

template<typename KeyT, typename ValT>
std::pair<Node<KeyT, ValT>*, KeyT> BPTree<KeyT, ValT>::splitNode(Node<KeyT, ValT>* _node){
    Node<KeyT, ValT> *new_node = new Node<KeyT, ValT>();
    new_node->parent = _node->parent;
    int mid = (_node->key.size() + 1) / 2 - 1;
    KeyT push_key = _node->key[mid];
    new_node->key.assign(_node->key.begin() + mid + 1, _node->key.end());
    new_node->ptr2node.assign(_node->ptr2node.begin() + mid + 1, _node->ptr2node.end());
    _node->key.erase(_node->key.begin() + mid, _node->key.end());
    _node->ptr2node.erase(_node->ptr2node.begin() + mid, _node->ptr2node.end());
    for(Node<KeyT, ValT>* each : new_node->ptr2node)
        each->parent = new_node;
    return std::make_pair(new_node, push_key);
}

template<typename KeyT, typename ValT>
void BPTree<KeyT, ValT>::insert(KeyT _key, ValT _val){  
    if(root == nullptr){
        root = new Node<KeyT, ValT>(LEAF);
        root->key.push_back(_key);
        root->ptr2val.emplace_back(new ValT(_val));
        root->ptr2node.push_back(nullptr);
        return;
    }
    std::pair<Node<KeyT, ValT>*, int> pair = keyIndexInLeaf(_key);
    Node<KeyT, ValT> *leaf = pair.first;
    int loc = pair.second;
    if(loc != -1 && leaf->key[loc] == _key){
        std::cout << "Key " << _key << " with value " << *(leaf->ptr2val[loc]) << " is already in B+ tree, overwrite it with new val " << _val << std::endl;
        *(leaf->ptr2val[loc]) = _val;
        return;
    }
    leaf->key.insert(leaf->key.begin() + loc + 1, _key);
    leaf->ptr2val.insert(leaf->ptr2val.begin() + loc + 1, new ValT(_val));
    if(leaf->key.size() > order){
        Node<KeyT, ValT> *new_leaf = splitLeaf(leaf);
        if(leaf == root){
            Node<KeyT, ValT> *new_root = new Node<KeyT, ValT>();
            new_root->key.push_back(new_leaf->key[0]);
            new_root->ptr2node.push_back(leaf);
            new_root->ptr2node.push_back(new_leaf);
            root = new_root;
            leaf->parent = root;
            new_leaf->parent = root;
        } else{
            makeIndex(new_leaf, new_leaf->key[0]);
        }
    }
}

template<typename KeyT, typename ValT>
void BPTree<KeyT, ValT>::makeIndex(Node<KeyT, ValT>* _new_node, KeyT _index){
    Node<KeyT, ValT> *node = _new_node->parent;
    int loc = keyIndex(node, _index);
    node->key.insert(node->key.begin() + loc + 1, _index);
    node->ptr2node.insert(node->ptr2node.begin() + loc + 2, _new_node);
    if(node->key.size() > order){
        std::pair<Node<KeyT, ValT>*, KeyT> pair = splitNode(node);
        Node<KeyT, ValT> *new_node = pair.first;
        KeyT push_key = pair.second;
        if(node == root){
            Node<KeyT, ValT> *new_root = new Node<KeyT, ValT>();
            new_root->key.push_back(push_key);
            new_root->ptr2node.push_back(node);
            new_root->ptr2node.push_back(new_node);
            root = new_root;
            node->parent = root;
            new_node->parent = root;
        } else{
            makeIndex(new_node, push_key);
        }
    }
}


template<typename KeyT, typename ValT>
inline int BPTree<KeyT, ValT>::keyIndex(Node<KeyT, ValT> *_node, KeyT _key){
    int loc = -1;
    int size = _node->key.size();
    while(_node->key[loc + 1] <= _key){
        loc++;
        if(loc == size - 1) break;
    }
    return loc;
}

template<typename KeyT, typename ValT>
inline std::pair<Node<KeyT, ValT>*, int> BPTree<KeyT, ValT>::keyIndexInLeaf(KeyT _key){
    if(root == nullptr){
        return std::make_pair(nullptr, static_cast<ValT>(0));
    }
    Node<KeyT, ValT> *node = root;
    while(true){
        int loc = keyIndex(node, _key);
        if(node->leaf){
            return std::make_pair(node, loc);
        } else{
            node = node->ptr2node[loc + 1];
        }
    }
}


template<typename KeyT, typename ValT>
void BPTree<KeyT, ValT>::display(){
    if(root == nullptr){
        std::cout << "B+ tree is empty!" << std::endl;
        return;
    }
    std::queue<Node<KeyT, ValT>*> q;
    q.push(root);
    while(!q.empty()){
        int q_size = q.size();
        while(q_size--){
            Node<KeyT, ValT> *node = q.front();
            q.pop();
            int key_size = node->key.size();
            for(int i = 0; i < key_size; ++i){
                if(node->leaf){
                    std::cout << *node->ptr2val[i] << " ";
                } else{
                    std::cout << node->key[i] << " ";
                    q.push(node->ptr2node[i]);
                    if(i + 1 == key_size)
                        q.push(node->ptr2node[i + 1]);
                }
            }
            std::cout << "| ";
        }
        std::cout << std::endl;
    }
}




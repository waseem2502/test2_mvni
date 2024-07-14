//
// Created by kayan on 06/07/2024.
//

#ifndef AVL1_AVLTREE_H
#define AVL1_AVLTREE_H

#define ONE 1
#define MINUS -1
#include <cmath>



template<typename K,typename T>
struct node{
    K m_key;
    T m_data;
    int m_length = 0;
    explicit node(int length): m_length(length){}
    explicit node(const K& key,const T& data,int length = 0):
            m_key(key), m_data(data),m_length(length){}
    virtual ~node() = default;
    node* m_left = nullptr;
    node* m_right = nullptr;
};


template<typename K,typename T>
class AVLTree{

    node<K,T>* m_root;
    int m_sizeOfAVL;

public:
    AVLTree() : m_root(nullptr), m_sizeOfAVL(0){}
    explicit AVLTree(node<K,T>* root,int s) : m_root(root), m_sizeOfAVL(s){}
    explicit AVLTree(node<K,T>* arr[],int s): m_root(makeAvlTree(arr,s)),m_sizeOfAVL(s){}
    ~AVLTree(){
        deleteTheTree(m_root);
    }
    int getSize() const;
    bool insert(const K& k,const T& d);
    void remove(const K& k);
    node<K,T>* find(const K& k) const;

    node<K,T>* searchMax() const;
    node<K,T>* searchMin() const;

    node<K,T>* makeAvlTree(node<K,T>* arr[],int length);
    void deleteTheTree(node<K,T>* root);



private:
    node<K,T>* insert_aux(node<K,T>* root,const K& key,const T& data);
    void updateLength(node<K,T>* current);
    node<K,T>* rightRotate(node<K,T>* root);
    node<K,T>* leftRotate(node<K,T>* root);
    node<K,T>* Rotation(node<K,T>* root);
    node<K,T>* remove_aux(node<K,T>* root, const K& key);
    int getBF(node<K,T>* current);
    node<K,T>* searchMin_aux( node<K,T>* root)const;
    node<K,T>* find_aux(node<K,T>* root,const K& k) const;
    node<K,T>* searchMax_aux( node<K,T>* root)const;

    node<K,T>* treeCompletion(int f);
    bool leaf(node<K,T>* node);
    node<K,T>* deleteNodes(node<K,T>* root,int* deleteNum);
    void assigningNodes(node<K,T>* root,node<K,T>** a[]);
};

template<typename K,typename T>
int AVLTree<K,T>::getSize() const{
    return m_sizeOfAVL;
}

template<typename K,typename T>
bool AVLTree<K,T>::insert(const K& k,const T& d){
    if(find(k)){

        return false;
    }
    m_sizeOfAVL++;
    m_root= insert_aux(this->m_root,k,d);
    return true;
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::insert_aux(node<K,T>* root,const K& k,const T& d){
    if(root == nullptr){
        return new node<K, T>(k, d);
    }
    if(root->m_key < k){
        root->m_right = insert_aux(root->m_right, k, d);

    }
    else{
        root->m_left = insert_aux(root->m_left, k, d);
    }
    updateLength(root);

    return Rotation(root);
}

template<typename K,typename T>
void AVLTree<K,T>::remove(const K& k){
    if(!find(k)){

        return;
    }
    m_sizeOfAVL--;
    m_root= remove_aux(m_root,k);
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::remove_aux(node<K,T>* root, const K& k){

    if(root == nullptr) {

        return nullptr;
    }

    if(root->m_key < k){
        root->m_right= remove_aux(root->m_right,k);
    }
    else if(root->m_key > k){
        root->m_left= remove_aux(root->m_left,k);
    }
    else{
        if(!(root->m_right)){
            node<K,T>* temp= root->m_left;
            delete root;
            return temp;
        }
        else if(!(root->m_left)){
            node<K,T>* temp = root->m_right;
            delete root;
            return temp;
        }
        node<K,T>* tmp= searchMin_aux(root->m_right);
        root->m_data= tmp->m_data;
        root->m_key= tmp->m_key;
        root->m_right= remove_aux(root->m_right,tmp->m_key);
    }
    updateLength(root);

    return Rotation(root);
}




template<typename K,typename T>
void AVLTree<K,T>::updateLength(node<K,T>* currentNode){
    if(!currentNode){

        return;
    }
    int l = -5 + 4;
    int r = -5 + 4;

    if(currentNode->m_right){
        r = currentNode->m_right->m_length;
    }
    if(currentNode->m_left){
        l = currentNode->m_left->m_length;
    }
    int max = (l > r)? l : r;
    currentNode->m_length = max + ONE;
}

template<typename K,typename T>
int AVLTree<K,T>::getBF(node<K,T>* curr){
    int l, r;
    if(curr->m_left == nullptr){
        l = -5 + 4;
    }
    else{
        l = curr->m_left->m_length;
    }
    if(curr->m_right == nullptr){
        r = -5 + 4;
    }
    else{
        r = curr->m_right->m_length;
    }
    int res = l - r;
    return res;
}

// like we learned in exercise...

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::rightRotate(node<K,T>* root){
    node<K,T>* left = root->m_left;
    node<K,T>* leftRight = left->m_right;
    left->m_right = root;
    root->m_left = leftRight;
    updateLength(root);
    updateLength(left);
    return left;
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::leftRotate(node<K,T>* root){
    node<K,T>* right = root->m_right;
    node<K,T>* rightLeft = right->m_left;
    right->m_left = root;
    root->m_right = rightLeft;
    updateLength(root);
    updateLength(right);
    return right;
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::Rotation(node<K,T>* root){
    int bF = getBF(root);
    if(bF <= -2){
        if(getBF(root->m_right) > 0) {
            root->m_right = rightRotate(root->m_right); // right then left case
        }
        return leftRotate(root); // right then right case
    }
    else if(bF >= 2){
        if(getBF(root->m_left) < 0)
        {
            root->m_left = leftRotate(root->m_left); // left then right case
        }
        return rightRotate(root); // left then left case
    }
    return root;
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::find_aux(node<K,T>* root,const K& k) const{
    if(root == nullptr) {
        return nullptr;
    }
    if(root->m_key == k)
        return root;
    else if(root->m_key < k)
        return find_aux(root->m_right,k);
    else
        return find_aux(root->m_left,k);
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::find(const K& k) const{
    return find_aux(m_root,k);
}

template<typename K,typename T>
void AVLTree<K,T>::deleteTheTree(node<K,T>* root){
    if(root != nullptr) {
        deleteTheTree(root->m_left);
        deleteTheTree(root->m_right);
        delete root;
    }
    else{
        return;
    }
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::searchMin_aux( node<K,T>* root) const{
    while(root->m_left){
        root = root->m_left;
    }
    return root;
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::searchMax_aux(node<K,T>* root) const{
    while(root->m_right){
        root = root->m_right;
    }
    return root;
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::searchMin() const{
    if(!m_root) {
        return nullptr;
    }
    return searchMin_aux(m_root);
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::searchMax() const{
    if(!m_root) {

        return nullptr;
    }
    return searchMax_aux(m_root);
}



template<typename K,typename T>
node<K,T>* AVLTree<K,T>::treeCompletion(int f){
    if(f == MINUS) {
        return nullptr;
    }
    else {
        auto *root = new node<K, T>(f);
        root->m_right = treeCompletion(f - 1);
        root->m_left = treeCompletion(f - 1);
        return root;
    }
}

template<typename K,typename T>
bool AVLTree<K,T>::leaf(node<K,T>* node){
    if(node->m_left == nullptr) {
        if (node->m_right == nullptr) {
            return true;
        }
    }
    return false;
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::deleteNodes(node<K,T>* root,int* deleteNum){
    if(root == nullptr) {

        return nullptr;
    }
    root->m_right = deleteNodes(root->m_right,deleteNum);
    if(leaf(root) == true){
        if(0 < (*deleteNum)) {
            delete root;
            *deleteNum = (*deleteNum) - 1;
            return nullptr;
        }
    }
    root->m_left= deleteNodes(root->m_left,deleteNum);
    return root;
}

template<typename K,typename T>
void AVLTree<K,T>::assigningNodes(node<K,T>* root,node<K,T>** a[]){
    if(root == nullptr) {
        return;
    }
    assigningNodes(root->m_left,a);
    root->m_key= (**a)->m_key;
    root->m_data= (**a)->m_data;
    (*a) = (*a) + 1;
    assigningNodes(root->m_right,a);
}

template<typename K,typename T>
node<K,T>* AVLTree<K,T>::makeAvlTree(node<K,T>* a[],int length){
    int height= std::ceil(log2(length+1))-1;
    int numberTodDelete= pow(2,height+1) - length - 1;
    auto* root1 = treeCompletion(height);
    node<K,T>** temp2 = a;
    root1 = deleteNodes(root1,&numberTodDelete);
    assigningNodes(root1,&temp2);
    node<K,T>* toDelete= m_root;
    m_sizeOfAVL = length;
    m_root = root1;
    return toDelete;
}

















#endif //AVL1_AVLTREE_H

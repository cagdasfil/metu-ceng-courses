#ifndef TWOPHASEBST_H
#define TWOPHASEBST_H

#include <iostream>
#include <string>
#include <stack>
#include <list>
// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class TwoPhaseBST {
private: //do not change
    struct SecondaryNode {
        std::string key;
        T data;
        SecondaryNode *left;
        SecondaryNode *right;

        SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r);
    };

    struct PrimaryNode {
        std::string key;
        PrimaryNode *left;
        PrimaryNode *right;
        SecondaryNode *rootSecondaryNode;

        PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn);
    };

public: // do not change.
    TwoPhaseBST();
    ~TwoPhaseBST();

    TwoPhaseBST &insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data);
    TwoPhaseBST &remove(const std::string &primaryKey, const std::string &secondaryKey);
    TwoPhaseBST &print(const std::string &primaryKey = "", const std::string &secondaryKey = "");
    T *find(const std::string &primaryKey, const std::string &secondaryKey);

private: // you may add your own utility member functions here.
    void destructNode(PrimaryNode * &root);
    void destructNode(SecondaryNode * &root);
    bool isEmpty();
    PrimaryNode* findPrimaryNodebyKey(const std::string& searchkey, PrimaryNode* searchroot);
    SecondaryNode* findSecondaryNodebyKey(const std::string& searchkey, SecondaryNode* searchroot);
    void insertPrimaryNode(const std::string& primaryKey, PrimaryNode*& root);
    void insertSecondaryNode(const std::string& secondaryKey, const T& data, SecondaryNode*& root);
    void removeSecondaryNode(const std::string& secondaryKey, SecondaryNode*& root);
    SecondaryNode* findMin(SecondaryNode* root);
    SecondaryNode* findMax(SecondaryNode* root);
    PrimaryNode* findMax(PrimaryNode* root);
    void printPrimary(PrimaryNode* root);
    void printSecondary(PrimaryNode* pr, SecondaryNode* root);
    SecondaryNode* findParent(const std::string& secondaryKey, SecondaryNode* root);

private: // do not change.
    PrimaryNode *root; //designated root.

    // do not provide an implementation. TwoPhaseBST's are not copiable.
    TwoPhaseBST(const TwoPhaseBST &);
    const TwoPhaseBST &operator=(const TwoPhaseBST &);
};

template <class T>
TwoPhaseBST<T>::SecondaryNode::SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r)
        : key(k), data(d), left(l), right(r) {}

template <class T>
TwoPhaseBST<T>::PrimaryNode::PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn)
        : key(k), left(l), right(r), rootSecondaryNode(rsn) {}

template <class T>
TwoPhaseBST<T>::TwoPhaseBST() : root(NULL) {}

template <class T>
TwoPhaseBST<T>::~TwoPhaseBST() {
    destructNode(root);
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data) {
    /* IMPLEMENT THIS */
    if(isEmpty()){
      root = new PrimaryNode(primaryKey, NULL, NULL, NULL);
      root->rootSecondaryNode = new SecondaryNode(secondaryKey, data, NULL, NULL);
      return *this;
    }

    PrimaryNode* tmpprimary = findPrimaryNodebyKey(primaryKey, root);

    if(tmpprimary == NULL){
      insertPrimaryNode(primaryKey, root);
      tmpprimary = findPrimaryNodebyKey(primaryKey, root);
      insertSecondaryNode(secondaryKey, data, tmpprimary->rootSecondaryNode);
    }
    else{
      insertSecondaryNode(secondaryKey, data, tmpprimary->rootSecondaryNode);
    }

    return *this;
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::remove(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    PrimaryNode* tmpprimary = findPrimaryNodebyKey(primaryKey, root);

    if(tmpprimary == NULL){
      return *this;
    }
    else{
      SecondaryNode* tmpsecondary = findSecondaryNodebyKey(secondaryKey, tmpprimary->rootSecondaryNode);

      if(tmpsecondary == NULL){
        return *this;
      }
      else{
        removeSecondaryNode(secondaryKey, tmpprimary->rootSecondaryNode);
      }
    }
    return *this;
}

template <class T>
TwoPhaseBST<T> &TwoPhaseBST<T>::print(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    if(primaryKey == "" && secondaryKey == ""){ // Both are not given.
      std::cout << "{";
      printPrimary(root);
      std::cout << "}" << '\n';
    }
    else if(primaryKey != "" && secondaryKey == ""){ // primaryKey is given.
      PrimaryNode* tmp = findPrimaryNodebyKey(primaryKey, root);
      if(tmp == NULL) {
        std::cout << "{}" << '\n';
        return *this;
      }
      std::cout << "{\"" << tmp->key << "\" : {";
      printSecondary(tmp, tmp->rootSecondaryNode);
      std::cout << "}}" << '\n';
    }
    else if(primaryKey == "" && secondaryKey != ""){ // No action.

    }
    else{ // Both are given.
      PrimaryNode* tmppr = findPrimaryNodebyKey(primaryKey, root);
      if(tmppr == NULL){
        std::cout << "{}" << '\n';
        return *this;
      }
      SecondaryNode* tmpse = findSecondaryNodebyKey(secondaryKey, tmppr->rootSecondaryNode);
      if(tmpse == NULL){
        std::cout << "{}" << '\n';
      }
      else{
        std::cout << "{\"" << tmppr->key << "\" : {\"" << tmpse->key << "\" : \"";
        std::cout << tmpse->data << "\"}}" << '\n';
      }
    }

    return *this;
}

template <class T>
T *TwoPhaseBST<T>::find(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    PrimaryNode* tmppr = findPrimaryNodebyKey(primaryKey, root);
    if(!tmppr) {
      return NULL;
    }
    SecondaryNode* tmpse = findSecondaryNodebyKey(secondaryKey, tmppr->rootSecondaryNode);
    if(tmpse){
      return &(tmpse->data);
    }
    return NULL;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::PrimaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    destructNode(root->rootSecondaryNode);

    delete root;

    root = NULL;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::SecondaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    delete root;

    root = NULL;
}

template <class T>
bool TwoPhaseBST<T>::isEmpty(){
  return root == NULL;
}

template <class T>
typename TwoPhaseBST<T>::PrimaryNode* TwoPhaseBST<T>::findPrimaryNodebyKey(const std::string& searchkey, PrimaryNode* searchroot){
  if(searchroot==NULL){
    return NULL;
  }
  else if(searchkey < searchroot->key){
    return findPrimaryNodebyKey(searchkey, searchroot->left);
  }
  else if(searchkey > searchroot->key){
    return findPrimaryNodebyKey(searchkey, searchroot->right);
  }
  else{
    return searchroot;
  }
}

template <class T>
typename TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::findSecondaryNodebyKey(const std::string& searchkey, SecondaryNode* searchroot){
  if(searchroot==NULL){
    return NULL;
  }
  else if(searchkey < searchroot->key){
    return findSecondaryNodebyKey(searchkey, searchroot->left);
  }
  else if(searchkey > searchroot->key){
    return findSecondaryNodebyKey(searchkey, searchroot->right);
  }
  else{
    return searchroot;
  }
}

template <class T>
void TwoPhaseBST<T>::insertPrimaryNode(const std::string& primaryKey, PrimaryNode*& root){
  if(root == NULL){
    root = new PrimaryNode(primaryKey, NULL, NULL, NULL);
  }
  else if(primaryKey < root->key){
    insertPrimaryNode(primaryKey, root->left);
  }
  else if(primaryKey > root->key){
    insertPrimaryNode(primaryKey, root->right);
  }
  else
    ;
}

template <class T>
void TwoPhaseBST<T>::insertSecondaryNode(const std::string& secondaryKey, const T& data, SecondaryNode*& root){
  if(root == NULL){
    root = new SecondaryNode(secondaryKey, data, NULL, NULL);
  }
  else if(secondaryKey < root->key){
    insertSecondaryNode(secondaryKey, data, root->left);
  }
  else if(secondaryKey > root->key){
    insertSecondaryNode(secondaryKey, data, root->right);
  }
  else
    ;
}

template <class T>
void TwoPhaseBST<T>::removeSecondaryNode(const std::string& secondaryKey, SecondaryNode*& root){
  if(root == NULL){
    return;
  }
  if(secondaryKey < root->key) {
    removeSecondaryNode(secondaryKey, root->left);
  }
  else if(secondaryKey > root->key){
    removeSecondaryNode(secondaryKey, root->right);
  }
  else if(root->left != NULL && root->right != NULL){
    SecondaryNode* min = findMin(root->right);
    SecondaryNode* parentmin = findParent(min->key, root);
    SecondaryNode* tmproot = root;
    if(parentmin->key == root->key){
      root = min;
      min->left = tmproot->left;
    }
    else{
      parentmin->left = min->right;
      min->left = tmproot->left;
      min->right = tmproot->right;
      root = min;
    }
    delete tmproot;
  }
  else{
    SecondaryNode* old = root;
    root = (root->left != NULL) ? root->left : root->right;
    delete old;
  }
}

template <class T>
typename TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::findMin(SecondaryNode* root){
  if(root == NULL) {
    return NULL;
  }
  if(root->left == NULL) {
    return root;
  }
  return findMin(root->left);
}

template <class T>
typename TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::findMax(SecondaryNode* root){
  if(root == NULL) {
    return NULL;
  }
  if(root->right == NULL) {
    return root;
  }
  return findMax(root->right);
}

template <class T>
typename TwoPhaseBST<T>::PrimaryNode* TwoPhaseBST<T>::findMax(PrimaryNode* root){
  if(root == NULL) {
    return NULL;
  }
  if(root->right == NULL) {
    return root;
  }
  return findMax(root->right);
}

template <class T>
void TwoPhaseBST<T>::printPrimary(PrimaryNode* root){
  if(root != NULL){
    printPrimary(root->left);
    std::cout << "\"" <<root->key << "\" : {";
    printSecondary(root, root->rootSecondaryNode);
    std::cout << "}";
    if(root->key != findMax(this->root)->key){
      std::cout << ", ";
    }
    printPrimary(root->right);
  }
}

template <class T>
void TwoPhaseBST<T>::printSecondary(PrimaryNode* pr, SecondaryNode* root){
  if(root != NULL) {
    printSecondary(pr, root->left);
    std::cout << "\"" << root->key << "\" : \"";
    std::cout << root->data << "\"";
    if(root->key != findMax(pr->rootSecondaryNode)->key){
      std::cout << ", ";
    }
    printSecondary(pr, root->right);
  }
}

template <class T>
typename TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::findParent(const std::string& secondaryKey, SecondaryNode* root){
  if(root == NULL) {
    return NULL;
  }
  if(root->key == secondaryKey){
    return NULL;
  }
  if(secondaryKey > root->key) {
    if(root->right != NULL){
      if(root->right->key == secondaryKey){
        return root;
      }
      else{
        return findParent(secondaryKey, root->right);
      }
    }
    else{
      return NULL;
    }
  }
  if(secondaryKey < root->key) {
    if(root->left != NULL){
      if(root->left->key == secondaryKey){
        return root;
      }
      else{
        return findParent(secondaryKey, root->left);
      }
    }
    else{
      return NULL;
    }
  }
  return NULL;
}


#endif //TWOPHASEBST_H

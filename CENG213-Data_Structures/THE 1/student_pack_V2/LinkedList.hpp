#ifndef LINKEDLIST_HPP
#define	LINKEDLIST_HPP

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T>
class LinkedList {
private:
    Node<T>* head;
    int length;
public:

    LinkedList();
    LinkedList(const LinkedList<T>& ll);
    LinkedList<T>& operator=(const LinkedList<T>& ll);
    ~LinkedList();


    Node<T>* getHead() const;
    Node<T>* first() const;
    Node<T>* findPrev(const T& data) const;
    Node<T>* findNode(const T& data) const;
    void insertNode(Node<T>* prev, const T& data);
    void deleteNode(Node<T>* prevNode);
    void clear();
    size_t getLength() const;
    void print() const;
    void swap(int index1, int index2);
};

template <class T>
void LinkedList<T>::print() const {
    const Node<T>* node = first();
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout << std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */

template<class T>
LinkedList<T>::LinkedList(){
  head = new Node<T>();
  length = 0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& ll){
  head = new Node<T>();
  *this = ll;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll){
  if (this != &ll){
    clear();
    const Node<T>* r = ll.first();
    Node<T>* p = getHead();
    while (r) {
      insertNode(p, r->getData());
      r = r->getNext();
      p = p->getNext();
    }
    length = ll.getLength();
  }
  return *this;
}

template<class T>
LinkedList<T>::~LinkedList(){
  clear();
  delete head;
}

template<class T>
Node<T>* LinkedList<T>::getHead() const{
  return head;
}

template<class T>
Node<T>* LinkedList<T>::first() const{
  return head->getNext();
}

template<class T>
Node<T>* LinkedList<T>::findPrev(const T& data) const{
  Node<T>* p = getHead();
  while (p->getNext()) {
    if (p->getNext()->getData() == data){
      return p;
    }
    p = p->getNext();
  }
  return NULL;
}

template<class T>
Node<T>* LinkedList<T>::findNode(const T& data) const{
  Node<T>* p = first();
  while(p){
    if(p->getData() == data){
      return p;
    }
    p = p->getNext();
  }
  return NULL;
}

template<class T>
void LinkedList<T>::insertNode(Node<T>* prev, const T& data){
  Node<T>* newNode = new Node<T>(data);
  newNode->setNext(prev->getNext());
  prev->setNext(newNode);
  length++;
}

template<class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode){
  if(prevNode){
    Node<T>* tmp = prevNode->getNext();
    prevNode->setNext(tmp->getNext());
    delete tmp;
    length--;
  }
}

template<class T>
void LinkedList<T>::clear(){
  while (getHead()->getNext() != NULL) {
    deleteNode(getHead());
  }
  length = 0;
}

template<class T>
size_t LinkedList<T>::getLength() const{
  return length;
}

template<class T>
void LinkedList<T>::swap(int index1, int index2){
  int i1, i2, i=-1;
  Node<T>* p;
  Node<T>* node1prev;
  Node<T>* node1;
  Node<T>* node1next;
  Node<T>* node2prev;
  Node<T>* node2;
  Node<T>* node2next;

  p = getHead();

  if(index1<index2){
    i1 = index1;
    i2 = index2;
  }
  else if(index1>index2){
    i1 = index2;
    i2 = index1;
  }
  else{
    return;
  }

  if(i2 >= getLength() || i1<0 || i2<0){
    return;
  }


  while(p){
    if(i==i1-1){
      node1prev = p;
      node1 = p->getNext();
      node1next = p->getNext()->getNext();
    }
    if(i==i2-1){
      node2prev = p;
      node2 = p->getNext();
      node2next = p->getNext()->getNext();
      break;
    }
    p = p->getNext();
    i++;
  }

  if( (i2-i1) == 1){
    node1prev->setNext(node2);
    node2->setNext(node1);
    node1->setNext(node2next);
  }
  else{
    node1prev->setNext(node2);
    node2->setNext(node1next);
    node2prev->setNext(node1);
    node1->setNext(node2next);
  }

  if (i2 == getLength()-1) {
    node1->setNext(NULL);
  }

}


/* end of your implementations*/
#endif

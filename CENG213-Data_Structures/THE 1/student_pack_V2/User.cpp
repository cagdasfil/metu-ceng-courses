#include <iostream>
#include "User.hpp"
#include "Video.hpp"

void User::printSubscriptions() {
    Node< Video*> * first = subscriptions.first();
    while (first) {
        cout << *(first->getData());
        first = first->getNext();
    }
    cout << std::endl;
}

void User::printFriends() {
    Node< User*> * first = friends.first();
    while (first) {
        cout << *(first->getData());
        first = first->getNext();
    }
    cout << std::endl;
}

ostream& operator<<(ostream& out, const User & user) {
    string st = (user.status == ACTIVE) ? "active" : "suspended";
    out << "username:" << user.username << ",name:" << user.name << ",surname:" << user.surname << ",Status:" << st << endl;
    return out;
}

/* TO-DO: method implementations below */

User::User(){
  username = "";
  name = "";
  surname = "";
  status = ACTIVE;
}
User::User(string username, string name , string surname ){
  this->username = username;
  this->name = name;
  this->surname = surname;
  this->status = ACTIVE;
}
User::~User(){
}

const string& User::getUsername() const{
  return username;
}
const string& User::getName() const{
  return name;
}
const string& User::getSurname() const{
  return surname;
}
Status User::getStatus() const{
  return status;
}
void User::updateStatus(Status st){
  status = st;
}
void User::subscribe(Video * video){
  subscriptions.insertNode(subscriptions.getHead(), video);
}
void User::unSubscribe(Video * video){
  Node<Video*>* tmp1;
  Node<Video*>* tmp2;
  tmp1 = subscriptions.findPrev(video);
  tmp2 = tmp1->getNext();
  tmp1->setNext(tmp2->getNext());
  delete tmp2;
}
void User::addFriend(User * user){
  if(username == user->getUsername()){
    return;
  }
  friends.insertNode(friends.getHead(), user);
}
void User::removeFriend(User * user){
  Node<User*>* tmp1;
  Node<User*>* tmp2;
  tmp1 = friends.findPrev(user);
  tmp2 = tmp1->getNext();
  tmp1->setNext(tmp2->getNext());
  delete tmp2;
}
LinkedList<Video* > * User::getSubscriptions(){
  return &subscriptions;
}
LinkedList< User* > * User::getFriends(){
  return &friends;
}
bool User::operator==(const User& rhs) const{
  if(username == rhs.getUsername()){
    return true;
  }
  else{
    return false;
  }
}

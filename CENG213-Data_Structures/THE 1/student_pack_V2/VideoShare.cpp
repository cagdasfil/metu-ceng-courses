#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "VideoShare.hpp"
#include "User.hpp"

using namespace std;

void VideoShare::printAllVideos() {
    videos.print();
}

void VideoShare::printAllUsers() {
    users.print();
}

/* TO-DO: method implementations below */

VideoShare::VideoShare(){
}

VideoShare::~VideoShare(){
}

void VideoShare::createUser(const string & userName, const string & name , const string & surname ){
  User newuser(userName, name, surname);
  users.insertNode(users.getHead(), newuser);
}
void VideoShare::loadUsers(const string & fileName){
  ifstream file;
  string line,tmp,username,name,surname;
  User* user;
  size_t pos;
  file.open(fileName.c_str(), ios::in);
  if(file.is_open()){
    while (getline(file,line)) {
      pos = line.find(";");
      username = line.substr(0,pos);
      tmp = line.substr(pos+1);
      pos = tmp.find(";");
      name = tmp.substr(0,pos);
      surname = tmp.substr(pos+1);
      if (username != "") {
        user = new User(username,name,surname);
        users.insertNode(users.getHead(), *user);
      }
    }
    file.close();
  }
}
void VideoShare::createVideo(const string & title, const string & genre){
  Video newvideo(title, genre);
  videos.insertNode(videos.getHead(), newvideo);
}
void VideoShare::loadVideos(const string & fileName){
  ifstream file;
  string line, title, genre;
  Video* video;
  size_t pos;
  file.open(fileName.c_str(), ios::in);
  if(file.is_open()){
    while (getline(file,line)) {
      pos = line.find(";");
      title = line.substr(0,pos);
      genre = line.substr(pos+1);
      if (title != "") {
        video = new Video(title, genre);
        videos.insertNode(videos.getHead(), *video);
      }
    }
    file.close();
  }
}
void VideoShare::addFriendship(const string & userName1, const string & userName2){
  Node<User>* user1nodeptr = NULL;
  Node<User>* user2nodeptr = NULL;
  Node<User>* tmp = users.first();

  if(userName1 == userName2){
    return;
  }

  while (tmp != NULL) {
    if( (tmp->getData()).getUsername() == userName1){
      user1nodeptr = tmp;
    }
    else if( (tmp->getData()).getUsername() == userName2 ){
      user2nodeptr = tmp;
    }
    else{
    }
    tmp = tmp->getNext();
  }

  if(user1nodeptr == NULL || user2nodeptr == NULL){
    return;
  }

  (user1nodeptr->getDataPtr())->addFriend(user2nodeptr->getDataPtr());
  (user2nodeptr->getDataPtr())->addFriend(user1nodeptr->getDataPtr());
}
void VideoShare::removeFriendship(const string & userName1, const string & userName2){
  Node<User>* user1nodeptr = NULL;
  Node<User>* user2nodeptr = NULL;
  Node<User>* tmp = users.getHead();

  if(userName1 == userName2){
    return;
  }

  while (tmp) {
    if( (tmp->getData()).getUsername() == userName1){
      user1nodeptr = tmp;
    }
    else if( (tmp->getData()).getUsername() == userName2 ){
      user2nodeptr = tmp;
    }
    else{
    }
    tmp = tmp->getNext();
  }

  if(user1nodeptr == NULL || user2nodeptr == NULL){
    return;
  }

  (user1nodeptr->getDataPtr())->removeFriend(user2nodeptr->getDataPtr());
  (user2nodeptr->getDataPtr())->removeFriend(user1nodeptr->getDataPtr());

}
void VideoShare::updateUserStatus(const string & userName, Status newStatus){
  Node<User>* usernodeptr = users.getHead();
  while(usernodeptr){
    if( (usernodeptr->getData()).getUsername() == userName){
      (usernodeptr->getDataPtr())->updateStatus(newStatus);
      break;
    }
    usernodeptr = usernodeptr->getNext();
  }
}
void VideoShare::subscribe(const string & userName, const string & videoTitle){
  Node<Video>* videonodeptr = videos.first();
  Node<User>* usernodeptr = users.first();
  while(videonodeptr){
    if( (videonodeptr->getData()).getTitle() == videoTitle){
      break;
    }
    videonodeptr = videonodeptr->getNext();
  }
  while(usernodeptr) {
    if( (usernodeptr->getData()).getUsername() == userName){
      break;
    }
    usernodeptr = usernodeptr->getNext();
  }
  if(videonodeptr != NULL && usernodeptr != NULL){
    (usernodeptr->getDataPtr())->subscribe(videonodeptr->getDataPtr());
  }
}
void VideoShare::unSubscribe(const string & userName, const string & videoTitle){
  Node<Video>* videonodeptr = videos.getHead();
  Node<User>* usernodeptr = users.getHead();
  while(videonodeptr){
    if( (videonodeptr->getData()).getTitle() == videoTitle){
      break;
    }
    videonodeptr = videonodeptr->getNext();
  }
  while(usernodeptr) {
    if( (usernodeptr->getData()).getUsername() == userName){
      break;
    }
    usernodeptr = usernodeptr->getNext();
  }
  if(videonodeptr != NULL && usernodeptr != NULL){
    (usernodeptr->getDataPtr())->unSubscribe(videonodeptr->getDataPtr());
  }
}
void VideoShare::deleteUser(const string & userName){
  Node<User>* usernodeptr = users.getHead();
  Node<User*>* friendnodeptr;
  Node<User*>* friendnodeptr2;
  Node<User>* prev;

  while(usernodeptr) {
    if( (usernodeptr->getDataPtr())->getUsername() == userName){
      break;
    }
    usernodeptr = usernodeptr->getNext();
  }
  if(usernodeptr == NULL){
    return;
  }
  friendnodeptr = usernodeptr->getDataPtr()->getFriends()->first();
  while(friendnodeptr != NULL){
    friendnodeptr2 = friendnodeptr->getNext();
    removeFriendship(usernodeptr->getDataPtr()->getUsername(), friendnodeptr->getData()->getUsername());
    friendnodeptr = friendnodeptr2;
  }
  prev = users.findPrev(usernodeptr->getDataPtr()->getUsername());
  prev->setNext(usernodeptr->getNext());
  delete usernodeptr;
}
void VideoShare::sortUserSubscriptions(const string & userName){
  size_t i, j, n;
  bool sorted = false;
  Node<User>* usernodeptr = users.first();
  Node<Video*>* video1ptr;
  Node<Video*>* video2ptr;
  while(usernodeptr) {
    if( (usernodeptr->getData()).getUsername() == userName ){
      break;
    }
    usernodeptr = usernodeptr->getNext();
  }
  if(usernodeptr == NULL){
    return;
  }
  n = ((usernodeptr->getDataPtr())->getSubscriptions())->getLength();
  for (i=0; (i<n) && !sorted ; i++) {
    sorted = true;
    for (j=1; j < (n-i) ; j++) {
      video1ptr = getNodePtrbyIndex((usernodeptr->getDataPtr())->getSubscriptions(), j-1);
      video2ptr = getNodePtrbyIndex((usernodeptr->getDataPtr())->getSubscriptions(), j);
      if(video1ptr->getData()->getTitle() > video2ptr->getData()->getTitle()){
        ((usernodeptr->getDataPtr())->getSubscriptions())->swap(j,j-1);
        sorted=false;
      }
    }
  }
}

void VideoShare::printUserSubscriptions(const string & userName){
  Node<User>* usernodeptr = users.first();
  while(usernodeptr) {
    if( (usernodeptr->getData()).getUsername() == userName ){
      break;
    }
    usernodeptr = usernodeptr->getNext();
  }
  if(usernodeptr == NULL){
    return;
  }
  usernodeptr->getDataPtr()->printSubscriptions();
}
void VideoShare::printFriendsOfUser(const string & userName){
  Node<User>* usernodeptr = users.first();
  while(usernodeptr) {
    if( (usernodeptr->getData()).getUsername() == userName ){
      break;
    }
    usernodeptr = usernodeptr->getNext();
  }
  if(usernodeptr == NULL){
    return;
  }
  usernodeptr->getDataPtr()->printFriends();
}
void VideoShare::printCommonSubscriptions(const string & userName1, const string & userName2){
  Node<Video*>* videoptr1nodeptr = NULL;
  Node<Video*>* videoptr2nodeptr = NULL;
  Node<User>* user1nodeptr = NULL;
  Node<User>* user2nodeptr = NULL;
  Node<User>* tmp = users.getHead();

  while (tmp) {
    if( (tmp->getData()).getUsername() == userName1){
      user1nodeptr = tmp;
    }
    else if( (tmp->getData()).getUsername() == userName2 ){
      user2nodeptr = tmp;
    }
    else{
    }
    tmp = tmp->getNext();
  }

  if(userName1 == userName2){
    user2nodeptr = user1nodeptr;
  }

  if(user1nodeptr == NULL || user2nodeptr == NULL){
    return;
  }

  videoptr1nodeptr = user1nodeptr->getDataPtr()->getSubscriptions()->first();
  videoptr2nodeptr = user2nodeptr->getDataPtr()->getSubscriptions()->first();

  while(videoptr1nodeptr && videoptr2nodeptr){
    if(videoptr1nodeptr->getData()->getTitle() == videoptr2nodeptr->getData()->getTitle()){
      std::cout << *(videoptr1nodeptr->getData()) << '\n';
      videoptr1nodeptr = videoptr1nodeptr->getNext();
      videoptr2nodeptr = videoptr2nodeptr->getNext();
    }
    else if(videoptr1nodeptr->getData()->getTitle() > videoptr2nodeptr->getData()->getTitle()){
      videoptr2nodeptr = videoptr2nodeptr->getNext();
    }
    else{
      videoptr1nodeptr = videoptr1nodeptr->getNext();
    }
  }
}
void VideoShare::printFriendSubscriptions(const string & userName){
  Node<User>* usernodeptr = users.first();
  Node<User*>* userfriendptr;
  Node<Video*>* userfriendvideoptr;
  Node<Video*>* searchptr;
  LinkedList<Video*> friendsvideos;
  int len;
  while(usernodeptr) {
    if( (usernodeptr->getData()).getUsername() == userName ){
      break;
    }
    usernodeptr = usernodeptr->getNext();
  }
  if(usernodeptr == NULL){
    return;
  }
  userfriendptr = usernodeptr->getDataPtr()->getFriends()->first();
  while(userfriendptr) {
    userfriendvideoptr = userfriendptr->getData()->getSubscriptions()->first();
    while(userfriendvideoptr) {
      searchptr = friendsvideos.getHead();
      if(friendsvideos.getLength() == 0){
        friendsvideos.insertNode(friendsvideos.getHead(), userfriendvideoptr->getData());
        continue;
      }
      while(searchptr->getNext()) {
        if(*(searchptr->getNext()->getData()) == *(userfriendvideoptr->getData())){
          break;
        }
        else if(searchptr->getNext()->getNext() == NULL){
          friendsvideos.insertNode(friendsvideos.getHead(), userfriendvideoptr->getData());
          break;
        }
        searchptr = searchptr->getNext();
      }
      userfriendvideoptr = userfriendvideoptr->getNext();
    }
    userfriendptr = userfriendptr->getNext();
  }

  searchptr = friendsvideos.first();

  len = friendsvideos.getLength();
  while (searchptr && len != 0) {
    std::cout << *(searchptr->getData()) << '\n';
    searchptr->getNext();
    len--;
  }
}
bool VideoShare::isConnected(const string & userName1, const string & userName2){

}
Node<Video*>* VideoShare::getNodePtrbyIndex(LinkedList<Video*>* subscriptionslistptr, size_t index){
  size_t i=0;
  Node<Video*>* videoptrnodeptr = subscriptionslistptr->first();
  while(i != index){
    videoptrnodeptr = videoptrnodeptr->getNext();
    i++;
  }
  return videoptrnodeptr;
}

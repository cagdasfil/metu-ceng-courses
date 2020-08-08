#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "VideoShare.hpp"
int main(int argc, char const *argv[]) {
  Node<int> a(5);
  Node<char> b('x');
  Node<int> c(3);
  Node<std::string> e("cago");
  a.setNext(&c);
  std::cout << a.getNext()->getData() << '\n';
  LinkedList<std::string> list;

  list.insertNode(list.getHead(),"a");
  list.insertNode(list.getHead(),"b");
  list.insertNode(list.getHead(),"c");

  list.print();
  std::cout << "len" << list.getLength() << '\n';
  list.clear();

  list.print();
  std::cout << "len" << list.getLength() << '\n';

  list.insertNode(list.getHead(),"a");
  list.insertNode(list.getHead(),"b");
  list.insertNode(list.getHead(),"c");

  list.print();
  std::cout << "len" << list.getLength() << '\n';

  std::cout << "head:" << *(list.getHead()) << '\n';
  std::cout << "first:" << *(list.first()) << '\n';
  std::cout << "next:" << *(list.first()->getNext()) << '\n';
  std::cout << "find data(b) : " << list.findNode("b")->getData() << '\n';
  std::cout << "find prev(c) : " << list.findPrev("b")->getData() << '\n';

  list.insertNode(list.findNode("c"),"d");
  list.insertNode(list.findNode("a"),"e");

  list.print();

  list.swap(0,4);

  list.print();

  list.deleteNode(list.findNode("e"));

  list.print();

  list.swap(0,3);

  list.print();
  std::cout << "len" << list.getLength() << '\n';

  User user1("cago","cagdas","fil");

  std::cout << user1 << '\n';

  User user2("kali","kali","muscle");

  std::cout << user2 << '\n';

  user1.addFriend(&user2);

  user1.printFriends();

  user2.printFriends();

  user1.removeFriend(&user2);

  user1.printFriends();

  user1.addFriend(&user2);

  std::cout << *(user1.getFriends()->first()->getData()) << '\n';

  Video video1("title1","genre1");
  Video video2("title2","genre2");
  Video video3("title3","genre3");

  user1.subscribe(&video1);
  user1.subscribe(&video2);
  user2.subscribe(&video2);
  user2.subscribe(&video3);

  user1.printSubscriptions();

  user2.printSubscriptions();

  user1.unSubscribe(&video1);
  user2.unSubscribe(&video3);

  user1.printSubscriptions();

  user2.printSubscriptions();

  user1.subscribe(&video1);
  user2.subscribe(&video3);

  VideoShare platform;

  platform.createUser("cago","cagdas","fil");
  platform.createUser("kivo","kivanc","fil");

  platform.createVideo("title1","genre1");
  platform.createVideo("title2","genre2");

  platform.addFriendship("cago","kivo");

  platform.printAllUsers();
  platform.printAllVideos();

  platform.printFriendsOfUser("cago");
  platform.printFriendsOfUser("kivo");

  platform.removeFriendship("cago","kivo");

  platform.printFriendsOfUser("cago");
  platform.printFriendsOfUser("kivo");

  platform.addFriendship("cago","kivo");

  platform.subscribe("cago","title1");
  platform.subscribe("cago","title2");
  platform.subscribe("kivo","title1");

  platform.unSubscribe("cago","title1");

  platform.printUserSubscriptions("cago");
  platform.printUserSubscriptions("kivo");

  platform.deleteUser("kivo");

  platform.printAllUsers();

  platform.createUser("kivo","kivanc","fil");

  platform.addFriendship("cago","kivo");

  platform.subscribe("cago","title1");
  platform.subscribe("kivo","title1");

  platform.printUserSubscriptions("cago");
  platform.printUserSubscriptions("kivo");

  platform.printCommonSubscriptions("cago","kivo");

  platform.printFriendSubscriptions("kivo");


  return 0;
}

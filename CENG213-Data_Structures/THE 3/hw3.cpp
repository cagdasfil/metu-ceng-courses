#include "hw3.h"

AccessControl::AccessControl(int table1Size, int table2Size){
  std::pair<std::string,std::string> initializer;
  initializer.first = EMPTY;
  initializer.second = EMPTY;

  this->table1Size = table1Size;
  this->table2Size = table2Size;
  elementsOfTable1 = 0;
  elementsOfTable2 = 0;

  regUsers.assign(table1Size, initializer);
  activeUsers.assign(table2Size, EMPTY);
}
AccessControl::~AccessControl(){

}
int AccessControl::addUser(std::string username, std::string pass){
  int hash, emptyPlace = -1;
  for(int i = 0; i < table1Size; i++) {
    hash = hashFunction(username, table1Size, i);
    if(regUsers[hash].first == username) {
      return 0;
    }
    else if(regUsers[hash].first == DELETED){
      if(emptyPlace == -1){
        emptyPlace = hash;
      }
    }
    else if(regUsers[hash].first == EMPTY) {
      if(emptyPlace == -1){
        regUsers[hash].first = username;
        regUsers[hash].second = pass;
      }
      else{
        regUsers[emptyPlace].first = username;
        regUsers[emptyPlace].second = pass;
      }
      elementsOfTable1++;
      if((double)elementsOfTable1/table1Size > MAX_LOAD_FACTOR){
        expandTable1();
      }
      return 1;
    }
  }
  return 0;
}
int AccessControl::addUsers(std::string filePath){
  std::ifstream file;
  int pos, added = 0;
  std::string entry, username, password;
  file.open(filePath.c_str());
  while(getline(file, entry)) {
    pos = entry.find(' ');
    username = entry.substr(0,pos);
    password = entry.substr(pos+1);
    if(addUser(username, password)) {
      added++;
    }
  }
  file.close();
  return added;
}
int AccessControl::delUser(std::string username, std::vector<std::string>& oldPasswords){
  int hash;
  bool registered = false;
  for (int i = 0; i < table1Size; i++) {
    hash = hashFunction(username, table1Size, i);
    if(regUsers[hash].first == EMPTY){
      break;
    }
    else if(regUsers[hash].first == username){
      registered = true;
      oldPasswords.push_back(regUsers[hash].second);
      regUsers[hash].first = DELETED;
      regUsers[hash].second = DELETED;
      elementsOfTable1--;
    }
  }
  if(registered) {
    return 1;
  }
  return 0;
}
int AccessControl::changePass(std::string username, std::string oldpass, std::string newpass){
  int hash;
  std::string currentPassword;
  for (int i = 0; i < table1Size; i++) {
    hash = hashFunction(username, table1Size, i);
    if(regUsers[hash].first == EMPTY){
      break;
    }
    else if(regUsers[hash].first == username){
      currentPassword = regUsers[hash].second;
    }
  }
  if(currentPassword == oldpass){
    for (int i = 0; i < table1Size; i++) {
      hash = hashFunction(username, table1Size, i);
      if(regUsers[hash].first == EMPTY || regUsers[hash].first == DELETED){
        regUsers[hash].first = username;
        regUsers[hash].second = newpass;
        elementsOfTable1++;
        if((double)elementsOfTable1/table1Size > MAX_LOAD_FACTOR){
          expandTable1();
        }
        return 1;
      }
    }
  }
  return 0;
}
int AccessControl::login(std::string username, std::string pass){
  int hash, emptyPlace = -1;
  std::string password;
  for (int i = 0; i < table1Size; i++) {
    hash = hashFunction(username, table1Size, i);
    if(regUsers[hash].first == EMPTY){
      break;
    }
    else if(regUsers[hash].first == username){
      password = regUsers[hash].second;
    }
  }
  if(pass == password){
    for(int i = 0; i < table2Size; i++){
      hash = hashFunction(username, table2Size, i);
      if(activeUsers[hash] == username){
        return 0;
      }
      else if(activeUsers[hash] == DELETED){
        if(emptyPlace == -1){
          emptyPlace = hash;
        }
      }
      else if(activeUsers[hash] == EMPTY) {
        if(emptyPlace == -1){
          activeUsers[hash] = username;
        }
        else{
          activeUsers[emptyPlace] = username;
        }
        elementsOfTable2++;
        if((double)elementsOfTable2/table2Size > MAX_LOAD_FACTOR){
          expandTable2();
        }
        return 1;
      }
    }
  }
  return 0;
}
int AccessControl::logout(std::string username){
  int hash;
  for (int i = 0; i < table2Size; i++) {
    hash = hashFunction(username, table2Size, i);
    if(activeUsers[hash] == username){
      activeUsers[hash] = DELETED;
      elementsOfTable2--;
      return 1;
    }
    else if(activeUsers[hash] == EMPTY) {
      return 0;
    }
  }
  return 0;
}
float AccessControl::printActiveUsers(){
  for (unsigned i = 0; i < activeUsers.size(); i++) {
    std::cout << activeUsers[i] << '\n';
  }
  return float(elementsOfTable2)/table2Size;
}
float AccessControl::printPasswords(){
  for (unsigned i = 0; i < regUsers.size(); i++) {
    std::cout << regUsers[i].first << " " << regUsers[i].second << '\n';
  }
  return float(elementsOfTable1)/table1Size;
}
int AccessControl::getNewTableSize(int oldSize){
  for (int i = 2*oldSize+1; ; i++) {
    for (int j = 2; j < i; j++) {
      if (i%j == 0) {
        break;
      }
      else if(j == i-1){
        return i;
      }
    }
  }
}
void AccessControl::expandTable1(){
  int newsize;
  int elementsOfNewTable = 0;
  int hash, newhash;
  std::string username, password;
  std::vector< std::pair<std::string,std::string> > newtable1;
  std::pair<std::string,std::string> initializer;
  initializer.first = EMPTY;
  initializer.second = EMPTY;
  newsize = getNewTableSize(table1Size);
  newtable1.assign(newsize, initializer);
  for (int i = 0; i < table1Size; i++) {
    if(regUsers[i].first != EMPTY && regUsers[i].first != DELETED){
      username = regUsers[i].first;
      for (int j = 0; j < table1Size; j++) {
        hash = hashFunction(username, table1Size, j);
        if(regUsers[hash].first == EMPTY){
          break;
        }
        else if(regUsers[hash].first == username){
          password = regUsers[hash].second;
          for(int k = 0; k < newsize; k++) {
            newhash = hashFunction(username, newsize, k);
            if(newtable1[newhash].first == username && newtable1[newhash].second == password) {
              break;
            }
            else if(newtable1[newhash].first == EMPTY) {
              newtable1[newhash].first = username;
              newtable1[newhash].second = password;
              elementsOfNewTable++;
              regUsers[hash].first = DELETED;
              regUsers[hash].second = DELETED;
              break;
            }
          }
        }
      }
    }
  }
  regUsers = newtable1;
  elementsOfTable1 = elementsOfNewTable;
  table1Size = regUsers.size();
}
void AccessControl::expandTable2(){
  int newsize;
  int elementsOfNewTable = 0;
  int newhash;
  std::string username, password;
  std::vector< std::string > newtable2;
  newsize = getNewTableSize(table2Size);
  newtable2.assign(newsize, EMPTY);
  for (int i = 0; i < table2Size; i++) {
    if(activeUsers[i] != EMPTY && activeUsers[i] != DELETED){
      username = activeUsers[i];
      for(int j = 0; j < newsize; j++) {
        newhash = hashFunction(username, newsize, j);
        if(newtable2[newhash] == EMPTY) {
          newtable2[newhash] = username;
          elementsOfNewTable++;
          break;
        }
      }
    }
  }
  activeUsers = newtable2;
  elementsOfTable2 = elementsOfNewTable;
  table2Size = activeUsers.size();
}

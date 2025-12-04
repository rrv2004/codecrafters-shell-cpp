#include <iostream>
#include <string>

void echo(std::string arg){
  std::cout<<arg<<std::endl;
}
void repl(){
  while(true){
    std::cout << "$ ";
    std::string command;
    std::getline(std::cin,command);
    if(command=="exit"){
      break;
    }
    if(command.substr(0,4)=="echo"){
      int l=4;
      while(command[l]==' '){
        l++;
      }
      echo(command.substr(l));
    }
    std::cout<<command<<": command not found"<<std::endl;
  }
}
int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  repl();
  
}

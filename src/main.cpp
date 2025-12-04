#include <iostream>
#include <string>

void repl(){
  while(true){
    std::cout << "$ ";
    std::string command;
    std::getline(std::cin,command);
    if(command=="exit"){
      break;
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

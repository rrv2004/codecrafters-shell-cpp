#include <iostream>
#include <string>

int main() {
  while (true) {

    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);
    if (command == "exit") {
      break;
    }
    else if (command.substr(0,4) == "echo" ) {
      std::cout << command.substr(5) << std::endl;
    }
    else if(command.substr(0,4)=="type"){
      std:: string arg=command.substr(5);
      if(arg=="exit"||arg=="echo"){
        std::cout<<command<<" is a shell builtin"<<std::endl;
      }
      else{
        std::cout<<command<<": not found"<<std::endl;
      }
    }
    else {
    std::cout << command << ": command not found" << std::endl;
    }
  }
}
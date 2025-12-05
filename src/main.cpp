#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <unistd.h>     // access()

// Split PATH by ':'
std::vector<std::string> split_path() {
  std::vector<std::string> dirs;

  char *path = getenv("PATH");
  if (!path) return dirs;

  std::stringstream ss(path);
  std::string dir;

  while (std::getline(ss, dir, ':'))
    dirs.push_back(dir);

  return dirs;
}

// Search PATH for executable
bool find_executable(const std::string &cmd, std::string &result) {
  auto dirs = split_path();

  for (const auto &dir : dirs) {
    std::string full = dir + "/" + cmd;

    // Exists?
    if (access(full.c_str(), F_OK) == 0) {
      // Executable?
      if (access(full.c_str(), X_OK) == 0) {
        result = full;
        return true;
      }
    }
  }
  return false;
}

int main() {
  while (true) {

    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);

    if (command == "exit") {
      break;
    }

    else if (command.substr(0, 4) == "echo") {
      std::cout << command.substr(5) << std::endl;
    }

    else if (command.substr(0, 4) == "type") {

      std::string arg = command.substr(5);

      // ---- Check builtin ----
      if (arg == "exit" || arg == "echo" || arg == "type") {
        std::cout << arg << " is a shell builtin" << std::endl;
      }
      else {
        // ---- Search PATH ----
        std::string path_result;

        if (find_executable(arg, path_result)) {
          std::cout << arg << " is " << path_result << std::endl;
        } else {
          std::cout << arg << ": not found" << std::endl;
        }
      }
    }

    else {
      std::cout << command << ": command not found" << std::endl;
    }
  }
}

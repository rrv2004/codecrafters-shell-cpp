#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>//getenv()
#include <unistd.h>// access(), fork(), execv(),chdir()
#include <sys/wait.h>// waitpid()
#include <limits.h>//PATH_MAX

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

// Split a command into tokens (cmd + args)
std::vector<std::string> tokenize(const std::string &line) {
  std::stringstream ss(line);
  std::string token;
  std::vector<std::string> tokens;

  while (ss >> token)
    tokens.push_back(token);

  return tokens;
}

int main() {
  while (true) {

    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "$ ";
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) continue;
    // Tokenize input
    auto tokens = tokenize(line);
    std::string cmd = tokens[0];
    // BUILTIN: exit
    if (cmd == "exit") {
      break;
    }
    // BUILTIN: echo
    else if (cmd == "echo") {
      // print everything after "echo"
      for (size_t i = 1; i < tokens.size(); i++) {
        std::cout << tokens[i];
        if (i + 1 < tokens.size()) std::cout << " ";
      }
      std::cout << std::endl;
    }
    // BUILTIN : pwd
    else if(cmd=="pwd"){
      char cwd[PATH_MAX];
      if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << std::endl;
      }
      else{
        perror("pwd");
      }
    }
    //BUILTIN: cd
    else if (cmd == "cd") {
    if (tokens.size() < 2) {
        continue;  // no argument for now
    }

    std::string dir = tokens[1];

    // ABSOLUTE PATH
    if (!dir.empty() && dir[0] == '/') {
        if (chdir(dir.c_str()) != 0) {
            std::cout << "cd: " << dir << ": No such file or directory" << std::endl;
        }
        continue;
    }
    // HOME PATH
    if(dir=="~"){
      char *path=getenv("HOME");
      chdir(path);
    }

    // RELATIVE PATH 
    // get cwd
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        perror("getcwd");
        continue;
    }

    // Build combined path: <cwd>/<dir>
    std::string full = std::string(cwd) + "/" + dir;

    // Try changing directory
    if (chdir(full.c_str()) != 0) {
        std::cout << "cd: " << dir << ": No such file or directory" << std::endl;
    }


    continue;
    } 
 
    // BUILTIN: type
    else if (cmd == "type") {

      if (tokens.size() < 2) continue;
      std::string arg = tokens[1];

      // Check builtins
      if (arg == "exit" || arg == "echo" || arg == "type"||arg=="pwd") {
        std::cout << arg << " is a shell builtin" << std::endl;
      }
      else {
        std::string path_result;

        if (find_executable(arg, path_result)) {
          std::cout << arg << " is " << path_result << std::endl;
        } else {
          std::cout << arg << ": not found" << std::endl;
        }
      }
    }
    
    // EXTERNAL COMMAND
    else {
      std::string path_result;
      // Search PATH for binary
      if
       (!find_executable(cmd, path_result)) {
        std::cout << cmd << ": command not found" << std::endl;
        continue;
      }

      // Build char* argv[] for execv
      std::vector<char*> argv;
      for (auto &s : tokens){
        argv.push_back(const_cast<char*>(s.c_str()));
        //const_cast converts const char* to char *(becomes modifiable pointer rather than a constant pointer)
      }
      argv.push_back(nullptr);

      pid_t pid = fork();

      if (pid == 0) {
        // Child process: execute program
        execv(path_result.c_str(), argv.data());
        perror("execv");  // Only runs if exec fails
        exit(1);
      }
      else {
        // Parent process: wait for child
        waitpid(pid, nullptr, 0);
      }
    }
  }
}

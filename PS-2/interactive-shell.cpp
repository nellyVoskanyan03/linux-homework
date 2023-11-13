#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

std::vector<std::string> splitString(const std::string& input) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;

    while (end < input.length()) {

        // Find the start of the next word
        while (start < input.length() && (input[start] == ' ' || input[start] == '\t')) {
            start++;
        }

        // Find the end of the word
        end = start;
        while (end < input.length() && input[end] != ' ' && input[end] != '\t') {
            end++;
        }

        if (start < input.length()) {
            tokens.push_back(input.substr(start, end - start));
            start = end;
        }
    }

    return tokens;
}

char* const* vectorToCharPointerArray(const std::vector<std::string>& vec) {
    char** arr = new char* [vec.size() + 1];  

    for (size_t i = 0; i < vec.size(); ++i) {
        arr[i] = strdup(vec[i].c_str());
    }

    arr[vec.size()] = nullptr;  
    return arr;
}

void executeCommand(const std::vector<std::string>& command) {
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
    }
    else if (child_pid == 0) {
        // Child process
 
        // Add the current directory to the PATH environment variable
        char* current_dir = get_current_dir_name();
        char* path_env = getenv("PATH");
        std::string new_path_env = "PATH=" + std::string(current_dir) + ":" + std::string(path_env);
        putenv(strdup(new_path_env.c_str()));

        if (command[0] == "silent") {
            if (command.size() < 2) {
                std::cerr << "Please provide the command" << std::endl;
                exit(1);
            }

            int pid = getpid();
            std::string log_file = std::to_string(pid) + ".log";
            int file_decorator = open(log_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
            if (file_decorator == -1) {
                std::cerr << "Error while editing .log file " << std::endl;
                exit(1);
            }
            dup2(file_decorator, STDOUT_FILENO);
            dup2(file_decorator, STDERR_FILENO);
            close(file_decorator);
        }
        
        char* const* argv = vectorToCharPointerArray(command);

        // Execute the command
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    }
    else { 
        // Parent process
        int status;
        waitpid(child_pid, &status, 0);
        if (WIFEXITED(status)) {
            std::cout << "Child process exited with status: " << WEXITSTATUS(status) << std::endl;
        }
    }
}

int main() {
    while (true) {
        std::string input;
        std::cout << "~ ";
        getline(std::cin, input);
        if (input == "exit") {
            break;
        }
        std::vector<std::string> command = splitString(input);
        if (!command.empty()) {
            executeCommand(command);
        }
    }

    return 0;
}
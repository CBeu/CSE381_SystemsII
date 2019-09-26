/* 
 * File:   beuerlcd_hw4.cpp
 * Author: Craig Beuerlein
 *
 * Copyright [2019] <Beuerlein>
 * 
 * This homework assignment was to create a custom shell for a user to type
 * basic commands into. A user can input echo, SERIAL, PARALLEL, exit, or type
 * a comment with '#' preceding it.
 */

#include <unistd.h>
#include <sys/wait.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
using strVec = std::vector<string>;

/**
 * This method checks to see if the exit case is met.
 * @param line The command being checked.
 * @return True if the command is exit, false otherwise.
 */
bool isExit(string line) {
    if (line == "exit") {
        return true;
    } else {
        return false;
    }
}

/**
 * This program checks to see if the command inputted should be ignored i.e. it
 * is a comment or a blank line.
 * @param line The command being checked to see if it is a comment or a blank 
 * line 
 * @return True if it is a blank line or comment. False otherwise.
 */
bool skip(string line) {
    if (line.length() < 1) {
        return true;
    } else if (line.at(0) == '#') {
        return true;
    } else {
        return false;
    }
}

/**
 * 
 * @param argList
 */
void executeCommand(strVec argList) {
    std::vector<char*> commands;
    for (size_t i = 0; i < argList.size(); i++) {
        commands.push_back(&argList[i][0]);
    }
    commands.push_back(nullptr);
    execvp(commands[0], &commands[0]);
}

/**
 * This method puts all the commands into a vector of strings and outputs the 
 * necessary text.
 * @param line the command being converted into a vector.
 * @return A vector of strings that is the command inputted by the user.
 */
strVec computeInput(string line) {
    strVec argsList;
    std::istringstream iStream(line);
    std::string word;
    while (iStream >> std::quoted(word)) {
        argsList.push_back(word);
    }
    std::cout << "Running:";
    for (size_t i = 0; i < argsList.size(); i++) {
        std::cout << " " + argsList[i];
    }
    std::cout << std::endl;\
    return argsList;
}

/**
 * This method checks to see if the pid is valid for a command to be executed.
 * @param argsList The vector of the command inputted by the user.
 * @return The pid of the command inputed. 
 */
int checkPid(strVec argsList) {
    const int pid = fork();
    if (pid == 0) {
        executeCommand(argsList);
    } else {
        return pid;
    }
    return pid;
}

/**
 * This method prints the exit code of the command that was executed by the 
 * shell.
 * @param pid The pid to be checked for the valid exit code.
 */
void printExitCode(int pid) {
    int exitCode;
    waitpid(pid, &exitCode, 0);
    std::cout << "Exit code: " << exitCode << std::endl;
}

/**
 * This method runs the commands from the valid file in a serial process.
 * @param file The file to be scanned for commands to be run in serial.
 */
void runSerial(std::string file) {
    std::ifstream input = std::ifstream(file);
    std::string line = "";
    while (std::getline(input, line)) {
        if (isExit(line)) {
            break;
        } else if (!skip(line)) {
            strVec argsList = computeInput(line);
            printExitCode(checkPid(argsList));
        }
    }
}

/**
 * This method puts all the pids of the commands run in parallel into a vector
 * @param pids The vector of pids of commands run in parallel.
 * @param pid The pid that will be added to the vector named pids
 * @return The vector of pids named pids.
 */
std::vector<int> getVecPids(std::vector<int> pids, int pid) {
    pids.push_back(pid);
    return pids;
}

/**
 * This method reads a file of commands and executes those commands in parallel.
 * @param file The file of commands to be run in parallel.
 */
void runParallel(std::string file) {
    std::ifstream input = std::ifstream(file);
    std::string line = "";
    std::vector<int> pids;
    while (std::getline(input, line)) {
       if (isExit(line)) {
           break;
        } else if (!skip(line)) {  
            strVec argsList = computeInput(line);
            pids = getVecPids(pids, checkPid(argsList));
        } 
    }
    while (!pids.empty()) {
        int pid = pids.back();
        pids.pop_back();
        printExitCode(pid);
    }
}

int main(int argc, char** argv) {
    while (1 == 1) {
        string input;
        getline(cin, input);
        if (isExit(input)) {
            break;
        }
        if (!skip(input)) {
                strVec argsList = computeInput(input);
                std::vector<int> pidList;
                if (argsList[0] == "SERIAL" || argsList[0] == "PARALLEL") {
                    if (argsList[0] == "SERIAL") {
                        runSerial(argsList[1]);
                    } else if (argsList[0] == "PARALLEL") {
                        runParallel(argsList[1]);
                    }
                } else {
                    printExitCode(checkPid(argsList));
                }
        }
    }
    return 0;
}

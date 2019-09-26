/* 
 * File:   beuerlcd_hw3.cpp
 * Author: Craig Beuerlein
 *
 * Copyright [2019] <Beuerlein>
 * 
 * This program will take a file and a starting PID from the command line. The
 *  program will then begin to process the file and print a process tree of the 
 * process ids, the parent ids, and commands in the file. The program will use 
 * "unm" instead of std::unordered_map<std::string, std::string> to increase
 * the readability of the program.
 */

#include "beuerlcd_hw3.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>

using namespace std;
using unm = std::unordered_map<std::string, std::string>;

/**
 * Empty constructor
 */
processHier::processHier() {
};

/**
 * This method takes a input file stream and reads each line from the 
 * proc_info1.txt file. The method then pulls out all relevant information from
 * each line of the stream by using a istringstream. After each line of the file
 * is read, the unordered maps have the parent process id and the command added
 * to their respective maps indexed by the process id.
 * @param ptp The unordered map that has the process id as an index and the 
 * parent process id as the value.
 * @param ptc The unordered map that has the process id as an index and the 
 * command as the value.
 * @param input The input file stream of the file specified in the command line 
 * argument.
 */
void processHier::readIS(unm& ptp, unm& ptc, std::ifstream& input) {
    std::string line = "";
    std::string pid = "";
    std::string ppid = "";
    std::string command = "";
    std::string sub = "";
    std::getline(input, line);
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        iss >> pid >> pid >> ppid >> command >> command >> command >> command;
        command = ""; sub ="";
        iss >> sub;
        command+=sub;
        while (iss >> sub) {
            command += " " + sub;
        }
        ptp[pid] = ppid;
        ptc[pid] = command;
    }
}

/**
 * This method begins the process of outputting the full hierarchy of the 
 * processes for a given PID. Once the method has printed the user specified PID
 *  and the main columns of data that will be printed, the method invokes 
 * printEnt and sends it the unordered maps used for storing the parent process
 *  id and the command.
 * @param ptp The unordered map that has the process id as an index and the 
 * parent process id as the value.
 * @param ptc The unordered map that has the process id as an index and the 
 * command as the value.
 * @param PID The process id the user specified in the command line argument.
 */
void processHier::output(unm& ptp, unm& ptc, std::string PID) {
    std::cout << "Process tree for PID: " << PID << std::endl;
    std::cout << "PID\tPPID\tCMD" << std::endl;
    std::string fin = "";
    printEnt(ptp, ptc, PID, fin);
}

/**
 * This method is a recursive helper method for the method output. The method
 *  checks the PID value sent to it by either the output method or from itself 
 * through recursion. If the PID equals 1, the method will print out the PID, 
 * the value found at index PID in the unordered map for the parent process and 
 * the unordered map for the commands. If the value does not equal 1, the method
 *  will print the same data but it will also begin a recursion loop until the 
 * PID send to the method equals 1.
 * @param ptp The unordered map that has the process id as an index and the 
 * parent process id as the value.
 * @param ptc The unordered map that has the process id as an index and the 
 * command as the value.
 * @param PID The process id the user specified in the command line argument.
 * @param fin This is a string that will ensure that the values being printed 
 * are printed in the correct order.
 */
void processHier::printEnt(unm& ptp, unm& ptc, std::string PID, 
        std::string fin) {
    if (PID == "1") {
        std::cout << PID << "\t" << ptp[PID] << "\t " << ptc[PID] << std::endl 
                << fin;
       
    } else {
       fin = PID + "\t" + ptp[PID] + "\t " + ptc[PID] + "\n" + fin;
       printEnt(ptp, ptc, ptp[PID], fin);
    }
}

/**
 * This main method creates a processHier (short for process hierarchy) object, 
 * the two unordered maps that will be used in the program along with a 
 * file stream and setting the starting PID. The main method will then invoke 
 * the readIS method and the output method.
 * @param argc
 * @param argv
 * @return 0 when all desired information from the input file has been printed.
 */
int main(int argc, char** argv) {
    processHier ph = processHier();
    std::ifstream input = std::ifstream(argv[1]);
    unm ptp = unm();
    unm ptc = unm();
    std::string PID = argv[2];
    ph.readIS(ptp, ptc, input);
    ph.output(ptp, ptc, PID);
    return 0;
}

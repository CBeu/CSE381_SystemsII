/* 
 * File:   beuerlcd_hw3.h
 * Author: Craig Beuerlein
 *
 * Copyright [2019] <Beuerlein>
 * 
 * This is a header file that will be expanded upon by the beuerlcd_hw3.cpp file
 * The processHier(short for process hierarchy) will be an object that will take
 *  a file and parse the relevant information from it.
 */

#ifndef BEUERLCD_HW3_H
#define BEUERLCD_HW3_H

#include <string>
#include <unordered_map>
using unm  = std::unordered_map<std::string, std::string>;

/**
 * Class processHier has 3 methods associated with it. 2 are public methods and 
 * one is a private method. The constructor requires no inputs. 
 */
class processHier {
public:
    processHier();
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
    void readIS(unm& ptp, unm& ptc, std::ifstream& input);
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
    void output(unm& ptp, unm& ptc, std::string PID);

private:
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
    void printEnt(unm& ptp, unm& ptc, std::string PID, std::string fin);
};

#endif /* BEUERLCD_HW3_H */


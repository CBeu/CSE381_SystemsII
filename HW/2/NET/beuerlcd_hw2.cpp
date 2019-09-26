
/* 
 * File:   beuerlcd_hw2.cpp
 * Author: Craig Beuerlein
 *
 * Copyright [2019] <Beuerlein>
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>      
#include <unordered_map>

using namespace std;

int getID(std::string line, int ID) {
    std::string::size_type sz;
    std::string build = "";
    int cCount = 0;
    for (auto x : line) {
        if (x == ':') {
            if (cCount == 2) {
                ID = std::stoi(build, &sz);
            }
            build = "";
            cCount++;
        } else {
            build = build + x;
        }
    }
    return ID;
}

std::string getMainID(std::string line, std::string ID) {
    std::string build = "";
    int cCount = 0;
    for (auto x : line) {
        if (x == ':') {
            if (cCount == 0) {
                ID = build;
            }
            build = "";
            cCount++;
        } else {
            build = build + x;
        }
    }
    return ID;
}

std::unordered_map<int, std::string>
readPasswd(std::unordered_map<int, std::string> umap) {
    std::ifstream passwd;
    passwd.open("passwd");
    std::string line = "";
    std::string loginID = "";
    int uid = 0;
    while (getline(passwd, line)) {
        uid = getID(line, uid);
        loginID = getMainID(line, loginID);
        umap[uid] = loginID;
    }
    passwd.close();
    return umap;
}

std::unordered_map<int, std::string>
readGroups(std::unordered_map<int, std::string> umap) {
    std::ifstream groups;
    groups.open("groups");
    std::string groupID = "";
    int gid = 0;
    std::string line;
    while (getline(groups, line)) {
        gid = getID(line, gid);
        groupID = getMainID(line, groupID);
        umap[gid] = groupID;
    }
    groups.close();
    return umap;
}

std::string response(int gid, std::unordered_map<int, std::string>
        umapPasswd, std::unordered_map<int, std::string> umapGroups) {
    std::string response = "";
    if (umapGroups.count(gid) != 0) {
        response = std::to_string(gid) + " = "
                + umapGroups.at(gid) + ":";
        if (umapPasswd.count(gid) != 0 && gid == 0) {
            response = response + " " + umapPasswd.at(gid) + "(" 
                    + std::to_string(gid) + ")";
        }
    } else {
        response = std::to_string(gid) +" = Group not found.";
    }
    return response;
}

int main(int argc, char** argv) {
    std::unordered_map<int, std::string> umapPasswd;
    std::unordered_map<int, std::string> umapGroups;
    std::unordered_map<int, std::string> umapUsrs;
    umapPasswd = readPasswd(umapPasswd);
    umapGroups = readGroups(umapGroups);
    for (int i = 1; i < argc; i++) {
        int gid = std::atoi(argv[i]);
        std::cout << response(gid, umapPasswd, umapGroups) << std::endl;
    }
    return 0;
}

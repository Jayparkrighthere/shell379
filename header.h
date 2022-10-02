#ifndef __SHELL379_H_
#define __SHELL379_H_

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <cstring>
#include <vector>

using namespace std;

const char DELIM[4] = " \n";
const int LINE_LENGTH = 100;  // Max # of characters in an input line
const int MAX_ARGS = 7;       // Max number of arguments to a command
const int MAX_LENGTH = 20;    // Max # of characters in an argument
const int AX_PT_ENTRIES = 32; // Max entries in the Process Table

void takesInFile(char *args[], char *inputFile, char *outputFile, bool *isUsingFile);
int splitCommands(char *inputLine, char *args[]);
void readLine(char *line);
void removeFileNamesFromArgs(char *args[]);
void handleSigchl(int sig);
void printUsage(string printLine);
int execBuiltInCmd(char *args[], char *line, int bltInCode);
int execCMD(char *args[], char *line, char *inputFile, char *outputFile, bool *isUsingFiles, bool isBackGround);
int BuiltInCMDCode(char *args[]);

class Process
{
public:
    Process(pid_t pid, char command[]);

    pid_t pid;
    char *command;

private:
};
class ProcessTable
{
public:
    vector<Process> pcb;
    int getTableSize();
    void addProcessToTable(Process p);
    void removeProcessFromTable(Process p);
    void printProcessTable();
    void getStatusByPid(pid_t pid);
    void clearTable();

private:
};

#endif

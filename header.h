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
#include <string>
#include <regex>
#include <sys/resource.h>

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
int execBuiltInCmd(char *args[], string cmd, int bltInCode);
void killProcess(pid_t pid);
int execCMD(char *args[], string cmd, char *inputFile, char *outputFile, bool *isUsingFiles, bool isBackGround);
int BuiltInCMDCode(char *args[]);

/*
    Description:Each process hold a pid, and
    a command used that was used to creat this process
*/
class Process
{
public:
    Process(pid_t pid, string command);

    pid_t pid;
    string command;

private:
};

/*
    Description: process table keep tracks
        of all the process created by our shell.
*/
class ProcessTable
{
public:
    vector<Process> pcb; // record all process in R,S,T states
    ProcessTable();
    int getTableSize();
    void addProcessToTable(Process p);
    void removeProcessFromTable(pid_t pid);
    void killAllProcessFromTable();
    void printProcessTable();
    bool printStatusByPid(pid_t pid, string command, int entrCount);
    void clearTable();
    void suspendProcess(pid_t pid);
    void resumeProcess(pid_t pid);
    int getNumActive();
    void upDateTable();
    void removeSuspProcess(pid_t pid);
    bool isSuspendedProcess(pid_t pid);

private:
    int activeNum;
    vector<pid_t> completedProcesses; // record finish process, but yet removed from pcb
    vector<pid_t> suspendedProcesses; // record all process in T state.
};

#endif

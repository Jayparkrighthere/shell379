#include "header.h"

Process ::Process(pid_t pid, string command)
{
    this->pid = pid;
    this->command = command;
}
#include "header.h"

Process ::Process(pid_t pid, char command[])
{
    this->pid = pid;
    this->command = command;
}
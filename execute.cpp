#include "header.h"
ProcessTable processTable = ProcessTable();

void handleSigchl(int sig)
{

    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0)
    {
    }
}

void killProcess(pid_t pid)
{

    if (kill(pid, SIGKILL) < 0)
    {
        perror("Kill");
    }
}

void printUsage(string printLine)
{
    struct rusage usage;
    getrusage(RUSAGE_CHILDREN, &usage);
    cout << printLine << endl;
    cout << "User time =        " << usage.ru_utime.tv_sec << endl;
    cout << "Sys  time =        " << usage.ru_stime.tv_sec << endl;
}
int execBuiltInCmd(char *args[], string cmd, int bltInCode)
{
    switch (bltInCode)
    {
    // exit
    case 0:
        processTable.killAllProcessFromTable();
        processTable.clearTable();
        printUsage("Resources used");
        _exit(0);
    // jobs
    case 1:
        processTable.printProcessTable();
        printUsage("Completed processes:");
        break;
    // kill
    case 2:
        if ((args[1]) == NULL)
        {
            cout << "kill : not enought arguments" << endl;
            cout << "Usage: kill <pid>" << endl;
            return 1;
        }
        killProcess(atoi(args[1]));
        processTable.removeProcessFromTable(atoi(args[1]));

        break;
    // resume
    case 3:
        if ((args[1]) == NULL)
        {
            cout << "Resume : not enought arguments" << endl;
            cout << "Usage: resume <pid>" << endl;
            return 1;
        }
        if (kill(atoi(args[1]), SIGCONT) < 0)
        {
            perror("Kill -SIGCONT");
        }
        processTable.resumeProcess(atoi(args[1]));
        break;
    // sleep
    case 4:
        if ((args[1]) == NULL)
        {
            cout << "sleep : not enought arguments" << endl;
            cout << "Usage: sleep <int>" << endl;
            return 1;
        }
        sleep(atoi(args[1]));
        break;
    // suspend
    case 5:
        if ((args[1]) == NULL)
        {
            cout << "suspend : not enought arguments" << endl;
            cout << "usage: suspend <pid>" << endl;
            return 1;
        }
        if (kill(atoi(args[1]), SIGSTOP) < 0)
        {
            perror("Kill -SIGSTOP");
        }
        processTable.suspendProcess(atoi(args[1]));
        break;
    // wait
    case 6:
        if ((args[1]) == NULL)
        {
            cout << "wait : not enought arguments" << endl;
            cout << "usage: wait <pid>" << endl;
            return 1;
        }
        if (processTable.isSuspendedProcess(atoi(args[1])))
        {
            cout << "could not wait on a suspended process. resume it first." << endl;
            return 1;
        }
        else
        {
            int status;
            waitpid(atoi(args[1]), &status, 0);
            processTable.removeProcessFromTable(atoi(args[1]));
        }

        break;
    };
    return 1;
}

int execCMD(char *args[], string cmd, char *inputFile, char *outputFile, bool *isUsingFiles, bool isBackGround)
{
    int inputFileDescriptor, outputFileDescriptor;

    // create new process here
    pid_t pid;
    int status;
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
    }
    // Child process
    else if (pid == 0)
    {
        // if reading from file, change stdin descriptor to file descriptor.
        if (isUsingFiles[0])
        {
            inputFileDescriptor = open(inputFile, O_RDONLY, S_IRWXU);
            if (inputFileDescriptor < 0)
            {
                perror("input file open failed");
                return 0;
            }
            dup2(inputFileDescriptor, STDIN_FILENO);
            close(inputFileDescriptor);
        }
        // Same for output file.
        if (isUsingFiles[1])
        {
            outputFileDescriptor = open(outputFile, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
            if (outputFileDescriptor < 0)
            {
                perror("output file open failed");
                return 0;
            }
            dup2(outputFileDescriptor, STDOUT_FILENO);
            close(outputFileDescriptor);
        }
        if (execvp(args[0], args) < 0)
        {
            perror("execvp");
            return 0;
        }
    }
    // parent process
    else
    {

        if (!isBackGround)
        {
            waitpid(pid, &status, 0);
        }
        else
        {
            if (processTable.getTableSize() >= AX_PT_ENTRIES)
            {
                cout << "exceeding max entries pcb can hold" << endl;
                cout << "can not run this process in background" << endl;
                waitpid(pid, &status, 0);
                return 1;
            }
            Process p = Process(pid, cmd);
            processTable.addProcessToTable(p);
            signal(SIGCHLD, handleSigchl);
        }
    }
    return 1;
}

int BuiltInCMDCode(char *args[])
{
    const char *builtInCMD[8] = {"exit",
                                 "jobs",
                                 "kill",
                                 "resume",
                                 "sleep",
                                 "suspend",
                                 "wait", NULL};
    int index = 0;
    while (builtInCMD[index] != NULL)
    {
        if (strcmp(args[0], builtInCMD[index]) == 0)
            return index;
        index++;
    }
    return -1;
}
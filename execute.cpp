#include "header.h"
ProcessTable processTable = ProcessTable();

void handleSigchl(int sig)
{
    // http: // www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
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
    /**
     * exit     0
     * jobs     1
     * kill     2
     * resume   3
     * sleep    4
     * suspend  5
     * wait     6
     * */
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
        processTable.resumeProcess();
        // Do any thing in the process table?
        break;
    // sleep
    case 4:
        cout << "executing sleep" << endl;
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
        processTable.suspendProcess();
        // Do any thing in the process table?
        break;
    // wait
    case 6:
        if ((args[1]) == NULL)
        {
            cout << "wait : not enought arguments" << endl;
            cout << "usage: wait <pid>" << endl;
            return 1;
        }
        int status;
        waitpid(atoi(args[1]), &status, 0);
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
            outputFileDescriptor = open(outputFile, O_CREAT | O_WRONLY, S_IRWXU);
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

        printf("this shouldn't print out\n"); // because it exect
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
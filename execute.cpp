#include "header.h"
ProcessTable processTable;

void handleSigchl(int sig)
{
    // http: // www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0)
    {
    }
}

void killProcess(char *args[], char *line)
{

    if (kill(atoi(args[1]), SIGKILL) < 0)
    {
        perror("Kill");
    }
    Process rp = Process(atoi(args[1]), line);
    processTable.removeProcessFromTable(rp);
}
void printUsage(string printLine)
{
    struct rusage usage;
    getrusage(RUSAGE_CHILDREN, &usage);
    cout << printLine << endl;
    cout << "User time =        " << usage.ru_utime.tv_sec << endl;
    cout << "Sys  time =        " << usage.ru_stime.tv_sec << endl;
}
int execBuiltInCmd(char *args[], char *line, int bltInCode)
{
    cout << "exectuing execbuildin cmd" << endl;
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
            cout << "Usage kill <pid>" << endl;
            return 1;
        }
        killProcess(args, line);
        break;
    // resume
    case 3:
        break;
    // sleep
    case 4:
        cout << "executing sleep" << endl;
        if ((args[1]) == NULL)
        {
            cout << "sleep : not enought arguments" << endl;
            cout << "Usage sleep <int>" << endl;
            return 1;
        }
        sleep(atoi(args[1]));
        break;
    // suspend
    case 5:
        break;
    // wait
    case 6:
        break;
    };
    return 1;
}

int execCMD(char *args[], char *line, char *inputFile, char *outputFile, bool *isUsingFiles, bool isBackGround)
{
    cout << "exectuing normal cmd" << endl;
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
            cout << "try open input file" << endl;
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
            cout << "this is parent. child process not running in background" << endl;
            waitpid(pid, &status, 0);
        }
        else
        {
            Process p = Process(pid, line);
            processTable.addProcessToTable(p);
            signal(SIGCHLD, handleSigchl);
        }
    }
    return 1;
}

int BuiltInCMDCode(char *args[])
{
    char *builtInCMD[8] = {"exit",
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
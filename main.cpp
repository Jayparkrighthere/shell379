#include "header.h"
#define __SHELL379_H_

// TODO: test if ps returns the command you used for you shell379 programs
// TODO: processConroltable class.

int execBuiltInCmd(char *args[])
{
    if (strcmp(args[0], "sleep") == 0)
    {
        cout << "executing sleep" << endl;
        int sec = args[1] == NULL ? 0 : atoi(args[1]);
        sleep(sec);
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }
    return 1;
}

int execCMD(char *args[], char *inputFile, char *outputFile, bool *isUsingFiles, bool isBackGround)
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
        printf("Parent process finished with child process: %d\n", pid);
    }
    return 1;
}

bool isBuiltInCMD(char *args[])
{
    char *builtInCMD[8] = {"exit",
                           "jobs",
                           "kill",
                           "resume",
                           "sleep",
                           "suspend",
                           "wait", NULL};
    int index;
    while (builtInCMD[index] != NULL)
    {
        if (strcmp(args[0], builtInCMD[index]) == 0)
            return true;
        index++;
    }
    return false;
}

int main(int argc, char *argv[])
{
    int status;
    char line[LINE_LENGTH];
    char *tokens[MAX_ARGS];
    char inputFile[MAX_LENGTH];
    char outputFile[MAX_LENGTH];
    int argsNum;
    signal(SIGCHLD, SIG_IGN);

    // ensure no zombie child process in our program
    bool isBackGround = false;
    bool isUsingFile[2];
    do
    {
        printf(">>");

        readLine(line);
        cout << "after reading line" << endl;
        // strcpy(line, "grep you <input.txt >output.txt");
        argsNum = splitCommands(line, tokens);
        cout << "after splitCommand" << endl;

        takesInFile(tokens, inputFile, outputFile, isUsingFile);
        cout << "after takesInFile" << endl;
        isBackGround = strcmp(tokens[argsNum - 1], "&") == 0;
        cout << "after Isbackground" << endl;

        if (isUsingFile[0] || isUsingFile[1])
        {
            removeFileNamesFromArgs(tokens);
        }
        if (isBuiltInCMD(tokens))
        {
            status = execBuiltInCmd(tokens);
        }
        else
        {
            status = execCMD(tokens, inputFile, outputFile, isUsingFile, isBackGround);
        }

        // clean up files
        memset(inputFile, 0, sizeof inputFile);
        memset(outputFile, 0, sizeof outputFile);

        cout << "status: " << status << endl;

        // free up spaces?
    } while (status);
    return 0;
}
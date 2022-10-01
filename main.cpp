
#include "header.h"
#define __SHELL379_H_

int executeCommand(char *args[], char *inputFile, char *outputFile, bool *isUsingFiles)
{

    int inputFileDescriptor, outputFileDescriptor;

    if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }
    // create new process here
    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        // Child process
        // if reading from file, turn stdin descriptor to file descriptor.
        if (isUsingFiles[0])
        {
            cout << "using an input file!!!" << endl;
            inputFileDescriptor = open(inputFile, O_CREAT | O_RDONLY, S_IRWXU);
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
            cout << "using an output file!!!" << endl;

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
    else
    {
        wait(NULL);
        printf("Parent process finished with child process: %d\n", pid);
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int status;
    char line[LINE_LENGTH];
    char *tokens[MAX_ARGS];
    char inputFile[MAX_LENGTH];
    char outputFile[MAX_LENGTH];

    bool isUsingFile[2];
    do
    {
        printf(">>");

        readLine(line);
        // strcpy(line, "grep you <input.txt >output.txt");
        splitCommands(line, tokens);
        takesInFile(tokens, inputFile, outputFile, isUsingFile);
        status = executeCommand(tokens, inputFile, outputFile, isUsingFile);

        // clean up files
        memset(inputFile, 0, sizeof inputFile);
        memset(outputFile, 0, sizeof outputFile);

        cout << "status: " << status << endl;

        // free up spaces?
    } while (status);
    return 0;
}
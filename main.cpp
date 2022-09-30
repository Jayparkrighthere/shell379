#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

const char DELIM[4] = " \n";
const int LINE_LENGTH = 100;  // Max # of characters in an input line
const int MAX_ARGS = 7;       // Max number of arguments to a command
const int MAX_LENGTH = 20;    // Max # of characters in an argument
const int AX_PT_ENTRIES = 32; // Max entries in the Process Table

bool *takesInFile(char *args[], char *inputFile, char *outputFile)
{
    int i = 1;
    bool isUsingFiles[2];
    isUsingFiles[0] = false;
    isUsingFiles[1] = false;

    while (args[i] != NULL)
    {

        if (args[i][0] == '<')
        {
            isUsingFiles[0] = true;
            char *fileTemp = args[i];
            (fileTemp)++;
            strcpy(inputFile, fileTemp);
        }
        else if (args[i][0] == '>')
        {
            isUsingFiles[1] = true;
            char *fileTemp = args[i];
            (fileTemp)++;
            strcpy(outputFile, fileTemp);
        }
        i++;
        // we should end the loop actaully
    };
    return isUsingFiles;
}

int executeCommand(char *args[], char *inputFile, char *outputFile, bool isUsingFiles[])
{
    cout << "<<<<<<<<<<<<<<<<<<<inside executeCommand<<<<<<<<<<<<<<<<" << endl;
    cout << "isUsingFiles[0]: " << isUsingFiles[0] << endl;
    cout << "isUsingFiles[1]: " << isUsingFiles[1] << endl;

    int inputFileDescriptor, outputFileDescriptor;

    if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }
    pid_t pid;

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

// split the input line by spaces
void splitCommands(char *inputLine, char *args[])
{
    char *arg;
    int i = 0;
    arg = strtok(inputLine, DELIM);
    while (arg != NULL)
    {
        args[i] = arg;
        i++;
        arg = strtok(NULL, DELIM);
    }
    args[i] = NULL; // marks end of array
}

// This function reads input line
void readLine(char *line)
{
    char *inputLine = NULL;
    size_t linecap = 100 * sizeof(char);
    if (getline(&inputLine, &linecap, stdin) < 0)
    {
        perror("getline");
    }
    strcpy(line, inputLine);
}

int main(int argc, char *argv[])
{
    int status;
    char line[LINE_LENGTH];
    char *tokens[MAX_ARGS];
    char inputFile[MAX_LENGTH];
    char outputFile[MAX_LENGTH];
    bool isInputFromFile = false;
    bool isOutputToFile = false;
    bool *isUsingFile;
    // do
    // {
    printf(">>");
    isInputFromFile = false;
    isOutputToFile = false;
    // readLine(line);
    strcpy(line, "pwd >output.txt");
    splitCommands(line, tokens);
    isUsingFile = takesInFile(tokens, inputFile, outputFile);

    status = executeCommand(tokens, inputFile, outputFile, isUsingFile);

    // clean up files
    memset(inputFile, 0, sizeof inputFile);
    memset(outputFile, 0, sizeof outputFile);

    cout << "status: " << status << endl;

    // free up spaces?
    // } while (status);
    return 0;
}
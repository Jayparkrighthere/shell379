#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;

const char DELIM[4] = " \n";

int executeCommand(char **args)
{
    // cout << "args[0]" << args[0] << endl;
    // cout << "args[1]" << args[1] << endl;
    // cout << "args[2]" << args[2] << endl;

    if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }
    int pid;
    pid = fork();
    // char *myargs[4];
    // char *env[] = {NULL};
    // myargs[0] = strdup("pwd"); // program: "wc" (word count)
    // myargs[1] = strdup("-P");  // argument: print the word counts
    // myargs[2] = NULL;          // marks end of array

    if (pid < 0)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) < 0)
        {
            perror("execvp");
        }
        printf("this shouldn't print out\n");
    }
    else
    {
        wait(NULL);
        printf("Parent process finished with child process: %d\n", pid);
    }
    return 1;
}

// split the input line by spaces
char **splitCommands(char *line)
{
    char *arg;
    char **args = new char *[100];
    int i = 0;
    arg = strtok(line, DELIM);
    while (arg != NULL)
    {
        args[i] = arg;
        i++;
        arg = strtok(NULL, DELIM);
    }
    args[i] = NULL; // marks end of array
    return args;
}

// This function reads input line
char *readLine()
{
    char *line = NULL;
    size_t linecap = 0;
    if (getline(&line, &linecap, stdin) < 0)
    {
        perror("getline");
    }
    return line;
}

int main(int argc, char *argv[])
{
    int status;
    char *line;
    char **tokens;
    do
    {
        printf(">>");
        line = readLine();
        tokens = splitCommands(line);
        status = executeCommand(tokens);
        cout << "status: " << status << endl;

        delete[] line;
        delete[] tokens;

    } while (status);
    return 0;
}
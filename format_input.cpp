#include "header.h"
void removeFileNamesFromArgs(char *args[])
{
    // remove args with input or output file names
    char *newArgs[MAX_ARGS];
    int indexOldArgs = 0;
    int indexNewArgs = 0;
    while (args[indexOldArgs] != NULL)
    {

        if (args[indexOldArgs][0] != '<' && args[indexOldArgs][0] != '>')
        {
            newArgs[indexNewArgs] = args[indexOldArgs];
            indexNewArgs++;
        };
        indexOldArgs++;
    };
    newArgs[indexNewArgs] = NULL;
    int index = 0;
    while (newArgs[index] != NULL)
    {
        args[index] = newArgs[index];
        index++;
    };
    args[index] = NULL;
}

void takesInFile(char *args[], char *inputFile, char *outputFile, bool *isUsingFile)
{

    isUsingFile[0] = false;
    isUsingFile[1] = false;

    // get input or output file name from args
    int i = 1;
    while (args[i] != NULL)
    {

        if (args[i][0] == '<' && !isUsingFile[0])
        {
            isUsingFile[0] = true;
            char *fileTemp = args[i];
            (fileTemp)++;
            strcpy(inputFile, fileTemp);
        }
        else if (args[i][0] == '>' && !isUsingFile[1])
        {
            isUsingFile[1] = true;
            char *fileTemp = args[i];
            (fileTemp)++;
            strcpy(outputFile, fileTemp);
        }
        i++;
    };
}

int splitCommands(char *inputLine, char *args[])
{
    // split the input line by spaces
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
    return i;
}

void readLine(char *line)
{
    // read from input line
    char *inputLine = NULL;
    size_t linecap = 100 * sizeof(char);
    clearerr(stdin);
    if (getline(&inputLine, &linecap, stdin) < 0)
    {
        perror("getline");
    }
    strcpy(line, inputLine);
}
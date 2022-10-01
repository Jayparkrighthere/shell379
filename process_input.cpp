#include "header.h"

void takesInFile(char *args[], char *inputFile, char *outputFile, bool *isUsingFile)
{
    int i = 1;

    isUsingFile[0] = false;
    isUsingFile[1] = false;

    // get input or output file name from args
    while (args[i] != NULL)
    {

        if (args[i][0] == '<')
        {
            cout << "input args contains <" << endl;
            isUsingFile[0] = true;
            char *fileTemp = args[i];
            (fileTemp)++;
            strcpy(inputFile, fileTemp);
        }
        else if (args[i][0] == '>')
        {
            cout << "input args contains >" << endl;

            isUsingFile[1] = true;
            char *fileTemp = args[i];
            (fileTemp)++;
            strcpy(outputFile, fileTemp);
        }
        if (isUsingFile[0] && isUsingFile[1])
            break;
        i++;
        // we should end the loop actaully
    };
    // remove args with input or output file names
    if (isUsingFile[0] || isUsingFile[1])
    {
        char *newArgs[MAX_ARGS];
        int indexOldArgs = 0;
        int indexNewArgs = 0;
        while (args[indexOldArgs] != NULL)
        {

            if (args[indexOldArgs][0] != '<' && args[indexOldArgs][0] != '>')
            {
                cout << "not equal < or >!!" << endl;
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
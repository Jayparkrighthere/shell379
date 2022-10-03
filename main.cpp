#include "header.h"
#define __SHELL379_H_
using namespace std;

int main(int argc, char *argv[])
{
    int status;
    char line[LINE_LENGTH];
    char *tokens[MAX_ARGS + 1];
    char inputFile[MAX_LENGTH];
    char outputFile[MAX_LENGTH];
    int argsNum;

    bool isBackGround = false;
    bool isUsingFile[2];
    do
    {
        printf(">>");
        readLine(line);
        // convert to string so that it could be pass in to Prcess constructor
        string lineStr(line);
        argsNum = splitCommands(line, tokens);

        takesInFile(tokens, inputFile, outputFile, isUsingFile);

        isBackGround = strcmp(tokens[argsNum - 1], "&") == 0;
        if (isBackGround)
        {
            // remove & arg
            tokens[argsNum - 1] = NULL;
        }
        if (isUsingFile[0] || isUsingFile[1])
        {
            // remove arguments that contains file name.
            removeFileNamesFromArgs(tokens);
        }
        int bltInCode = BuiltInCMDCode(tokens);
        if (bltInCode < 0)
        {
            status = execCMD(tokens, lineStr, inputFile, outputFile, isUsingFile, isBackGround);
        }
        else
        {
            status = execBuiltInCmd(tokens, lineStr, bltInCode);
        }

        // clean up files
        memset(inputFile, 0, sizeof inputFile);
        memset(outputFile, 0, sizeof outputFile);

    } while (status);
    return 0;
}
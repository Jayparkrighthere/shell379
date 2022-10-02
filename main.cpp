#include "header.h"
#define __SHELL379_H_
#include <cstring>
using namespace std;
// TODO: test what you have on lab machine first! if error, maybe from prcoess.command?

int main(int argc, char *argv[])
{
    int status;
    char line[LINE_LENGTH];
    char *tokens[MAX_ARGS + 1];
    char inputFile[MAX_LENGTH];
    char outputFile[MAX_LENGTH];
    int argsNum;

    // ensure no zombie child process in our program
    bool isBackGround = false;
    bool isUsingFile[2];
    do
    {
        printf(">>");

        readLine(line);
        string lineStr(line);
        // strcpy(line, "sleep 3");
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
            cout << "removing file names" << endl;
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

        cout << "status: " << status << endl;

        // free up spaces?
    } while (status);
    return 0;
}
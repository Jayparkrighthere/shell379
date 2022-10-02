#include "header.h"
// TODO: parse the values returned from ps
void ProcessTable ::addProcessToTable(Process p)
{
    cout << "adding process to table:" << endl;
    cout << "pid = " << p.pid << " coammnd = " << p.command << endl;
    pcb.push_back(p);
}

void ProcessTable ::removeProcessFromTable(Process p)
{
    cout << "clearing all the vectors" << endl;
    for (vector<Process>::iterator it = pcb.begin(); it != pcb.end(); ++it)
    {
        if (it->pid == p.pid)
        {
            pcb.erase(it);
            break;
        }
    }
    pcb.clear();
}
void ProcessTable::getStatusByPid(pid_t pid)
{
    char *psArgs[5];
    char pidBuffer[1024];
    char pStatusBuffer[1024];
    FILE *fp;

    sprintf(pidBuffer, "%d", pid);
    cout << "pid char array is" << pidBuffer << endl;

    // char str1[100] = "ps -p ";
    // char str2[20] = " -o pid,state,time";
    // strcat(str1, pidBuffer);
    // strcat(str1, str2);
    psArgs[0] = "ps ";
    psArgs[1] = "-p ";
    psArgs[2] = pidBuffer;
    psArgs[3] = " -o ";
    psArgs[4] = "pid,state,time";
    psArgs[5] = NULL; // marks end of args
    fp = popen(*psArgs, "r");
    // cout << "str1 is " << str1 << endl;
    // fp = popen(str1, "r");

    while (fgets(pStatusBuffer, 1024, fp) != NULL)
    {
        cout << "printing from ps: " << endl;
        cout << pStatusBuffer << endl;
    }
    pclose(fp);
}

void ProcessTable::clearTable()
{
    pcb.clear();
}

void ProcessTable::printProcessTable()
{
    cout << "Running processes:" << endl;
    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
    for (vector<Process>::iterator it = pcb.begin(); it != pcb.end(); ++it)
    {
        cout << "pid:" << (it->pid) << endl;
        getStatusByPid(it->pid);
    }
    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
    clearTable();
}

int ProcessTable::getTableSize()
{
    return pcb.size();
}
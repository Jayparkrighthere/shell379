#include "header.h"
// TODO: parse the values returned from ps
ProcessTable::ProcessTable()
{
    activeNum = 0;
}

void ProcessTable ::addProcessToTable(Process p)
{
    pcb.push_back(p);
    activeNum++;
}

void ProcessTable ::removeProcessFromTable(pid_t pid)
{
    for (vector<Process>::iterator it = pcb.begin(); it != pcb.end(); ++it)
    {
        if (it->pid == pid)
        {
            pcb.erase(it);
            break;
        }
    }
    activeNum--;
}

void ProcessTable ::killAllProcessFromTable()
{
    for (vector<Process>::iterator it = pcb.begin(); it != pcb.end(); ++it)
    {
        killProcess(it->pid);
        activeNum--;
    }
    if (activeNum > 0)
    {
        cout << "still uncleaned process left in table" << endl;
    }
    else if (activeNum < 0)
    {
        cout << "killAllProcessFromTable error" << endl;
    }
}

void ProcessTable::printStatusByPid(pid_t pid, string command)
{
    char *psArgs[5];
    char pidBuffer[1024];
    char pStatusBuffer[1024];
    FILE *fp;

    sprintf(pidBuffer, "%d", pid);

    char str1[100] = "ps -p ";
    char str2[20] = " -o pid,stat,time";
    strcat(str1, pidBuffer);
    strcat(str1, str2);
    fp = popen(str1, "r");
    int index = 0;
    while (fgets(pStatusBuffer, 1024, fp) != NULL)
    {
        if (index)
        {
            string statusStr(pStatusBuffer);
            statusStr = regex_replace(statusStr, regex("\\n"), "");
            cout << index++ << ":   ";
            cout << statusStr;
            cout << command << endl;
        }

        index++;
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
    if (getTableSize() > 0)
    {
        cout << "#      PID S   SEC     COMMAND" << endl;
        for (vector<Process>::iterator it = pcb.begin(); it != pcb.end(); ++it)
        {
            cout << "pid = " << it->pid << " coammnd = " << it->command << endl;
            printStatusByPid(it->pid, it->command);
        }
    }

    cout << "Processes =      " << getNumActive() << " active" << endl;
}

int ProcessTable::getTableSize()
{
    return pcb.size();
}

int ProcessTable::getNumActive()
{
    return activeNum;
}

void ProcessTable::suspendProcess()
{
    activeNum--;
}

void ProcessTable::resumeProcess()
{
    activeNum++;
}
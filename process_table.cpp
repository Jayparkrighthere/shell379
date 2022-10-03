#include "header.h"
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
    if (isSuspendedProcess(pid))
    {
        removeSuspProcess(pid);
    }
    else
    {
        activeNum--;
    }
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

bool ProcessTable::printStatusByPid(pid_t pid, string command, int entrCount)
{
    char *psArgs[5];
    char pidBuffer[1024];
    char pStatusBuffer[1024];
    FILE *fp;
    bool isPrinted = false;

    sprintf(pidBuffer, "%d", pid);

    char str1[100] = "ps -p ";
    char str2[21] = " -o pid,stat,etimes=";
    strcat(str1, pidBuffer);
    strcat(str1, str2);
    fp = popen(str1, "r");
    int index = 0;
    while (fgets(pStatusBuffer, 1024, fp) != NULL)
    {
        if (index)
        {
            // not dot print the header returned from ps.
            string statusStr(pStatusBuffer);
            statusStr = regex_replace(statusStr, regex("\\n"), "");
            cout << entrCount << ":   " << statusStr << "   " << command;
            /*
            if the process is completed, then ps will not return a line
            for that process. ps will only return a header line.
            */
            isPrinted = true;
        }
        index++;
    };

    if (!isPrinted)
    {
        /*
        is process is not printed, we need to take this process
        out from out pcb table, because this process is dead already.
        */
        completedProcesses.push_back(pid);
    };
    pclose(fp);
    return isPrinted;
}

void ProcessTable::clearTable()
{
    pcb.clear();
}

void ProcessTable::printProcessTable()
{
    // keep tracks of the # of process we are printing in the table
    int existingEntriesCount = 0;
    cout << "Running processes:" << endl;
    if (getTableSize() > 0)
    {
        cout << "#      PID S   SEC     COMMAND" << endl;
        for (vector<Process>::iterator it = pcb.begin(); it != pcb.end(); ++it)
        {
            if (printStatusByPid(it->pid, it->command, existingEntriesCount))
                existingEntriesCount++;
        }
    }
    if (completedProcesses.size() > 0)
    {
        // update table if there are any completed process in our table
        upDateTable();
    }
    cout << "Processes =      " << getNumActive() << " active" << endl;
}

void ProcessTable::upDateTable()
{
    for (pid_t pid : completedProcesses)
    {
        removeProcessFromTable(pid);
    }
    completedProcesses.clear();
}

int ProcessTable::getTableSize()
{
    return pcb.size();
}

int ProcessTable::getNumActive()
{
    return activeNum;
}

void ProcessTable::suspendProcess(pid_t pid)
{
    if (!isSuspendedProcess(pid))
    {
        suspendedProcesses.push_back(pid);
        activeNum--;
    }
    else
    {
        cout << "suspend failed: " << pid << " is already suspended." << endl;
    };
}

void ProcessTable::resumeProcess(pid_t pid)
{
    if (isSuspendedProcess(pid))
    {
        removeSuspProcess(pid);
        activeNum++;
    }
    else
    {
        cout << "resume failed: " << pid << " is not in suspended state." << endl;
    }
}

bool ProcessTable::isSuspendedProcess(pid_t pid)
{
    for (int i = 0; i < suspendedProcesses.size(); i++)
    {
        pid_t susPid = suspendedProcesses[i];
        if (susPid == pid)
        {
            return true;
        }
    };
    return false;
}

void ProcessTable::removeSuspProcess(pid_t pid)
{
    for (int i = 0; i < suspendedProcesses.size(); i++)
    {
        pid_t susPid = suspendedProcesses[i];
        if (susPid == pid)
        {
            suspendedProcesses.erase(suspendedProcesses.begin() + i);
            break;
        }
    };
}
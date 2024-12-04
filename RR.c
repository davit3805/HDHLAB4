#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct {
    int iPID;
    int iArrival, iBurst, iRemaining;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]) {
    printf("Input Process:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &P[i].iPID, &P[i].iArrival, &P[i].iBurst);
        P[i].iRemaining = P[i].iBurst;
    }
}

void printProcess(int n, PCB P[]) {
    for (int i = 0; i < n; i++) {
        P[i].iWaiting = P[i].iFinish - P[i].iArrival - P[i].iBurst;
        P[i].iStart = P[i].iResponse + P[i].iArrival;
        printf("%-10d %-10d %-10d %-10d %-10d %-10d %-10d %-10d\n", 
               P[i].iPID, P[i].iArrival, P[i].iBurst, 
               P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }
}
void exportGanttChart(int n, PCB P[]) {
    for (int i = 0; i < n; i++) {
        printf("| %d---%d | P%d ", P[i].iStart, P[i].iFinish, P[i].iPID);
    }
    printf("|\n");
}
void pushProcess(int *n, PCB P[], PCB Q)
{
    (*n)++;
    P[*n - 1] = Q;
}
void removeProcess(int *n, int index, PCB P[])
{
    for (int i = index; i < *n - 1; i++) P[i] = P[i + 1];
    (*n)--;
}
int swapProcess(PCB *P, PCB *Q)
{
    PCB tmp = *P; *P = *Q; *Q = tmp;
}
int partition (PCB P[], int low, int high, int iCriteria)
{
    int pivot;
    switch (iCriteria)
    {
        case 0:
            pivot = P[high].iArrival; break;
        case 1:
            pivot = P[high].iPID; break;
        case 2:
            pivot = P[high].iBurst; break;
        case 3:
            pivot = P[high].iStart; break;  
    }
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) 
    {
        int tmp;
        switch (iCriteria)
        {
            case 0:
                tmp = P[j].iArrival; break;
            case 1:
                tmp = P[j].iPID; break;
            case 2:
                tmp = P[j].iBurst; break;
            case 3:
                tmp = P[j].iStart; break;  
        }
        if (tmp < pivot) 
        {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);  
    return i + 1;
}
void quickSort(PCB P[], int low, int high, int iCriteria)
{
    if (low < high) 
    {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}
void calculateAWT(int n, PCB P[])
{
    float result = 0;
    for (int i = 0; i < n; i++) 
        result += P[i].iFinish - P[i].iArrival - P[i].iBurst;
    result /= n;
    printf("Average Waiting Time: %.2f\n", result);
}
void calculateATaT(int n, PCB P[])
{
    float result = 0;
    for (int i = 0; i < n; i++) 
        result += P[i].iFinish - P[i].iArrival;
    result /= n;
    printf("Average Turnaround Time: %.2f\n", result);
}
int main() 
{ 
    PCB Input[10], ReadyQueue[10], TerminatedArray[10], GanttArray[100];
    int iCurrentTime, iQuantumTime, iNumberOfProcess, iRemain, iReady = 0, iTerminated = 0, iGantt = 0;

    printf("Please input Quantum time: ");
    scanf("%d", &iQuantumTime);
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    iRemain = iNumberOfProcess;

    inputProcess(iNumberOfProcess, Input); 
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL); 

    pushProcess(&iReady, ReadyQueue, Input[0]); 
    removeProcess(&iRemain, 0, Input); 

    ReadyQueue[0].iStart = ReadyQueue[0].iArrival; 
    ReadyQueue[0].iFinish = ReadyQueue[0].iArrival + (iQuantumTime < ReadyQueue[0].iBurst ? iQuantumTime : ReadyQueue[0].iBurst); 
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival; 
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse; 
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
    iCurrentTime = ReadyQueue[0].iFinish; 
    ReadyQueue[0].iRemaining -= (iQuantumTime < ReadyQueue[0].iRemaining ? iQuantumTime : ReadyQueue[0].iRemaining);

    while (iTerminated < iNumberOfProcess) 
    { 
        while (iRemain > 0 && Input[0].iArrival <= iCurrentTime) 
        { 
            pushProcess(&iReady, ReadyQueue, Input[0]); 
            removeProcess(&iRemain, 0, Input); 
        } 

        if (iReady > 0) 
        { 
            while ((iReady == 1 || ReadyQueue[0].iFinish < ReadyQueue[1].iArrival) && ReadyQueue[0].iRemaining > 0) 
            {   
                ReadyQueue[0].iFinish += (iQuantumTime < ReadyQueue[0].iRemaining ? iQuantumTime : ReadyQueue[0].iRemaining);
                iCurrentTime += (iQuantumTime < ReadyQueue[0].iRemaining ? iQuantumTime : ReadyQueue[0].iRemaining);
                ReadyQueue[0].iRemaining -= (iQuantumTime < ReadyQueue[0].iRemaining ? iQuantumTime : ReadyQueue[0].iRemaining);
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
                ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst; 
            }

            if (ReadyQueue[0].iRemaining > 0) 
                pushProcess(&iReady, ReadyQueue, ReadyQueue[0]);
            else 
                pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);

            pushProcess(&iGantt, GanttArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);

            if (iReady > 0) { 
                if (iGantt > 0) {
                    ReadyQueue[0].iStart = (iCurrentTime > GanttArray[iGantt - 1].iFinish) ? iCurrentTime : GanttArray[iGantt - 1].iFinish;
                } else {
                    ReadyQueue[0].iStart = (iCurrentTime > ReadyQueue[0].iArrival) ? iCurrentTime : ReadyQueue[0].iArrival;
                }

                if (ReadyQueue[0].iRemaining == ReadyQueue[0].iBurst) {
                    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
                }

                ReadyQueue[0].iFinish = ReadyQueue[0].iStart + (iQuantumTime < ReadyQueue[0].iRemaining ? iQuantumTime : ReadyQueue[0].iRemaining);
                iCurrentTime = ReadyQueue[0].iFinish;
                ReadyQueue[0].iRemaining -= (iQuantumTime < ReadyQueue[0].iRemaining ? iQuantumTime : ReadyQueue[0].iRemaining);
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
                ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst;
            }

        } 
    } 
    printf("\n===== Round Robin Scheduling =====\n");
    printf("%-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", 
           "PID", "Arrival", "Burst", "Start", "Finish", "Waiting", "Response", "TaT");
    printProcess(iTerminated, TerminatedArray);
    printf("===== Gantt Chart ======\n");
    exportGanttChart(iGantt, GanttArray); 
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID); 
    calculateAWT(iTerminated, TerminatedArray); 
    calculateATaT(iTerminated, TerminatedArray); 
    return 0; 
} 

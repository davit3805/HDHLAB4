#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3
typedef struct{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;
void inputProcess(int n, PCB P[])
{
    printf("Input Process:\n");
    for (int i = 0; i < n; i++) 
        scanf("%d %d %d", &P[i].iPID, &P[i].iArrival, &P[i].iBurst);
}
void printProcess(int n, PCB P[])
{
    for (int i = 0; i < n; i++)
        printf("%-10d %-10d %-10d %-10d %-10d %-10d %-10d %-10d\n", 
               P[i].iPID, P[i].iArrival, P[i].iBurst, 
               P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
}
void exportGanttChart (int n, PCB P[])
{
    for (int i = 0; i < n; i++)
    {
        printf("| %d", P[i].iStart);
        for (int j = 0; j < P[i].iFinish - P[i].iStart; j++) printf("-");
        printf("%d | P%d ", P[i].iFinish, P[i].iPID);
    }
    printf("\n");
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

int main() {
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;

    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0, currentTime = 0;

    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);

    while (iTerminated < iNumberOfProcess) {
        while (iRemain > 0 && Input[0].iArrival <= currentTime) {
            pushProcess(&iReady, ReadyQueue, Input[0]);
            removeProcess(&iRemain, 0, Input);
        }

        if (iReady == 0) {
            currentTime++;
            continue;
        }

        quickSort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST);
        ReadyQueue[0].iStart = currentTime;
        ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
        ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
        ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
        ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

        pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
        removeProcess(&iReady, 0, ReadyQueue);
        currentTime = TerminatedArray[iTerminated - 1].iFinish;
    }

    printf("\n===== SJF Scheduling =====\n");
    printf("%-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", 
           "PID", "Arrival", "Burst", "Start", "Finish", "Waiting", "Response", "TaT");
    printProcess(iTerminated, TerminatedArray);

    printf("===== Gantt Chart ======\n");
    exportGanttChart(iTerminated, TerminatedArray);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

    return 0;
}

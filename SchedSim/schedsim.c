// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"
#include <stdbool.h>

// Round Robin (RR) Scheduling
// Function to find the waiting time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n,int quantum) 
{ 
  /*
    1. Create an array *rem_bt[]* to keep track of remaining burst time of processes. This array is initially a copy of *plist[].bt* (all processes burst times)
    2. Store waiting times of processes in plist[].wt. Initialize this array as 0.
    3. Initialize time : t = 0
    4. Keep traversing the all processes while all processes are not done. Do following for i'th process if it is not done yet.
      - If rem_bt[i] > quantum
        (i)  t = t + quantum
        (ii) bt_rem[i] -= quantum;
      - Else // Last cycle for this process
        (i)  t = t + bt_rem[i];
        (ii) plist[i].wt = t - plist[i].bt
        (iii) bt_rem[i] = 0; // This process is over
      
  */

  int rem_bt[n]; 
  for (int i = 0; i < n; i++) 
  {
      rem_bt[i] = plist[i].bt; 
  }

  int t = 0; 
  bool done;

  do 
  { 
      done = true; 
      for (int i = 0; i < n; i++) 
      { 
          if (rem_bt[i] > 0) 
          { 
              done = false; 
              if (rem_bt[i] > quantum) 
              { 
                  t += quantum; 
                  rem_bt[i] -= quantum; 
              } 
              else 
              { 
                  t += rem_bt[i]; 
                  plist[i].wt = t - plist[i].bt - plist[i].art; 
                  rem_bt[i] = 0; 
              } 
          } 
      } 
  } while (!done);

  for (int i = 0; i < n; i++) 
  {
      if (plist[i].wt < 0) 
      {
          plist[i].wt = 0;
      }
  }

  
} 

// Shortest Job First (SJF) Scheduling
// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
    /*
      * 1 Traverse until all process gets completely executed.
        - Find process with minimum remaining time at every single time lap.
        - Reduce its time by 1.
        - Check if its remaining time becomes 0 
        - Increment the counter of process completion.
        - Completion time of *current process = current_time +1;*
        - Calculate waiting time for each completed process. *wt[i]= Completion time - arrival_time-burst_time*
        - Increment time lap by one.

    */

    int rt[n]; 
    for (int i = 0; i < n; i++) 
    {
        rt[i] = plist[i].bt; 
    }

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    int check = 0;

    while (complete != n) 
    { 
        for (int j = 0; j < n; j++) 
        { 
            if ((plist[j].art <= t) && (rt[j] < minm) && rt[j] > 0) 
            { 
                minm = rt[j]; 
                shortest = j; 
                check = 1; 
            } 
        } 

        if (check == 0) 
        { 
            t++; 
            continue; 
        } 

        rt[shortest]--; 

        minm = rt[shortest];

        if (minm == 0) 
        { 
            minm = INT_MAX;
        } 

        if (rt[shortest] == 0)
        {
          complete++;
          check =0;

          finish_time = t + 1;

          plist[shortest].wt = finish_time - plist[shortest].art - plist[shortest].bt;

          if (plist[shortest].wt < 0)
          {
            plist[shortest].wt = 0;
          }
        }
        t++; 
    } 
} 

// Function to find the waiting time for all processes in FCFS
void findWaitingTimeFCFS(ProcessType plist[], int n)
{ 
    // Waiting time for the first process is 0
    plist[0].wt = 0;

    // Calculating waiting time for each process
    for (int i = 1; i < n; i++) {
        plist[i].wt = plist[i - 1].wt + plist[i - 1].bt;
    }
}

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // Initialize waiting time for the first process
    // If the first process doesn't start at time 0, it should wait until its arrival time
    plist[0].wt = 0;

    // Initialize a variable to keep track of the end time of the last process
    int endTime = plist[0].art + plist[0].bt;

    for (int i = 1; i < n; i++) {
        // Waiting time is the end time of the last process minus the arrival time of the current process
        plist[i].wt = endTime - plist[i].art;

        // If the waiting time is negative (which means the process arrived after the last one finished),
        // set the waiting time to 0 and update the end time to this process's arrival time
        if (plist[i].wt < 0) {
            plist[i].wt = 0;
            endTime = plist[i].art;
        }

        // Update the end time of the current process
        endTime += plist[i].bt;
    }
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
  
  /*  
  * 1. Cast this and that into (ProcessType *)
  * 2. return 1 if this->pri < that->pri
  */ 
  
  ProcessType *p1 = (ProcessType *)this;
  ProcessType *p2 = (ProcessType *)that;

  // Primary sort by priority
  int priorityDiff = p2->pri - p1->pri;;

  // If priorities are equal, then sort by arrival time
  if (priorityDiff == 0) 
  {
      return p1->art - p2->art;
  }

  return priorityDiff;
} 

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeFCFS(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

void findWaitingTimePriority(ProcessType plist[], int n) {
    int completedTime = 0;

    for (int i = 0; i < n; i++) {
        if (plist[i].art > completedTime) {
            // If the next process arrives after the current one has completed
            completedTime = plist[i].art;
        }
        plist[i].wt = completedTime - plist[i].art; // Waiting time is current time minus arrival time
        completedTime += plist[i].bt; // Update completed time
    }
}


//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
  
  /*
  * 1- Sort the processes (i.e. plist[]), burst time and priority according to the priority.
  * 2- Now simply apply FCFS algorithm.
  */
  qsort(plist, n, sizeof(ProcessType), my_comparer); 
  findWaitingTimePriority(plist, n); 
  findTurnAroundTime(plist, n);
  //printMetrics(plist, n);

  //Display processes along with all details 
  printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 

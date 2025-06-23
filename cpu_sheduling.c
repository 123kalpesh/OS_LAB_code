/*cpu sheduling*/

#include <stdio.h>
#include <limits.h>

// Structure to represent a process
struct Process {
    int pid, at, bt, priority, ct, tat, wt;
    int remaining_bt;  // For preemptive algorithms
    int completed;     // For tracking completion status
};

// Function to sort processes by arrival time
void sort_by_arrival(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

// Function to sort processes by PID
void sort_by_pid(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].pid > p[j + 1].pid) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

// FCFS Scheduling Algorithm
void fcfs_scheduling(struct Process p[], int n) {
    sort_by_arrival(p, n);

    int time = 0; 
    float total_tat = 0, total_wt = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].at) {  
            time = p[i].at;  
        }
        p[i].ct = time + p[i].bt; 
        time = p[i].ct; 
    }

    // Calculate TAT, WT and sum them for average
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        total_tat += p[i].tat;
        total_wt += p[i].wt;
    }

    // Print Gantt Chart
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n; i++) {
        printf("--------");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        printf(" P%d |", p[i].pid);
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        printf("--------");
    }
    printf("\n0");
    for (int i = 0; i < n; i++) {
        printf("      %d", p[i].ct);
    }
    printf("\n");

    // Sort by PID for final output
    sort_by_pid(p, n);

    // Print Process Table
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    // Print Averages
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

// Non-Preemptive SJF Scheduling Algorithm
void sjf_non_preemptive(struct Process p[], int n) {
    int completed = 0, time = 0;
    float total_tat = 0, total_wt = 0;

    // Initialize completion status for all processes
    for (int i = 0; i < n; i++) {
        p[i].completed = 0;
    }

    int gantt[n], gantt_time[n + 1];
    int gantt_index = 0;

    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n; i++) {
        printf("--------");
    }
    printf("\n|");

    while (completed < n) {
        int shortest = -1;
        int min_bt = INT_MAX;  // Large value to find minimum burst time

        // Find the process with the shortest burst time that has arrived
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].completed == 0 && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                shortest = i;
            }
        }

        if (shortest == -1) {
            time++;  // If no process is available, move time forward
        } else {
            printf(" P%d |", p[shortest].pid);

            gantt[gantt_index] = p[shortest].pid;
            gantt_time[gantt_index] = time;
            gantt_index++;

            time += p[shortest].bt;
            p[shortest].ct = time;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;

            total_tat += p[shortest].tat;
            total_wt += p[shortest].wt;

            p[shortest].completed = 1;
            completed++;
        }
    }

    // Store the final completion time
    gantt_time[gantt_index] = time;

    printf("\n ");
    for (int i = 0; i < n; i++) {
        printf("--------");
    }

    // Print Gantt Chart times
    printf("\n");
    for (int i = 0; i <= gantt_index; i++) {
        printf("%d\t", gantt_time[i]);
    }
    printf("\n");

    // Sort by PID for final output
    sort_by_pid(p, n);

    // Print Process Table
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    // Print Averages
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

// Function to find the process with the shortest remaining time
int findShortestProcess(struct Process p[], int n, int current_time) {
    int shortest = -1, min_bt = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (p[i].at <= current_time && p[i].remaining_bt > 0 && p[i].remaining_bt < min_bt) {
            min_bt = p[i].remaining_bt;
            shortest = i;
        }
    }
    return shortest;
}

// Preemptive SJF (SRTF) Scheduling Algorithm
void sjf_preemptive(struct Process p[], int n) {
    int completed = 0, time = 0;
    float total_tat = 0, total_wt = 0;
    
    // Initialize remaining burst time for all processes
    for (int i = 0; i < n; i++) {
        p[i].remaining_bt = p[i].bt;
    }

    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n * 5; i++) {
        printf("-");
    }
    printf("\n|");

    int last_executed = -1;
    int gantt_time[100], gantt_pid[100], gantt_index = 0;

    while (completed < n) {
        int shortest = findShortestProcess(p, n, time);

        if (shortest == -1) {
            time++;
        } else {
            if (last_executed != p[shortest].pid) {
                printf(" P%d |", p[shortest].pid);
                last_executed = p[shortest].pid;

                // Store time values for Gantt Chart
                gantt_pid[gantt_index] = p[shortest].pid;
                gantt_time[gantt_index] = time;
                gantt_index++;
            }

            p[shortest].remaining_bt--;
            time++;

            // If process is completed
            if (p[shortest].remaining_bt == 0) {
                p[shortest].ct = time;
                p[shortest].tat = p[shortest].ct - p[shortest].at;
                p[shortest].wt = p[shortest].tat - p[shortest].bt;
                
                total_tat += p[shortest].tat;
                total_wt += p[shortest].wt;
                
                completed++;
            }
        }
    }

    printf("\n ");
    for (int i = 0; i < n * 5; i++) {
        printf("-");
    }
    printf("\n");

    // Print time values below Gantt Chart
    printf("0");
    for (int i = 0; i < gantt_index; i++) {
        printf("     %d", gantt_time[i]);
    }
    printf("     %d\n", time);

    // Print Process Table
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    // Print Averages
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

// Function to sort processes by Arrival Time (AT) and Priority
void sort_by_priority(struct Process p[], int n, int start_time, int priority_order) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int should_swap = 0;

            if (p[j].at <= start_time && p[j + 1].at <= start_time) {
                // If both processes have arrived, sort by priority
                if (priority_order == 1) {  // Higher number = Higher priority
                    should_swap = (p[j].priority < p[j + 1].priority);
                } else {  // Lower number = Higher priority
                    should_swap = (p[j].priority > p[j + 1].priority);
                }
            } else if (p[j].at > start_time && p[j + 1].at <= start_time) {
                should_swap = 1;
            }

            if (should_swap) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

// Non-Preemptive Priority Scheduling Algorithm
void priority_non_preemptive(struct Process p[], int n, int priority_order) {
    int completed = 0, time = 0, prev_time = 0;
    float total_tat = 0, total_wt = 0;

    // Initialize completion status
    for (int i = 0; i < n; i++) {
        p[i].completed = 0;
    }

    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n; i++) {
        printf("--------");
    }
    printf("\n|");

    int gantt_time[100], gantt_pid[100], gantt_index = 0;

    while (completed < n) {
        int selected = -1;

        // Sort available processes based on priority order
        sort_by_priority(p, n, time, priority_order);

        // Select the first process in the sorted list
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].completed == 0) {
                selected = i;
                break;
            }
        }

        if (selected == -1) {
            time++;  // If no process is available, move time forward
        } else {
            printf(" P%d |", p[selected].pid);
            prev_time = time;
            time += p[selected].bt;
            p[selected].ct = time;
            p[selected].tat = p[selected].ct - p[selected].at;
            p[selected].wt = p[selected].tat - p[selected].bt;

            total_tat += p[selected].tat;
            total_wt += p[selected].wt;

            p[selected].completed = 1;
            completed++;

            // Store Gantt Chart times
            gantt_pid[gantt_index] = p[selected].pid;
            gantt_time[gantt_index] = time;
            gantt_index++;
        }
    }

    printf("\n ");
    for (int i = 0; i < n; i++) {
        printf("--------");
    }
    printf("\n");

    // Print time values under Gantt Chart
    printf("0");
    for (int i = 0; i < gantt_index; i++) {
        printf("      %d", gantt_time[i]);
    }
    printf("\n");

    // Print Process Table
    printf("\nPID\tAT\tBT\tPriority\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].priority, p[i].ct, p[i].tat, p[i].wt);
    }

    // Print Averages
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

// Function to find the process with the highest priority that has arrived
int find_highest_priority(struct Process p[], int n, int time, int priority_order) {
    int highest_priority = (priority_order == 1) ? -9999 : 9999;
    int selected = -1;

    for (int i = 0; i < n; i++) {
        if (p[i].at <= time && p[i].remaining_bt > 0) {
            if ((priority_order == 1 && p[i].priority > highest_priority) || 
                (priority_order == 2 && p[i].priority < highest_priority)) {
                highest_priority = p[i].priority;
                selected = i;
            }
        }
    }
    return selected;
}

// Preemptive Priority Scheduling Algorithm
void priority_preemptive(struct Process p[], int n, int priority_order) {
    int completed = 0, time = 0;
    float total_tat = 0, total_wt = 0;
    
    // Copy burst times to remaining burst time
    for (int i = 0; i < n; i++) {
        p[i].remaining_bt = p[i].bt;
    }

    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n * 3; i++) {
        printf("-");
    }
    printf("\n|");

    int gantt_time[100], gantt_pid[100], gantt_index = 0;
    int last_executed = -1;

    while (completed < n) {
        int selected = find_highest_priority(p, n, time, priority_order);

        if (selected == -1) {
            time++;  // If no process is available, move time forward
        } else {
            if (last_executed != p[selected].pid) {
                printf("P%d|", p[selected].pid);
                last_executed = p[selected].pid;
                
                gantt_pid[gantt_index] = p[selected].pid;
                gantt_time[gantt_index] = time;
                gantt_index++;
            }

            time++;
            p[selected].remaining_bt--;

            if (p[selected].remaining_bt == 0) {
                completed++;
                p[selected].ct = time;
                p[selected].tat = p[selected].ct - p[selected].at;
                p[selected].wt = p[selected].tat - p[selected].bt;

                total_tat += p[selected].tat;
                total_wt += p[selected].wt;
            }
        }
    }

    printf("\n ");
    for (int i = 0; i < n * 3; i++) {
        printf("-");
    }
    printf("\n");

    // Print time values under Gantt Chart
    printf("0");
    for (int i = 0; i < gantt_index; i++) {
        printf("  %d", gantt_time[i]);
    }
    printf("  %d\n", time);

    // Print Process Table
    printf("\nPID\tAT\tBT\tPriority\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].priority, p[i].ct, p[i].tat, p[i].wt);
    }

    // Print Averages
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

// Round Robin Scheduling Algorithm
void round_robin(struct Process p[], int n, int time_quantum) {
    int completed = 0, time = 0, queue[100], front = 0, rear = 0;
    float total_tat = 0, total_wt = 0;
    
    // Copy burst times to remaining burst time
    for (int i = 0; i < n; i++) {
        p[i].remaining_bt = p[i].bt;
    }

    // Track execution for Gantt Chart
    int gantt_pid[100], gantt_time[100], gantt_index = 0;

    // Initialize queue with first arrived process
    for (int i = 0; i < n; i++) {
        if (p[i].at == 0) {
            queue[rear++] = i;
        }
    }

    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n * 3; i++) {
        printf("-");
    }
    printf("\n|");

    while (completed < n) {
        if (front == rear) { // If queue is empty (CPU idle)
            time++;
            // Check for newly arrived processes
            for (int i = 0; i < n; i++) {
                if (p[i].at == time && p[i].remaining_bt > 0) {
                    queue[rear++] = i;
                }
            }
            continue;
        }

        int idx = queue[front++];
        printf("P%d|", p[idx].pid);
        gantt_pid[gantt_index] = p[idx].pid;
        gantt_time[gantt_index++] = time;

        int exec_time = (p[idx].remaining_bt < time_quantum) ? p[idx].remaining_bt : time_quantum;
        time += exec_time;
        p[idx].remaining_bt -= exec_time;

        // Add newly arrived processes to queue
        for (int i = 0; i < n; i++) {
            if (p[i].at > time - exec_time && p[i].at <= time && p[i].remaining_bt > 0) {
                queue[rear++] = i;
            }
        }

        if (p[idx].remaining_bt > 0) {
            queue[rear++] = idx; // Add the same process back
        } else {
            completed++;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            total_tat += p[idx].tat;
            total_wt += p[idx].wt;
        }
    }

    // Print Gantt Chart
    printf("\n ");
    for (int i = 0; i < n * 3; i++) {
        printf("-");
    }
    printf("\n0");
    for (int i = 0; i < gantt_index; i++) {
        printf("  %d", gantt_time[i] + (p[gantt_pid[i]-1].remaining_bt == 0 ? 
                                        p[gantt_pid[i]-1].bt % time_quantum : 
                                        time_quantum));
    }
    printf("\n");

    // Print Process Table
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    // Print Averages
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

int main() {
    int choice, n, priority_order, time_quantum;
    
    while (1) {
        printf("\n\n===== CPU SCHEDULING ALGORITHMS =====\n");
        printf("1. First Come First Served (FCFS)\n");
        printf("2. Shortest Job First (SJF) - Non-Preemptive\n");
        printf("3. Shortest Remaining Time First (SRTF) - Preemptive SJF\n");
        printf("4. Priority Scheduling - Non-Preemptive\n");
        printf("5. Priority Scheduling - Preemptive\n");
        printf("6. Round Robin\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 0) {
            printf("Exiting program...\n");
            break;
        }
        
        printf("Enter number of processes: ");
        scanf("%d", &n);
        
        struct Process p[n];
        
        switch (choice) {
            case 1: // FCFS
                printf("Enter Process ID, Arrival Time, and Burst Time for each process:\n");
                for (int i = 0; i < n; i++) {
                    scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
                }
                fcfs_scheduling(p, n);
                break;
                
            case 2: // Non-Preemptive SJF
                printf("Enter Process ID, Arrival Time, and Burst Time for each process:\n");
                for (int i = 0; i < n; i++) {
                    scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
                }
                sjf_non_preemptive(p, n);
                break;
                
            case 3: // Preemptive SJF (SRTF)
                printf("Enter Process ID, Arrival Time, and Burst Time for each process:\n");
                for (int i = 0; i < n; i++) {
                    scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
                }
                sjf_preemptive(p, n);
                break;
                
            case 4: // Non-Preemptive Priority
                printf("Enter Process ID, Arrival Time, Burst Time, and Priority for each process:\n");
                for (int i = 0; i < n; i++) {
                    scanf("%d %d %d %d", &p[i].pid, &p[i].at, &p[i].bt, &p[i].priority);
                }
                
                printf("\nChoose priority order:\n");
                printf("1. Higher number = Higher priority\n");
                printf("2. Lower number = Higher priority\n");
                printf("Enter your choice (1 or 2): ");
                scanf("%d", &priority_order);
                
                priority_non_preemptive(p, n, priority_order);
                break;
                
            case 5: // Preemptive Priority
                printf("Enter Process ID, Arrival Time, Burst Time, and Priority for each process:\n");
                for (int i = 0; i < n; i++) {
                    scanf("%d %d %d %d", &p[i].pid, &p[i].at, &p[i].bt, &p[i].priority);
                }
                
                printf("\nChoose priority order:\n");
                printf("1. Higher number = Higher priority\n");
                printf("2. Lower number = Higher priority\n");
                printf("Enter your choice (1 or 2): ");
                scanf("%d", &priority_order);
                
                priority_preemptive(p, n, priority_order);
                break;
                
            case 6: // Round Robin
                printf("Enter Process ID, Arrival Time, and Burst Time for each process:\n");
                for (int i = 0; i < n; i++) {
                    scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
                }
                
                printf("Enter Time Quantum: ");
                scanf("%d", &time_quantum);
                
                round_robin(p, n, time_quantum);
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}



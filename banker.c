
/*banker algorithm*/
#include <stdio.h>
#include <stdbool.h>
#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

// Function to check if the system is in a safe state and find safe sequence
void findSafeSequence(int processes, int resources, int available[], int maximum[][MAX_RESOURCES], 
                      int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES]) {
    // Create a copy of the available resources array
    int work[MAX_RESOURCES];
    for (int i = 0; i < resources; i++) {
        work[i] = available[i];
    }

    // Array to mark which processes are finished
    bool finish[MAX_PROCESSES] = {false};

    // Store the safe sequence
    int safeSeq[MAX_PROCESSES];
    int count = 0;

    // Find a process that can be allocated resources
    while (count < processes) {
        bool found = false;
        for (int p = 0; p < processes; p++) {
            // If this process is not finished
            if (finish[p] == false) {
                // Check if all resources can be allocated
                int j;
                for (j = 0; j < resources; j++) {
                    if (need[p][j] > work[j])
                        break;
                }

                // If all resources can be allocated
                if (j == resources) {
                    // Add the allocated resources back to work
                    for (int k = 0; k < resources; k++) {
                        work[k] += allocation[p][k];
                    }

                    // Add process to safe sequence and mark it as finished
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }

        // If we couldn't find a process to allocate resources to
        if (found == false) {
            printf("System is not in safe state.\n");
            return;
        }
    }

    // If we get here, the system is in a safe state
    printf("System is in safe state.\nSafe sequence: ");
    for (int i = 0; i < processes; i++) {
        printf("P%d", safeSeq[i]);
        if (i < processes - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

// Function to calculate need matrix
void calculateNeedMatrix(int processes, int resources, int maximum[][MAX_RESOURCES], 
                         int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES]) {
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

// Function to display matrices
void displayState(int processes, int resources, int available[], int maximum[][MAX_RESOURCES], 
                 int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES]) {
    printf("\nCurrent System State:\n");
    
    printf("\nAvailable:\n");
    for (int i = 0; i < resources; i++) {
        printf("R%d: %d  ", i, available[i]);
    }
    
    printf("\n\nMax Need:\n");
    for (int i = 0; i < processes; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < resources; j++) {
            printf("%d ", maximum[i][j]);
        }
        printf("\n");
    }
    
    printf("\nAllocation:\n");
    for (int i = 0; i < processes; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < resources; j++) {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }
    
    printf("\nRemaining Need:\n");
    for (int i = 0; i < processes; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < resources; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to calculate available resources from total resources
void calculateAvailableResources(int processes, int resources, int total[], 
                                int allocation[][MAX_RESOURCES], int available[]) {
    for (int j = 0; j < resources; j++) {
        int sum = 0;
        for (int i = 0; i < processes; i++) {
            sum += allocation[i][j];
        }
        available[j] = total[j] - sum;
        
        // Validation check
        if (available[j] < 0) {
            printf("Error: Total allocation for resource R%d exceeds total available\n", j);
            // Set to 0 to avoid negative values
            available[j] = 0;
        }
    }
}

int main() {
    int processes, resources, input_method;
    
    printf("Enter number of processes: ");
    scanf("%d", &processes);
    
    printf("Enter number of resources: ");
    scanf("%d", &resources);
    
    if (processes > MAX_PROCESSES || resources > MAX_RESOURCES) {
        printf("Error: Maximum limit for processes or resources exceeded\n");
        return 1;
    }

    int available[MAX_RESOURCES];
    int total[MAX_RESOURCES];
    int maximum[MAX_PROCESSES][MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];

    // Input method selection
    printf("\nSelect what you have?\n");
    printf("1. Available\n");
    printf("2. Total\n");
    printf("Enter choice (1 or 2): ");
    scanf("%d", &input_method);

    // Input maximum claim for each process
    printf("\nEnter Max Need0 1:\n");
    for (int i = 0; i < processes; i++) {
        printf("For process P%d: ", i);
        for (int j = 0; j < resources; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    // Input allocated resources for each process
    printf("\nEnter Allocation:\n");
    for (int i = 0; i < processes; i++) {
        printf("For process P%d: ", i);
        for (int j = 0; j < resources; j++) {
            scanf("%d", &allocation[i][j]);
            
            // Validation: allocation should not exceed maximum
            if (allocation[i][j] > maximum[i][j]) {
                printf("Error: Allocation exceeds maximum claim for resource R%d\n", j);
                i--;  // Re-enter for this process
                break;
            }
        }
    }

    // Handle input based on the selected method
    if (input_method == 1) {
        // Direct input of available resources
        printf("\nEnter Available:\n");
        for (int i = 0; i < resources; i++) {
            printf("R%d: ", i);
            scanf("%d", &available[i]);
        }
    } else if (input_method == 2) {
        // Input total resources and calculate available
        printf("\nEnter Total:\n");
        for (int i = 0; i < resources; i++) {
            printf("R%d: ", i);
            scanf("%d", &total[i]);
        }
        
        // Calculate available resources
        calculateAvailableResources(processes, resources, total, allocation, available);
        
        printf("\nCalculated Available:\n");
        for (int i = 0; i < resources; i++) {
            printf("R%d: %d  ", i, available[i]);
        }
        printf("\n");
    } else {
        printf("Invalid choice. Exiting program.\n");
        return 1;
    }

    // Calculate need matrix
    calculateNeedMatrix(processes, resources, maximum, allocation, need);
    
    // Display the current state
    displayState(processes, resources, available, maximum, allocation, need);
    
    // Find safe sequence
    findSafeSequence(processes, resources, available, maximum, allocation, need);

    return 0;
}


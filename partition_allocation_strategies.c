#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void firstFit(int blockCount, int processCount, int blockSize[], int processSize[]) {
    int allocation[processCount];
    int fragment[processCount];
    int blockUsed[blockCount];
    
    // Initialize arrays
    for (int i = 0; i < processCount; i++) {
        allocation[i] = -1;
        fragment[i] = 0;
    }
    
    for (int i = 0; i < blockCount; i++) {
        blockUsed[i] = 0;
    }
    
    // First Fit algorithm
    for (int i = 0; i < processCount; i++) {
        for (int j = 0; j < blockCount; j++) {
            if (blockUsed[j] == 0 && blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockUsed[j] = 1;
                fragment[i] = blockSize[j] - processSize[i];
                break;
            }
        }
    }
    
    // Display allocation results
    printf("\nFirst Fit Allocation:\n");
    printf("-----------------------------------------------------------\n");
    printf("| Process No. | Process Size | Block Allocated | Fragment |\n");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < processCount; i++) {
        printf("| %-11d | %-12d | ", i, processSize[i]);
        
        if (allocation[i] != -1) {
            char buffer[30];
            printf("%-14s | %-8d |\n", 
                   (sprintf(buffer, "%d (%02d)", blockSize[allocation[i]], allocation[i]), buffer),
                   fragment[i]);
        } else {
            printf("%-14s | %-8s |\n", "NA", "NA");
        }
        printf("-----------------------------------------------------------\n");
    }
}

void bestFit(int blockCount, int processCount, int blockSize[], int processSize[]) {
    int allocation[processCount];
    int fragment[processCount];
    int blockUsed[blockCount];
    
    // Initialize arrays
    for (int i = 0; i < processCount; i++) {
        allocation[i] = -1;
        fragment[i] = 0;
    }
    
    for (int i = 0; i < blockCount; i++) {
        blockUsed[i] = 0;
    }
    
    // Best Fit algorithm
    for (int i = 0; i < processCount; i++) {
        int bestIdx = -1;
        for (int j = 0; j < blockCount; j++) {
            if (blockUsed[j] == 0 && blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx]) {
                    bestIdx = j;
                }
            }
        }
        
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockUsed[bestIdx] = 1;
            fragment[i] = blockSize[bestIdx] - processSize[i];
        }
    }
    
    // Display allocation results
    printf("\nBest Fit Allocation:\n");
    printf("-----------------------------------------------------------\n");
    printf("| Process No. | Process Size | Block Allocated | Fragment |\n");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < processCount; i++) {
        printf("| %-11d | %-12d | ", i, processSize[i]);
        
        if (allocation[i] != -1) {
            char buffer[30];
            printf("%-14s | %-8d |\n", 
                   (sprintf(buffer, "%d (%02d)", blockSize[allocation[i]], allocation[i]), buffer),
                   fragment[i]);
        } else {
            printf("%-14s | %-8s |\n", "NA", "NA");
        }
        printf("-----------------------------------------------------------\n");
    }
}

void worstFit(int blockCount, int processCount, int blockSize[], int processSize[]) {
    int allocation[processCount];
    int fragment[processCount];
    int blockUsed[blockCount];
    
    // Initialize arrays
    for (int i = 0; i < processCount; i++) {
        allocation[i] = -1;
        fragment[i] = 0;
    }
    
    for (int i = 0; i < blockCount; i++) {
        blockUsed[i] = 0;
    }
    
    // Worst Fit algorithm
    for (int i = 0; i < processCount; i++) {
        int worstIdx = -1;
        for (int j = 0; j < blockCount; j++) {
            if (blockUsed[j] == 0 && blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j;
                }
            }
        }
        
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockUsed[worstIdx] = 1;
            fragment[i] = blockSize[worstIdx] - processSize[i];
        }
    }
    
    // Display allocation results
    printf("\nWorst Fit Allocation:\n");
    printf("-----------------------------------------------------------\n");
    printf("| Process No. | Process Size | Block Allocated | Fragment |\n");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < processCount; i++) {
        printf("| %-11d | %-12d | ", i, processSize[i]);
        
        if (allocation[i] != -1) {
            char buffer[30];
            printf("%-14s | %-8d |\n", 
                   allocation[i] == -1 ? "NA" : 
                   (sprintf(buffer, "%d (%02d)", blockSize[allocation[i]], allocation[i]), buffer),
                   fragment[i]);
        } else {
            printf("%-14s | %-8s |\n", "NA", "NA");
        }
        printf("-----------------------------------------------------------\n");
    }
}

int main() {
    int blockCount, processCount;
    char choice;
    
    printf("\nSelect memory allocation strategy:\n");
    printf("1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
    scanf(" %c", &choice);
    
    if (choice == '4') {
        printf("Exiting program...\n");
        exit(0);
    }
    
    if (choice < '1' || choice > '3') {
        printf("Invalid choice! Please restart the program.\n");
        return 1;
    }
    
    printf("Enter the no. of blocks :- ");
    scanf("%d", &blockCount);
    
    int blockSize[blockCount];
    printf("Enter the size of block:\n");
    for (int i = 0; i < blockCount; i++) {
        printf("%d :- ", i);
        scanf("%d", &blockSize[i]);
    }
    
    printf("Enter the no. of processes :- ");
    scanf("%d", &processCount);
    
    int processSize[processCount];
    printf("Enter size of process:\n");
    for (int i = 0; i < processCount; i++) {
        printf("%d :- ", i);
        scanf("%d", &processSize[i]);
    }
    
    switch(choice) {
        case '1':
            firstFit(blockCount, processCount, blockSize, processSize);
            break;
        case '2':
            bestFit(blockCount, processCount, blockSize, processSize);
            break;
        case '3':
            worstFit(blockCount, processCount, blockSize, processSize);
            break;
    }
    
    return 0;
}
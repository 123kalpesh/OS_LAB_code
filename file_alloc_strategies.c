#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_BLOCKS 1000
#define MAX_FILENAME_LENGTH 20

// Sequential File Allocation
typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    int start_block;
    int length;
} SequentialFile;

// Linked File Allocation
typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    int start_block;
    int size;
    int blocks[MAX_BLOCKS];
} LinkedFile;

// Indexed File Allocation
typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    int index_block;
    int length;
    int blocks[MAX_BLOCKS];
} IndexedFile;

// Function prototypes
void sequential_allocation();
void linked_allocation();
void indexed_allocation();
void display_menu();

int main() {
    int choice;
    
    do {
        display_menu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                sequential_allocation();
                break;
            case 2:
                linked_allocation();
                break;
            case 3:
                indexed_allocation();
                break;
            case 4:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);
    
    return 0;
}

void display_menu() {
    printf("\n----- FILE ALLOCATION STRATEGIES -----\n");
    printf("1. Sequential Allocation\n");
    printf("2. Linked Allocation\n");
    printf("3. Indexed Allocation\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void sequential_allocation() {
    int num_files, i, j;
    SequentialFile files[MAX_FILES];
    
    printf("\n----- SEQUENTIAL ALLOCATION -----\n");
    printf("Enter no. of files: ");
    scanf("%d", &num_files);
    
    // Input file details
    for (i = 0; i < num_files; i++) {
        sprintf(files[i].filename, "%d", i+1);  // Filenames are 1, 2, 3...
        
        printf("Enter no. of blocks occupied by file%d: ", i+1);
        scanf("%d", &files[i].length);
        
        printf("Enter the starting block of file%d: ", i+1);
        scanf("%d", &files[i].start_block);
    }
    
    // Display file allocation table
    printf("-----------------------------------\n");
    printf("| Filename | Start block | Length |\n");
    printf("-----------------------------------\n");
    
    for (i = 0; i < num_files; i++) {
        printf("| %-8s | %-11d | %-6d |\n", files[i].filename, files[i].start_block, files[i].length);
        printf("-----------------------------------\n");
    }
    
    // Allow querying specific files
    char filename[MAX_FILENAME_LENGTH];
    printf("Enter file name: ");
    scanf("%s", filename);
    
    // Find the file
    for (i = 0; i < num_files; i++) {
        if (strcmp(files[i].filename, filename) == 0) {
            printf("File name is: %s   length is: %d   blocks occupied ", 
                   files[i].filename, files[i].length);
            
            // Print the blocks occupied
            for (j = 0; j < files[i].length; j++) {
                printf("%d ", files[i].start_block + j);
            }
            printf("\n");
            break;
        }
    }
    
    if (i == num_files) {
        printf("File not found!\n");
    }
}

void linked_allocation() {
    int num_files, i, j;
    LinkedFile files[MAX_FILES];
    
    printf("\n----- LINKED ALLOCATION -----\n");
    printf("Enter no. of files: ");
    scanf("%d", &num_files);
    
    // Input file details
    for (i = 0; i < num_files; i++) {
        printf("Enter file name: ");
        scanf("%s", files[i].filename);
        
        printf("Enter starting block: ");
        scanf("%d", &files[i].start_block);
        
        printf("Enter no. of blocks: ");
        scanf("%d", &files[i].size);
        
        printf("Enter block numbers:\n");
        for (j = 0; j < files[i].size; j++) {
            scanf("%d", &files[i].blocks[j]);
        }
    }
    
    // Display file allocation table
    printf("--------------------------------------\n");
    printf("|File | Start | Size | Block         |\n");
    printf("--------------------------------------\n");
    
    for (i = 0; i < num_files; i++) {
        printf("| %-4s | %-5d | %-4d | ", files[i].filename, files[i].start_block, files[i].size);
        
        // Print linked list of blocks
        for (j = 0; j < files[i].size; j++) {
            printf("%d", files[i].blocks[j]);
            if (j < files[i].size - 1) {
                printf("-->");
            }
        }
        printf(" |\n");
        printf("--------------------------------------\n");
    }
}

void indexed_allocation() {
    int num_files, i, j;
    IndexedFile files[MAX_FILES];
    int blocks_occupied;
    
    printf("\n----- INDEXED ALLOCATION -----\n");
    printf("Enter no. of files: ");
    scanf("%d", &num_files);
    
    // Input file details
    for (i = 0; i < num_files; i++) {
        sprintf(files[i].filename, "%d", i+1);  // Filenames are 1, 2, 3...
        
        printf("Enter starting block and size of file%d:\n", i+1);
        scanf("%d", &files[i].index_block);
        scanf("%d", &files[i].length);
        
        printf("Enter blocks occupied by file%d: ", i+1);
        scanf("%d", &blocks_occupied);
        
        printf("Enter blocks of file%d: ", i+1);
        for (j = 0; j < blocks_occupied; j++) {
            scanf("%d", &files[i].blocks[j]);
        }
    }
    
    // Display file allocation table
    printf("-------------------------\n");
    printf("| File | Index | Length |\n");
    printf("-------------------------\n");
    
    for (i = 0; i < num_files; i++) {
        printf("| %-4s | %-5d | %-6d |\n", files[i].filename, files[i].index_block, files[i].length);
        printf("-------------------------\n");
    }
    
    // Option to display index block contents
    int file_num;
    printf("Enter file number to view index block contents (0 to skip): ");
    scanf("%d", &file_num);
    
    if (file_num > 0 && file_num <= num_files) {
        i = file_num - 1;  // Adjust to zero-based index
        printf("Index block contents for file %s:\n", files[i].filename);
        printf("Block#\tBlock Allocated\n");
        for (j = 0; j < files[i].length; j++) {
            printf("%d\t%d\n", j, files[i].blocks[j]);
        }
    }
}
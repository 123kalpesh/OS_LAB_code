/*Producer-Consumer problem using Semaphore*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_ITEMS 5  // Each producer produces this many items

// Shared buffer
int buffer[BUFFER_SIZE];
int in = 0;  // Index where producer will put next item
int out = 0; // Index where consumer will get next item

// Semaphores
sem_t empty;     // Counts empty buffer slots
sem_t full;      // Counts full buffer slots
sem_t mutex;     // Binary semaphore for mutual exclusion

// Function to display buffer contents
void display_buffer() {
    printf("Buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");
}

// Producer thread function
void *producer(void *arg) {
    int producer_id = *((int *)arg);
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        // Generate a random item
        int item = rand() % 100;
        
        // Wait if buffer is full
        sem_wait(&empty);
        
        // Get exclusive access to buffer
        sem_wait(&mutex);
        
        // Add item to buffer
        buffer[in] = item;
        printf("Producer %d produced item %d at position %d\n", producer_id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        display_buffer();
        
        // Release buffer
        sem_post(&mutex);
        
        // Signal that buffer has an item
        sem_post(&full);
        
        // Sleep for a random amount of time
        usleep(rand() % 500000);
    }
    
    printf("Producer %d finished production\n", producer_id);
    pthread_exit(NULL);
}

// Consumer thread function
void *consumer(void *arg) {
    int consumer_id = *((int *)arg);
    
    for (int i = 0; i < (NUM_ITEMS * NUM_PRODUCERS) / NUM_CONSUMERS; i++) {
        // Wait if buffer is empty
        sem_wait(&full);
        
        // Get exclusive access to buffer
        sem_wait(&mutex);
        
        // Remove item from buffer
        int item = buffer[out];
        printf("Consumer %d consumed item %d from position %d\n", consumer_id, item, out);
        buffer[out] = -1;  // Mark as empty (just for display purposes)
        out = (out + 1) % BUFFER_SIZE;
        display_buffer();
        
        // Release buffer
        sem_post(&mutex);
        
        // Signal that buffer has an empty slot
        sem_post(&empty);
        
        // Sleep for a random amount of time
        usleep(rand() % 500000);
    }
    
    printf("Consumer %d finished consumption\n", consumer_id);
    pthread_exit(NULL);
}

int main() {
    // Initialize the random seed
    srand(time(NULL));
    
    // Initialize buffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = -1;  // -1 indicates empty slot
    }
    
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // Buffer starts empty
    sem_init(&full, 0, 0);             // Buffer starts with no items
    sem_init(&mutex, 0, 1);            // Binary semaphore (mutex) starts unlocked
    
    // Create producer and consumer threads
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];
    
    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i + 1;
        if (pthread_create(&producers[i], NULL, producer, &producer_ids[i]) != 0) {
            perror("Failed to create producer thread");
            exit(EXIT_FAILURE);
        }
    }
    
    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i + 1;
        if (pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]) != 0) {
            perror("Failed to create consumer thread");
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for producer threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        if (pthread_join(producers[i], NULL) != 0) {
            perror("Failed to join producer thread");
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for consumer threads to finish
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        if (pthread_join(consumers[i], NULL) != 0) {
            perror("Failed to join consumer thread");
            exit(EXIT_FAILURE);
        }
    }
    
    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    
    printf("All threads completed successfully\n");
    
    return 0;
}

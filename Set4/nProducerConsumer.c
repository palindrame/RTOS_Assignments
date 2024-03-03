#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 10
#define MAX_PRODUCERS 10
#define MAX_CONSUMERS 10

// Circular Queue
int queue[MAX_QUEUE_SIZE];
int front = 0, rear = 0;

// Mutex and Condition Variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

// Producer and Consumer Threads
pthread_t producers[MAX_PRODUCERS];
pthread_t consumers[MAX_CONSUMERS];
int num_producers = 0, num_consumers = 0;

// Function Declarations
void enQ(int item);
int deQ();
void *producer(void *data);
void *consumer(void *data);
void *manager(void *data);

// Main Function
int main() {
    pthread_t manager_thread;
    pthread_create(&manager_thread, NULL, manager, NULL);
    
    for (int i = 0; i < num_producers; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    
    for (int i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    pthread_join(manager_thread, NULL);
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    return 0;
}

// Enqueue an item into the queue
void enQ(int item) {
    pthread_mutex_lock(&mutex);
    while ((rear + 1) % MAX_QUEUE_SIZE == front) {
        pthread_cond_wait(&full, &mutex);
    }
    queue[rear] = item;
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
}

// Dequeue an item from the queue
int deQ() {
    pthread_mutex_lock(&mutex);
    while (front == rear) {
        pthread_cond_wait(&empty, &mutex);
    }
    int item = queue[front];
    front = (front + 1) % MAX_QUEUE_SIZE;
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex);
    return item;
}

// Producer Thread Function
void *producer(void *data) {
    while (1) {
        int item = rand() % 100; // Generate a random item
        enQ(item);
        printf("Produced: %d\n", item);
        usleep(100000); // Sleep for 0.1 seconds
    }
    return NULL;
}

// Consumer Thread Function
void *consumer(void *data) {
    while (1) {
        int item = deQ();
        printf("Consumed: %d\n", item);
        usleep(100000); // Sleep for 0.1 seconds
    }
    return NULL;
}

// Manager Thread Function
void *manager(void *data) {
    char choice;
    while (1) {
        printf("Enter choice: ");
        scanf(" %c", &choice);
        switch (choice) {
            case 'p':
                if (num_producers < MAX_PRODUCERS) {
                    num_producers++;
                    pthread_create(&producers[num_producers - 1], NULL, producer, NULL);
                }
                break;
            case 'c':
                if (num_consumers < MAX_CONSUMERS) {
                    num_consumers++;
                    pthread_create(&consumers[num_consumers - 1], NULL, consumer, NULL);
                }
                break;
            case 'q':
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
    return NULL;
}


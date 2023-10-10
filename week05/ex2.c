#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct Thread {
    pthread_t id;
    int i;
    char message[256];
};

void *thread_func(void *arg) {
    struct Thread *thread_data = (struct Thread*)arg;
    printf("Thread with id %lu prints: %s\n", thread_data->id, thread_data->message);
    pthread_exit(NULL);
}

int main(int args, char* argv[]) {
    int n = 3;
    if (args > 1) {
        n = atoi(argv[1]);
    }

    struct Thread threads[n];
    for (int i = 0; i < n; i++) {
        threads[i].i = i;
        snprintf(threads[i].message, sizeof(threads[i].message), "Hello from thread %d", threads[i].i);

        pthread_create(&threads[i].id, NULL, thread_func, &threads[i]);
        printf("Thread %d is created\n", threads[i].i);

        pthread_join(threads[i].id, NULL);
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// primality test
bool is_prime(int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Primes counter in [a , b)
int primes_count(int a, int b) {
    int ret = 0;
    for (int i = a; i < b; i++) {
        if (is_prime(i))
            ret++;
    }
    return ret;
}

// argument to the start_routine of the thread
typedef struct prime_request {
    int a, b;
} prime_request;

int total_prime_count = 0;

void *prime_counter(void *arg) {
    prime_request *req = (prime_request *)arg;
    int count = primes_count(req->a, req->b);
    total_prime_count += count;

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <m>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];
    prime_request requests[m];

    int interval_size = n / m;
    for (int i = 0; i < m; i++) {
        requests[i].a = i * interval_size;
        requests[i].b = (i == m - 1) ? n : (i + 1) * interval_size;
        pthread_create(&threads[i], NULL, prime_counter, &requests[i]);
    }

    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total primes in [0, %d) = %d\n", n, total_prime_count);

    return 0;
}


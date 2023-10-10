#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

int k = 2;
int c = 0; 
int n;

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return false;
    }
    return true;
}

int get_number_to_check() {
    int ret;
    pthread_mutex_lock(&global_lock);
    ret = k;
    if (k != n)
        k++;
    pthread_mutex_unlock(&global_lock);
    return ret;
}

void increment_primes() {
    pthread_mutex_lock(&global_lock);
    c++;
    pthread_mutex_unlock(&global_lock);
}

void *check_primes(void *arg) {
    while (1) {
        int num = get_number_to_check();
        if (num >= n)
            break;

        if (is_prime(num)) {
            increment_primes();
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <m>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];
    
    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, check_primes, NULL);
    }
    
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total primes in [0, %d) = %d\n", n, c);

    return 0;
}


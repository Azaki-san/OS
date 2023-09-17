#include <stdio.h>

int tribonacci(int n) {
    int a = 0;
    int b = 1;
    int c = 1;
    if (n == 0) {
        return a;
    } else if (n == 1 || n == 2) {
        return b;
    } else {
        int res;
        for (int i = 2; i < n; i++) {
            res = c + b + a;
            a = b;
            b = c;
            c = res;
        }
        return res;
    }


}
int main(void) {
    printf("%d\n%d\n", tribonacci(4), tribonacci(36));
    return 0;
}

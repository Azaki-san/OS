#include <stdio.h>

long long powFunc(int system, int systemPow) {
    int res = 1;
    for (int i = 0; i < systemPow; i++) {
        res *= system;
    }
    return (long long) res;
}

int calculateDigits(long long number) {
    int count = 0;
    while (number > 0) {
        count++;
        number /= 10;
    }
    return count;
}

int calculateDigitsInConverted(long long number, int system) {
    int count = 0;
    while (number > 0) {
        count++;
        number /= system;
    }
    return count;
}

long long to_decimal(long long number, int system) {
    long long resultInDecimal = 0;
    int systemPow = 0;
    while (number > 0) {
        long long currentDigit = number % 10;
        if (currentDigit >= system) {
            return -1;
        }
        number /= 10;
        resultInDecimal += currentDigit * powFunc(system, systemPow);
        systemPow++;
    }
    return resultInDecimal;
}

long long convert(long long numberInSource, int sourceSystem, int targetSystem) {
    long long number = to_decimal(numberInSource, sourceSystem);
    if (sourceSystem > 10 || sourceSystem < 2 || targetSystem > 10 || targetSystem < 2 || number == -1) {
        return -1;
    }
    char numberString[calculateDigits(number)];
    char numberResult[calculateDigitsInConverted(number, targetSystem)];
    long long res;
    sprintf(numberString, "%lld", number);
    int index = 0;
    while (number > 0) {
        for (int j = index; j >= 1; j--) {
            numberResult[j] = numberResult[j - 1];
        }
        numberResult[0] = '0' + (number % targetSystem);
        number /= targetSystem;
        index++;
    }
    sscanf(numberResult, "%lld", &res);
    return res;
}

int main(void) {
    long long number;
    int sourceSystem;
    int targetSystem;
    scanf("%lld %d %d", &number, &sourceSystem, &targetSystem);
    long long result = convert(number, sourceSystem, targetSystem);
    if (result == -1) {
        printf("cannot convert!\n");
    } else {
        printf("%lld\n", result);
    }

    return 0;
}

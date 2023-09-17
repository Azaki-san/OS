#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

int count(const char string[], int length, char character) {
    int res = 0;
    for (int i = 0; i < length; i++) {
        if (string[i] == character) {
            res++;
        }
    }
    return res;
}

int countAll(const char string[], int length) {
    for (int i = 0; i < length; i++) {
        if (i != length - 1) {
            printf("%c:%d, ", string[i], count(string, length, string[i]));
        } else {
            printf("%c:%d\n", string[i], count(string, length, string[i]));
        }
    }
}

int main(void) {
    char stringInput[MAX_LENGTH];
    char string[MAX_LENGTH];
    char character;
    int flag = 0;
    int length = 0;
    fgets(stringInput, MAX_LENGTH, stdin);
    for (int i = 0; i < strlen(stringInput); i++) {
        char currentSymbol = stringInput[i];
        if (currentSymbol != ',' && currentSymbol != '\n') {
            string[i] = tolower(currentSymbol);
            length++;
        } else if (currentSymbol == ',') {
            character = tolower(stringInput[i + 2]);
            flag = 1;
        } else {
            break;
        }
    }
    if (flag == 1) {
        int countOfOccurrences = count(string, length, character);
        printf("%c:%d\n", character, countOfOccurrences);
    } else {
        countAll(string, length);
    }

    return 0;
}

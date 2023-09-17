#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 256

int main() {
   char stringInput[MAX_LENGTH];
   char stringOutput[MAX_LENGTH];
   fgets(stringInput, MAX_LENGTH, stdin);
   for (int i = 0; i < strlen(stringInput); i++) {
       char currentSymbol = stringInput[i];
       if (currentSymbol != '.' && currentSymbol != '\n') {
           for (int j = i; j >= 1; j--) {
               stringOutput[j] = stringOutput[j - 1];
           }
           stringOutput[0] = currentSymbol;
       } else {
           break;
       }
   }
   printf("%s\n", stringOutput);

   return 0;
}

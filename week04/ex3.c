#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int args, char* argv[]){
    for (int i = 0; i < atoi(argv[1]); i++){
        fork();
       sleep(5);
    }
}

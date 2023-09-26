#include <stdio.h>
#include <string.h>

#define MAX_PATH 2048
#define MAX_FILE_NAME 63
#define MAX_FILE_DATA_SIZE 1024

struct File {
    unsigned int id;
    char name[MAX_FILE_NAME];
    unsigned int size;
    char data[MAX_FILE_DATA_SIZE];
    struct Directory* directory;
};

struct Directory {
    char name[MAX_FILE_NAME];
    struct File* files[MAX_PATH];
    struct Directory* subdirs[MAX_PATH];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH];
};

void overwriteToFile(struct File* file, const char* str) {
    if (file) {
        strcpy(file->data, str);
        file->size = strlen(str);
    }
}

void appendToFile(struct File* file, const char* str) {
    if (file) {
        strcat(file->data, str);
        file->size += strlen(str);
    }
}

void printpfile(struct File* file) {
    if (file) {
        printf("%s/%s\n", file->directory->path, file->name);
    }
}

void addFile(struct File* file, struct Directory* dir) {
    file->directory = dir;
    dir->files[dir->nf++] = file;
}

void addDir(struct Directory* dir1, struct Directory* dir2) {
    dir2->subdirs[dir2->nd++] = dir1;

    char tempPath[MAX_PATH];
    if (strcmp(dir2->path, "/")) {
        strcpy(tempPath, dir2->path);
        strcat(tempPath, "/");
        strcat(tempPath, dir1->name);
        strcpy(dir1->path, tempPath);
    } else {
        strcpy(tempPath, "/");
        strcat(tempPath, dir1->name);
        strcpy(dir1->path, tempPath);
    }
}

int main() {
    struct Directory root;
    strcpy(root.name, "/");
    root.nf = 0;
    root.nd = 0;
    strcpy(root.path, "/");

    struct Directory home;
    strcpy(home.name, "home");
    home.nf = 0;
    home.nd = 0;
    strcpy(home.path, "/home");

    struct Directory bin;
    strcpy(bin.name, "bin");
    bin.nf = 0;
    bin.nd = 0;
    strcpy(bin.path, "/bin");

    addDir(&home, &root);
    addDir(&bin, &root);

    struct File ex3_1;
    ex3_1.id = 1;
    strcpy(ex3_1.name, "ex3_1.c");
    ex3_1.size = strlen("int printf(const char * format, ...);");
    strcpy(ex3_1.data, "int printf(const char * format, ...);");
    addFile(&ex3_1, &home);

    struct File ex3_2;
    ex3_2.id = 2;
    strcpy(ex3_2.name, "ex3_2.c");
    ex3_2.size = strlen("//This is a comment in C language");
    strcpy(ex3_2.data, "//This is a comment in C language");
    addFile(&ex3_2, &home);

    struct File bash;
    bash.id = 3;
    strcpy(bash.name, "bash");
    bash.size = strlen("Bourne Again Shell!!");
    strcpy(bash.data, "Bourne Again Shell!!");
    addFile(&bash, &bin);

    appendToFile(&ex3_1, "int main(){printf(\"Hello World!\");}");

    printpfile(&ex3_1);
    printpfile(&ex3_2);
    printpfile(&bash);

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>

const char *watched_directory;

void find_all_hlinks(const char *source) {
    struct stat source_stat;
    if (lstat(source, &source_stat) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(watched_directory)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            struct stat file_stat;
            const char *entr_temp = entry->d_name;
            int size = snprintf(NULL, 0, "%s/%s", watched_directory, entr_temp);
            char *x = (char *) malloc(size + 1);
            snprintf(x, size + 1, "%s/%s", watched_directory, entr_temp);
            if (lstat(x, &file_stat) == -1) {
                perror("lstat");
                exit(EXIT_FAILURE);
            }

            if (file_stat.st_ino == source_stat.st_ino && file_stat.st_dev == source_stat.st_dev) {

                printf("Inode: %lu, Path: %s\n", file_stat.st_ino, x);
            }
        }
    }

    closedir(dir);
}

void unlink_all(const char *source) {
    struct stat source_stat;
    if (lstat(source, &source_stat) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(watched_directory)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {

        if (entry->d_type == DT_REG) {
            struct stat file_stat;
            const char *entr_temp = entry->d_name;
            int size = snprintf(NULL, 0, "%s/%s", watched_directory, entr_temp);
            char *x = (char *) malloc(size + 1);
            snprintf(x, size + 1, "%s/%s", watched_directory, entr_temp);
            if (lstat(x, &file_stat) == -1) {
                perror("lstat");
                exit(EXIT_FAILURE);
            }

            if (file_stat.st_ino == source_stat.st_ino && file_stat.st_dev == source_stat.st_dev) {
                printf("%s", x);
                if (unlink(x) == -1) {
                    perror("unlink");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    closedir(dir);

    char resolved_path[PATH_MAX];
    if (realpath(source, resolved_path) == NULL) {
        perror("realpath");
        exit(EXIT_FAILURE);
    }

    printf("Last hard link: %s\n", resolved_path);
}

void create_sym_link(const char *source, const char *link) {
    if (symlink(source, link) == -1) {
        perror("symlink");
        exit(EXIT_FAILURE);
    }
}

int copy_file(const char *source_path, const char *dest_path) {
    int source_fd = open(source_path, O_RDONLY);
    if (source_fd == -1) {
        perror("open source");
        return -1;
    }
    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dest_fd == -1) {
        perror("open dest");
        close(source_fd);
        return -1;
    }

    char buffer[4096];
    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("write");
            close(source_fd);
            close(dest_fd);
            return -1;
        }
    }

    if (bytes_read == -1) {
        perror("read");
        close(source_fd);
        close(dest_fd);
        return -1;
    }

    close(source_fd);
    close(dest_fd);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <watched_directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    watched_directory = argv[1];

    const char *myfile11_path_temp = "myfile11.txt";
    int size = snprintf(NULL, 0, "%s/%s", watched_directory, myfile11_path_temp);
    char *myfile11_path = (char *) malloc(size + 1);
    snprintf(myfile11_path, size + 1, "%s/%s", watched_directory, myfile11_path_temp);

    const char *myfile12_path_temp = "myfile12.txt";
    size = snprintf(NULL, 0, "%s/%s", watched_directory, myfile12_path_temp);
    char *myfile12_path = (char *) malloc(size + 1);
    snprintf(myfile12_path, size + 1, "%s/%s", watched_directory, myfile12_path_temp);

    const char *myfile1_path_temp = "myfile1.txt";
    size = snprintf(NULL, 0, "%s/%s", watched_directory, myfile1_path_temp);
    char *myfile1_path = (char *) malloc(size + 1);
    snprintf(myfile1_path, size + 1, "%s/%s", watched_directory, myfile1_path_temp);


    int fd = open(myfile1_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    write(fd, "Hello world.", 12);
    close(fd);

    link(myfile1_path, myfile11_path);
    link(myfile1_path, myfile12_path);

    printf("All hard links to myfile1.txt:\n");
    find_all_hlinks(myfile1_path);

    const char *new_path = "/tmp/myfile1.txt";
    if (copy_file(myfile1_path, new_path) == -1) {
        perror("copy_file");
        exit(EXIT_FAILURE);
    }

    if (unlink(myfile1_path) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    int myfile11_fd = open(myfile11_path, O_WRONLY | O_APPEND);
    if (myfile11_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    write(myfile11_fd, " Modification.", 15);
    close(myfile11_fd);

    const char *myfile13_path_temp = "myfile13.txt";
    size = snprintf(NULL, 0, "%s/%s", watched_directory, myfile13_path_temp);
    char *myfile13_path = (char *) malloc(size + 1);
    snprintf(myfile13_path, size + 1, "%s/%s", watched_directory, myfile13_path_temp);
    create_sym_link(new_path, myfile13_path);

    int tmp_file_fd = open(new_path, O_WRONLY | O_APPEND);
    if (tmp_file_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    write(tmp_file_fd, " Another modification.", 22);
    close(tmp_file_fd);
    unlink_all(new_path);

    return 0;
}

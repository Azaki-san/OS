#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
char *watched_directory;


void print_stat_info(const char *path) {
    struct stat stat_info;

    if (stat(path, &stat_info) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("%s is ", path);
    printf("node %lo, mode %o, size %ld, uid %d, gid %d\n",
           stat_info.st_ino, stat_info.st_mode, stat_info.st_size, stat_info.st_uid, stat_info.st_gid);
}

void print_tree(const char *path, int level) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char sub_path[PATH_MAX];
                snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);

                for (int i = 0; i < level; ++i) {
                    printf("  "); // Adjust the indentation according to your preference
                }

                printf("[%s] (directory)\n", entry->d_name);

                print_tree(sub_path, level + 1);
            }
        } else {
            char full_path[PATH_MAX];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

            for (int i = 0; i < level; ++i) {
                printf("  ");
            }

            print_stat_info(full_path);
        }
    }

    closedir(dir);
}

void handle_signal(int signum) {
    print_tree(watched_directory, 0);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    print_tree(argv[1], 0);
    int length, i = 0;
    int fd;
    char buffer[EVENT_BUF_LEN];

    fd = inotify_init();

    if (fd < 0) {
        perror("inotify_init");
    }

    int wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }
    watched_directory = argv[1];
    signal(SIGINT, handle_signal);
    while (1) {
        length = read(fd, buffer, EVENT_BUF_LEN);

        if (length < 0) {
            perror("read");
        }
        i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR) {
                        printf("New directory %s/%s created.\n", argv[1], event->name);
                    } else {
                        printf("New file %s/%s created.\n", argv[1], event->name);
                    }
                } else if (event->mask & IN_DELETE) {
                    if (event->mask & IN_ISDIR) {
                        printf("Directory %s/%s deleted.\n", argv[1], event->name);
                    } else {
                        printf("File %s/%s deleted.\n", argv[1], event->name);
                    }
                } else if (event->mask & IN_ACCESS) {
                    if (event->mask & IN_ISDIR) {
                        printf("New directory %s/%s accessed.\n", argv[1], event->name);
                    } else {
                        printf("New file %s/%s accessed.\n", argv[1], event->name);
                    }
                }else if (event->mask & IN_MODIFY) {
                    if (event->mask & IN_ISDIR) {
                        printf("New directory %s/%s modified.\n", argv[1], event->name);
                    } else {
                        printf("New file %s/%s modified.\n", argv[1], event->name);
                    }
                }else if (event->mask & IN_OPEN) {
                    if (event->mask & IN_ISDIR) {
                        printf("New directory %s/%s opened.\n", argv[1], event->name);
                    } else {
                        printf("New file %s/%s opened.\n", argv[1], event->name);
                    }
                }else if (event->mask & IN_ATTRIB) {
                    if (event->mask & IN_ISDIR) {
                        printf("Metadata changed.\n");
                    } else {
                        printf("Metadata changed.\n");
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}
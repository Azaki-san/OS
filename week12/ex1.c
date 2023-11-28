#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define INPUT_DEVICE_FILE "/dev/input/by-path/platform-i8042-serio-0-event-kbd"
FILE *file;


// Function for printing the events, which happened with the key
// It can be released, pressed or repeated!
void print_event(struct input_event *event) {
    if (event->value == 0) {
        fprintf(file, "RELEASED 0x%04x (%d)\n", event->code, event->code);
        printf("RELEASED 0x%04x (%d)\n", event->code, event->code);
    } else if (event->value == 1) {
        fprintf(file, "PRESSED 0x%04x (%d)\n", event->code, event->code);
        printf("PRESSED 0x%04x (%d)\n", event->code, event->code);
    } else if (event->value == 2) {
        fprintf(file, "REPEATED 0x%04x (%d)\n", event->code, event->code);
        printf("REPEATED 0x%04x (%d)\n", event->code, event->code);
    }
}

// Function for checking if shortcut is pressed
int check_shortcut(int size, int *pressed_keys) {
    for (int i = 0; i < size; ++i) {
        if (pressed_keys[i] == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int fd;
    struct input_event event;

    fd = open(INPUT_DEVICE_FILE, O_RDONLY);
    file = fopen("ex1.txt", "w");

    printf("Monitoring keyboard events. Available shortcuts:\n");
    printf("0. EX - Exit\n");
    printf("1. PE - I passed the Exam!\n");
    printf("2. CAP - Get some cappuccino!\n");
    printf("3. LA - Random phrase!!\n");
    printf("4. FPGA - Field-programmable gate array\n");

    int p_pressed = 0;
    int a_pressed = 0;
    int e_pressed = 0;
    int x_pressed = 0;
    int c_pressed = 0;
    int l_pressed = 0;
    int f_pressed = 0;
    int g_pressed = 0;

    while (1) {
        if (read(fd, &event, sizeof(struct input_event)) == sizeof(struct input_event)) {
            if (event.type == EV_KEY && (event.value == 0 || event.value == 1 || event.value == 2)) {
                print_event(&event);
		// check if some keys are pressed
                if (event.code == KEY_P) {
                    p_pressed = event.value;
                } else if (event.code == KEY_A) {
                    a_pressed = event.value;
                } else if (event.code == KEY_C) {
                    c_pressed = event.value;
                } else if (event.code == KEY_L) {
                    l_pressed = event.value;
                } else if (event.code == KEY_E) {
                    e_pressed = event.value;
                } else if (event.code == KEY_X) {
                    x_pressed = event.value;
                } else if (event.code == KEY_F) {
                    f_pressed = event.value;
                } else if (event.code == KEY_G) {
                    g_pressed = event.value;
                }
                // checking if we need to print something because of pressed shortcut
                if (check_shortcut(2, (int[]){e_pressed, x_pressed})) {
                   printf("Exiting...\n");
                   fprintf(file, "Exiting...\n");
                   return 0;
                }

                if (check_shortcut(2, (int[]){p_pressed, e_pressed})) {
                    printf("I passed the Exam!\n");
                    fprintf(file, "I passed the Exam!\n");
                }

                if (check_shortcut(3, (int[]){c_pressed, a_pressed, p_pressed})) {
                    printf("Get some cappuccino!\n");
                    fprintf(file, "Get some cappuccino!\n");
                }

                if (check_shortcut(2, (int[]){l_pressed, a_pressed})) {
                    printf("Random phrase!!\n");
                    fprintf(file, "Random phrase!!\n");
                }
                
                if (check_shortcut(4, (int[]){f_pressed, p_pressed, g_pressed, a_pressed})) {
                    printf("Field-programmable gate array\n");
                }
            }
        }
    }
    close(fd);

    return 0;
}


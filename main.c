#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#define DEVICE_PATH "/dev/gpu_driver"

int set_background_color (int R, int G, int B) {
    int fd = open(DEVICE_PATH, O_WRONLY);

    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    unsigned char command[9] = {0};
    int reg = 0b00000; // Register number (5 bits)
    //int r = 0b111;     // Red color value (3 bits)
    //int g = 0b000;     // Green color value (3 bits)
    //int b = 0b000;     // Blue color value (3 bits)

    // Construct the command
    command[0] = 0; // Reserved for future use
    command[1] = reg;
    command[2] = R;
    command[3] = G;
    command[4] = B;

    // THEM QUE COLOCAR ISSO NO CODIGO PRINCIPAL
    /*fd = open(DEVICE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open the device file");
        return 1;
    }*/

    // Write the command to the device
    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return 0;
    }

    printf("Command sent to device: register=%d, r=%d, g=%d, b=%d\n", reg, R, G, B);

    close(fd);

    return 1;
}

int main () {

    set_background_color(7, 7, 7);
    return 0;
}

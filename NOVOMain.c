#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

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
    command[1] = B;
    command[2] = G;
    command[3] = R;
    
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

int set_sprite(int registrador, int x, int y, int offset, int sp) {
    int fd = open(DEVICE_PATH, O_WRONLY);

    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    unsigned char command[9] = {0};

    // Construct the command
    command[0] = 1; // Reserved for future use
    command[1] = registrador;
    command[2] = offset;
    command[3] = x;
    command[4] = y;
    command[5] = sp;
    
    // Write the command to the device
    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return 0;
    }

    printf("Command sent to device: register=%d, offset=%d, x=%d, y=%d\n", registrador, offset, x, y);

    close(fd);
    return 1;
}

int set_poligono(int address, int ref_x, int ref_y, int size, int r, int g, int b, int shape) {

    int fd = open(DEVICE_PATH, O_WRONLY);

    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    unsigned char command[9] = {0};

    // Construct the command
    command[0] = 4;  // DP
    command[1] = address;
    command[2] = ref_x;
    command[3] = ref_y;
    command[4] = size;
    command[5] = r;
    command[6] = g;
    command[7] = b;
    command[8] = shape;
    
    // Write the command to the device
    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return 0;
    }
    printf("Command sent to device: address=%d, ref_x=%d, ref_y=%d, size=%d, r=%d g=%d b=%d shape=%d\n", address, ref_x, ref_y, size,r,g,b,shape );
    close(fd);
    return 1;
}


int set_background_block(int address, int R, int G, int B) {
    int fd = open(DEVICE_PATH, O_WRONLY);

    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    unsigned char command[9] = {0};

    // Construct the command
    command[0] = 2; // Reserved for future use
    command[1] = address;
    command[2] = R;
    command[3] = G;
    command[4] = B;
    
    // Write the command to the device
    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return 0;
    }
    
    printf("Command sent to device: addres=%d, R=%d, G=%d, B=%d\n", address, R, G, B);

    close(fd);
    return 1;
}

int main () {
    set_poligono(6, 50, 50, 9, 7, 0, 0, 1);
    set_poligono(7, 50, 150, 9, 0, 7, 0, 0);
    set_poligono(9, 50, 200, 1, 7, 0, 7, 0);
  
   
    return 0;
}

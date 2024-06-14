#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define LEFT 0
#define RIGHT 4
#define UP 2
#define DOWN 6
#define UPPER_RIGHT 1
#define UPPER_LEFT 3
#define BOTTOM_LEFT 5
#define BOTTOM_RIGHT 7

#define DEVICE_PATH "/dev/gpu_driver"

typedef struct {
    int pos_X;
    int pos_Y;
    int direction;
    int offset;
    int register_val; 
    int mov_X;
    int mov_Y;
    int enable;
    int collision;
}Sprite;

typedef struct {
 int pos_x;
 int pos_Y;
 int offset;
 int register_val;
 int enable;
 } Sprite_Fixed;

typedef struct {
    int ref_X;
    int ref_Y;
    int address;
    int register_val; 
    int size;
    int R;
    int G;
    int B;
    int shape;
} Poligono;

int reset_poligonos(){
    int i = 0;
    for (i; i< 16; i++){
        set_poligono(i, 0, 0, 0, 0, 0, 0, 0);
    }
    return 0;
}

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

    char new_reg = (char) registrador;
    char new_x = (char) x;
    char new_y = (char) y;
    char new_offset = (char) offset;
    char new_sp = (char) sp;

    unsigned char command[9] = {0};

    // Construct the command
    command[0] = 1; // Reserved for future use
    command[1] = new_reg;
    command[2] = new_offset;
    command[3] = new_x;
    command[4] = new_y;
    command[5] = new_sp;
    
    // Write the command to the device
    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return 0;
    }



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

Sprite_Fixed create_fixed_sprite(int pos_x, int pos_Y, int offset, int register_val, int enable) {
    Sprite_Fixed new_fixed_sprite;

    new_fixed_sprite.pos_x = pos_x;
    new_fixed_sprite.pos_Y = pos_Y;
    new_fixed_sprite.offset = offset;
    new_fixed_sprite.register_val = register_val;
    new_fixed_sprite.enable = enable;

    set_sprite(new_fixed_sprite.register_val, new_fixed_sprite.pos_x, new_fixed_sprite.pos_Y, new_fixed_sprite.offset, new_fixed_sprite.enable);

    return new_fixed_sprite;
}

Poligono create_poligono(int ref_X, int ref_Y, int address, int size, int R, int G, int B, int shape) {
    Poligono new_poligono;

    new_poligono.ref_X = ref_X;
    new_poligono.ref_Y = ref_Y;
    new_poligono.address = address;
    new_poligono.size = size;
    new_poligono.R = R;
    new_poligono.G = G;
    new_poligono.B = B;
    new_poligono.shape = shape;

    set_poligono(new_poligono.address, new_poligono.ref_X, new_poligono.ref_Y, new_poligono.size, new_poligono.R, new_poligono.G, new_poligono.B, new_poligono.shape);

    return new_poligono;
}

Sprite create_moving_sprite(int pos_X, int pos_Y, int direction, int offset, int register_val, 
                           int mov_x, int mov_y, int enable, int collision) {

    Sprite new_sprite;
    new_sprite.pos_X = pos_X;
    
    new_sprite.pos_Y = pos_Y;
    new_sprite.direction = direction;
    new_sprite.offset = offset;
    new_sprite.register_val = register_val;
    new_sprite.mov_X = mov_x;
    new_sprite.mov_Y = mov_y;
    new_sprite.enable = enable;
    new_sprite.collision = collision;
    

    set_sprite(new_sprite.register_val, new_sprite.pos_X, new_sprite.pos_Y, new_sprite.offset, new_sprite.enable);

    return new_sprite;
}

int move_sprite (Sprite *sp, int mirror, int move_direction) {
    if (move_direction == LEFT){
        (*sp).pos_X -= (*sp).mov_X;
    } else if (move_direction == RIGHT){
        (*sp).pos_X += (*sp).mov_X;
    } else if (move_direction == UP){
        (*sp).pos_Y -= (*sp).mov_Y;
    } else if (move_direction == DOWN){
        (*sp).pos_Y += (*sp).mov_Y;
    } else if (move_direction == UPPER_RIGHT){
        (*sp).pos_Y -= (*sp).mov_Y;
        (*sp).pos_X += (*sp).mov_X;
    } else if (move_direction == UPPER_LEFT){
        (*sp).pos_Y -= (*sp).mov_Y;
        (*sp).pos_X -= (*sp).mov_X;
    } else if (move_direction == BOTTOM_LEFT){
        (*sp).pos_Y += (*sp).mov_Y;
        (*sp).pos_X -= (*sp).mov_X;
    } else if (move_direction == BOTTOM_LEFT){
        (*sp).pos_Y += (*sp).mov_Y;
        (*sp).pos_X += (*sp).mov_X;
    }

    set_sprite((*sp).register_val, (*sp).pos_X, (*sp).pos_Y, (*sp).offset, (*sp).enable);
    return 0;
}


int main () {
    set_sprite(1, 300, 256, 1, 1);
    return 0;
}

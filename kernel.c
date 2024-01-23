#include<stdint.h>
void print_char(char character, int row, int col, uint8_t color) {
    volatile uint16_t *video_memory = (uint16_t *)0xb8000;
    int offset = (row * 80 + col);
    video_memory[offset] = (uint16_t)(character | ((uint16_t)color << 8));
}

void print_string(const char *str, int row, int col, uint8_t color) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i], row, col + i, color);
    }
}


void kernel_main() {

    print_string("Hello, kernel World!", 0, 0, 0x03);
}

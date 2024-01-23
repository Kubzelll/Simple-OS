#include<stdint.h>
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

void clear_screen() {
    volatile uint16_t* video_memory = (uint16_t*)VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
        video_memory[i] = 0;
    }
}

void print_char(char character, int x, int y, uint8_t color) {
    volatile uint16_t* video_memory = (uint16_t*)VGA_MEMORY;
    int offset = y * VGA_WIDTH + x;
    video_memory[offset] = (uint16_t)character | ((uint16_t)color << 8);
}

void print_string(const char* str, int x, int y, uint8_t color) {
    int i = 0;
    while (str[i] != '\0') {
        print_char(str[i], x, y, color);
        x++;
        i++;
    }
}

char get_input() {
    char key;
    asm volatile (
        "int $0x16\n"
        : "=a" (key)
        : "a" (0x00)
    );
    return key;
}

void draw_button(const char* label, int x, int y, uint8_t color, uint8_t bg_color) {
    int label_length = 0;
    while (label[label_length] != '\0') {
        label_length++;
    }

    for (int i = 0; i < label_length + 4; ++i) {
        print_char('-', x + i, y, color);
        print_char('-', x + i, y + 2, color);
    }
    for (int i = 0; i < 3; ++i) {
        print_char('|', x, y + i, color);
        print_char('|', x + label_length + 3, y + i, color);
    }

    print_string(label, x + 2, y + 1, color);
}

void kernel_main() {

    print_string("Welcome to Simple-OS by Kubzel", 0, 0, 0x03);
    print_string("-------------------\n", 0, 1, 0x0F);

    int selected_option = 0;

    while (1){
        draw_button("Terminal", 2, 3, (selected_option == 0) ? 0x1F : 0x0F, 0x00);
        draw_button("Credits", 15, 3, (selected_option == 2) ? 0x1F : 0x0F, 0x00);

    }
}

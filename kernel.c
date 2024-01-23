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

char check_q_key() {
    char key;
    asm volatile (
        "int $0x16\n"
        : "=a" (key)
        : "a" (0x00)
    );
    return key;
}

void restart_os() {
    asm volatile (
        "int $0x19\n"
    );
}

void kernel_main() {

    
    print_string("Hello, kernel World!", 0, 0, 0x03);
    while (1){
	char key = check_q_key();

	if(key == 'Q' || key == 'q'){
		restart_os();
	}
    }
}

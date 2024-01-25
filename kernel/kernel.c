#include<stdint.h>
#include "types.h"
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000
#define KEYBOARD_PORT 0x60

void sleep(uint32 delay)
{
    while(1){
        asm volatile("nop");
        delay--;
        if(delay <= 0)
            break;
    }
}


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
void restart_os() {
    asm volatile (
        "int $0x19\n"  
    );
}
void shutdown_system() {
    //acpi only for x86
    asm volatile (
        "mov $0x200, %ax\n"
        "mov $0x0, %ebx\n"
        "int $0x15\n"
    );
}

uint8 inb(uint16 port)
{
  uint8 ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
  return ret;
}

char get_input_keycode()
{
  char ch = 0;
  while((ch = inb(KEYBOARD_PORT)) != 0){
    if(ch > 0)
      return ch;
  }
  return ch;
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


void terminal_window(){
    print_string("In the future there will be Simple-OS terminal", 0, 0, 0x04);
}


void kernel_main() {

    //Added sleep
    sleep(0x2FFFFFFF);
    print_string("Welcome to Simple-OS by Kubzel", 0, 0, 0x03);
    print_string("-------------------\n", 0, 1, 0x03);
    int selected_option = 0;
    int rendered_window = 0;

    while (1){
        if (rendered_window == 0){
            draw_button("Terminal", 2, 3, (selected_option == 0) ? 0x1F : 0x0F, 0x00);
            draw_button("Shut down", 14, 3, (selected_option == 1) ? 0x1F : 0x0F, 0x00);

            char key = get_input_keycode();
            
            switch (key)
            {
            case '\0':
                break;
            case 0x4B:
                selected_option = (selected_option > 0) ? selected_option - 1 : selected_option;
                break;
            case 0x4D:
                selected_option = (selected_option < 1) ? selected_option + 1 : selected_option;
                break;
            case 0x1C:
                print_string("Enter!", 25, 25, 0x0F);
                switch (selected_option)
                {
                case 0:
                    clear_screen();
                    rendered_window = 1;
                    break;
                case 1:
                    shutdown_system();
                default:
                    break;
                }
            default:
                break;
            }
        }else if (rendered_window == 1)
        {
            terminal_window();
        }
        
    }

}

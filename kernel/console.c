#include "types.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define DEFAULT_FONT 0x1F

U16* video_mem;
U32 x;
U32 y;

void enable_cursor(U8 cursor_start, U8 cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (insb(0x3D5) & 0xC0) | cursor_start);
    
    outb(0x3D4, 0x0B);
    outb(0x3D5, (insb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(U32 x, U32 y)
{
    U16 pos = (y * VGA_WIDTH) + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (U8) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (U8) ((pos >> 8) & 0xFF));
}

void ConsoleInstall()
{
    video_mem = (U16*)0xB8000;
    enable_cursor(0, 15);
    clear();
}

void scroll()
{
    for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; ++i)
    {
        video_mem[i] = video_mem[i + VGA_WIDTH];
    }
}

void put_cxy(char c, int x, int y)
{
	video_mem[(y * VGA_WIDTH) + x] = DEFAULT_FONT << 8 | c;
}

void put_c(char c)
{
    if (x >= VGA_WIDTH)
    {
        x = 0;
        y++;
    }

    if (y >= VGA_HEIGHT)
    {
        clear();
    }

    if (c == '\n')
    {
        x = 0;
        y++;
        update_cursor(x, y);
    
    } else
    {
        video_mem[(y * VGA_WIDTH) + x] = DEFAULT_FONT << 8 | c;
        x++;
        update_cursor(x, y);
    }
}

void clear()
{
    x = 0;
    y = 0;

    for (U32 i = 0; i < VGA_HEIGHT; ++i)
    {
        for (U32 j = 0; j < VGA_WIDTH; ++j)
        {
            put_c(' ');
        }
    }

    x = 0;
    y = 0;

}

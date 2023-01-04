#include <stdio.h>
#include <ncurses.h>

#include "../headers/draw.h"

int main()
{
    init_curses();
    init_header();
    WINDOW *footer = newwin(FOOTER_HEIGHT, TERMINAL_WIDTH, TERMINAL_HEIGHT - 1, 0); // init footer
    int play_again;
    do {
        play_again = footer_playagain(footer);
    } while(play_again == 1);
    
    uninit_curses();
    return 0;
}
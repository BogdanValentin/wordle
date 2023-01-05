#include <stdio.h>
#include <ncurses.h>

#include "../headers/draw.h"

int main()
{
    init_curses();
    init_header();
    WINDOW *footer = newwin(FOOTER_HEIGHT, TERMINAL_WIDTH, TERMINAL_HEIGHT - 1, 0);
    do {
        werase(footer);
        wrefresh(footer);
        char *word = malloc(6 * sizeof(char));
        pick_word(word);

        init_mainwindow(footer, word);

        free(word);
    } while(footer_playagain(footer) == 1);
    
    uninit_curses();
    return 0;
}
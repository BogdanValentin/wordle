#include <ncurses.h>

#include "ncurses.h"

#define TERMINAL_HEIGHT 20
#define TERMINAL_WIDTH  40

#define HEADER_HEIGHT 4

void init_curses() {
    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(0, COLOR_CYAN, COLOR_BLACK);          // default text color
    init_pair(1, COLOR_BLACK, COLOR_GREEN);         // litera pe poz corecta
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);        // litera pe poz gresita
    init_pair(3, COLOR_WHITE, COLOR_BLACK);         // litera incorecta
    curs_set(0);                                    // cursor invizibil
}

void uninit_curses() {
    endwin();
}

void init_header() {
    WINDOW *header = newwin(HEADER_HEIGHT, TERMINAL_WIDTH, 0, 0);
    wattron(header, COLOR_PAIR(0));
    mvwprintw(header, 0, 0, "   _ _ _ _____ _____ ____  __    _____");
    mvwprintw(header, 1, 0, "  | | | |     | __  |    \\|  |  |   __|");
    mvwprintw(header, 2, 0, "  | | | |  |  |    -|  |  |  |__|   __|");
    mvwprintw(header, 3, 0, "  |_____|_____|__|__|____/|_____|_____|");
    wattroff(header, COLOR_PAIR(0));
    wgetch(header);
    wrefresh(header);
}

void init_footer() {

}

void init_mainwindow() {

}


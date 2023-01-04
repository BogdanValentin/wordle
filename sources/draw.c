#include <ncurses.h>

#include "../headers/draw.h"

void init_curses() {
    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);          // default text color
    init_pair(2, COLOR_BLACK, COLOR_GREEN);         // litera pe poz corecta
    init_pair(3, COLOR_BLUE, COLOR_YELLOW);         // litera pe poz gresita
    init_pair(4, COLOR_WHITE, COLOR_BLACK);         // litera incorecta
    init_pair(10, COLOR_RED, COLOR_BLACK);          // cuvant invalid
    curs_set(0);                                    // cursor invizibil
}

void uninit_curses() {
    endwin();
}

void init_header() {
    WINDOW *header = newwin(HEADER_HEIGHT, TERMINAL_WIDTH, 0, 0);
    wattron(header, COLOR_PAIR(1));
    mvwprintw(header, 0, 0, "   _ _ _ _____ _____ ____  __    _____ ");
    mvwprintw(header, 1, 0, "  | | | |     | __  |    \\|  |  |   __|");
    mvwprintw(header, 2, 0, "  | | | |  |  |    -|  |  |  |__|   __|");
    mvwprintw(header, 3, 0, "  |_____|_____|__|__|____/|_____|_____|");
    wattroff(header, COLOR_PAIR(1));
    wrefresh(header);
}

void footer_error(WINDOW *footer, int state) {
    wattron(footer, COLOR_PAIR(10));
    switch (state) {
    case 1:
        mvwprintw(footer, 0, 0, "Cuvant invalid.");
        break;
    case 2:
        mvwprintw(footer, 0, 0, "Alegere invalida.");
        break;
    }
    wattroff(footer, COLOR_PAIR(10));
    wrefresh(footer);
}

int footer_playagain(WINDOW *footer) {
    int result;
    do {
        wattron(footer, COLOR_PAIR(1));
        mvwprintw(footer, 0, 0, "Doriti sa jucati iar? (1/0)");
        wattroff(footer, COLOR_PAIR(1));
        result = wgetch(footer);
        if (result == '1') {
            return 1;
        } else if (result == '0') {
            return 0;
        } else {
            werase(footer);
            footer_error(footer, 2);
            napms(1000);                // afisam pentru 1 secunda eroarea
            // to do opreste userul din a scrie prea multe caractere cat timp doarme procesul
        }
    } while (result != '0' && result != '1');
}

void init_mainwindow() {

}


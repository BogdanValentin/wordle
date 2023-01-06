#include "../headers/draw.h"

void init_curses() {
    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);          // default text color
    init_pair(2, COLOR_BLACK, COLOR_GREEN);         // litera pe poz corecta
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);        // litera pe poz gresita
    init_pair(4, COLOR_CYAN, COLOR_BLACK);          // litera incorecta
    init_pair(10, COLOR_RED, COLOR_BLACK);          // cuvant invalid
    init_pair(11, COLOR_GREEN, COLOR_BLACK);        // game win
    curs_set(0);
}

void uninit_curses() {
    endwin();
}

/*  creaza o fereastra de inaltime HEADER_HEIGHT in partea de sus a
    terminalului in care se afla titlul WORDLE scris ca ASCII ART */
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

void init_rules(WINDOW *mainwindow) {
    wattron(mainwindow, COLOR_PAIR(1));
    mvwprintw(mainwindow,  3, 16, "Legenda:");
    mvwprintw(mainwindow,  4, 16, "- verde: litera exista");
    mvwprintw(mainwindow,  5, 16, "si e pe poz. buna");
    mvwprintw(mainwindow,  6, 16, "- galben: litera exista");
    mvwprintw(mainwindow,  7, 16, "dar nu e pe poz. buna");
    mvwprintw(mainwindow,  8, 16, "- galben: litera nu");
    mvwprintw(mainwindow,  9, 16, "exista in cuvant");
    mvwprintw(mainwindow, 11, 16, "- apasati ':' pentru");
    mvwprintw(mainwindow, 12, 16, " a iesi sau restart");
    wattroff(mainwindow, COLOR_PAIR(1));
}

/*  afiseaza in fereastra 'footer' unul dintre cele 5 mesaje.
    parametrul state trebuie sa fie intre 0 si 4 in functie de mesajul dorit */
void print_footer(WINDOW *footer, int state) {
    wattron(footer, COLOR_PAIR(10));
    switch (state) {
    case 0:
        mvwprintw(footer, 0, 0, "Caracter invalid.");
        break;
    case 1:
        mvwprintw(footer, 0, 0, "Cuvantul trebuie sa aiba 5 litere.");
        break;
    case 2:
        mvwprintw(footer, 0, 0, "Alegere invalida.");
        break;
    case 3:
        wattron(footer, COLOR_PAIR(1));
        mvwprintw(footer, 0, 0, "Doriti sa jucati iar? (1/0)");
        wattroff(footer, COLOR_PAIR(1));
        break;
    case 4:
        mvwprintw(footer, 0, 0, "Iesire / Joc nou (1/2)");
        break;
    }
    wattroff(footer, COLOR_PAIR(10));
    wrefresh(footer);
}

/*  afiseaza in fereastra 'footer' o intrebare legata de continuarea
    jocului. Aceasta citeste inputul userului si se ocupa de eventualele
    inputuri gresite. In final returneaza 1 daca se doreste continuarea
    jocului sau 0 in caz contrar */
int footer_playagain(WINDOW *footer) {
    int result;
    do {
        print_footer(footer, 3);
        result = wgetch(footer);
        if (result == '1') {
            return 1;
        } else if (result == '0') {
            return 0;
        } else {
            werase(footer);
            print_footer(footer, 2);
            napms(1000);
        }
    } while (result != '0' && result != '1');
}

/*  genereaza un numar aleatoriu 'random_int' intre 0 si WORDCOUNT
    si returneaza prin parametrul 'word' cuvantul de pe linia 'random_int'*/
void pick_word(char *word) {
    srand(time(0));
    int random_int = rand() % WORDCOUNT;
    FILE *file = fopen("../cuvinte.txt", "rt");
    fseek(file, random_int * 7, 0);
    fscanf(file, "%s", word);
    fclose(file);
}

/*  afiseaza placeholderele literelor('_') pe cele 6 linii */
void draw_lines(WINDOW *mainwindow) {
    wattron(mainwindow, COLOR_PAIR(1));
    for(int j = 0; j < 6; j++) {
        wmove(mainwindow, 4 + j, 3);
        for(int i = 0; i < 5; i++) {
            wprintw(mainwindow, "_");
            if(i != 4) {
                wprintw(mainwindow, " ");
            }
        }
    }
    wattroff(mainwindow, COLOR_PAIR(1));
}

/*  afiseaza literele pe */
int guess_word(WINDOW *mainwindow, WINDOW *footer, char *word, int line) {
    char written_word[5];
    int buffer, count = 0;
    wattron(mainwindow, COLOR_PAIR(1));
    while(count < 6) {
        buffer = wgetch(mainwindow);
        // 127 e backspace si 10 e enter
        // vezi de ce nu merg \b \n sau KEY_etc
        while(buffer != 127 && buffer != 10 && buffer != 58 &&
        (buffer < 97 || buffer > 122)) {
            print_footer(footer, 0);
            napms(1000);
            werase(footer);
            wrefresh(footer);
            buffer = wgetch(mainwindow);
        }
        if(buffer == 10) {
            if(count != 5) {
                print_footer(footer, 1);
                napms(1000);
                werase(footer);
                wrefresh(footer);
            } else if(count == 5) {
                count++;
            }
        } else if(buffer == 58) {
            print_footer(footer, 4);
            
            buffer = wgetch(mainwindow);
            if(buffer == '1') {
                uninit_curses();
                exit(0);
            } else if(buffer == '2') {
                werase(footer);
                wrefresh(footer);
                return -1;
            }
        } else if(buffer == 127) {
            if(count > 0) {
                count--;
                mvwprintw(mainwindow, 3 + line, 3 + 2 * count, "_");
            }
        } else {
            if(count < 5) {
                written_word[count] = buffer;
                mvwprintw(mainwindow, 3 + line, 3 + 2 * count, "%c", buffer - 32);
                count++;
            }
        }
    }
    wattroff(mainwindow, COLOR_PAIR(1));
    for(int i = 0; i < 5; i++) {
        if(word[i] == written_word[i]) {
            wattron(mainwindow, COLOR_PAIR(2));
        } else if (strchr(word, written_word[i]) != NULL) {
            wattron(mainwindow, COLOR_PAIR(3));
        } else {
            wattron(mainwindow, COLOR_PAIR(4));
        }
        mvwprintw(mainwindow, 3 + line, 3 + 2 * i, "%c", written_word[i] - 32);
        wattroff(mainwindow, COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4));
    }
    if(strcmp(word, written_word) == 0) {
        return 1;
    } else {
        return 0;
    }
}

void init_mainwindow(WINDOW *footer, char *word) {
    WINDOW *mainwindow = newwin(MAINWINDOW_HEIGHT, TERMINAL_WIDTH, HEADER_HEIGHT, 0);
    init_rules(mainwindow);
    draw_lines(mainwindow);
    for(int line = 1; line <= 6; line++) {
        int result = guess_word(mainwindow, footer, word, line);
        if(result == 1) {
            wattron(mainwindow, COLOR_PAIR(11));
            mvwprintw(mainwindow, SUBTITLE, 3, "Felicitari! Ai ghicit! :D");
            wattroff(mainwindow, COLOR_PAIR(11));
            break;
        } else if(result == 0 && line == 6) {
            wattron(mainwindow, COLOR_PAIR(10));
            mvwprintw(mainwindow, SUBTITLE, 3, "Ai pierdut! Cuvantul era %s", word);
            wattroff(mainwindow, COLOR_PAIR(10));
        } else if(result == -1) {
            draw_lines(mainwindow);
            line = 0;
        }
    }
    wrefresh(mainwindow);
}


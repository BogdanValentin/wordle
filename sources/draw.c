#include "../headers/draw.h"

void init_curses() {
    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);          // default text color
    init_pair(2, COLOR_BLACK, COLOR_GREEN);         // litera pe poz corecta
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);         // litera pe poz gresita
    init_pair(4, COLOR_CYAN, COLOR_BLACK);         // litera incorecta
    init_pair(10, COLOR_RED, COLOR_BLACK);          // cuvant invalid
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

/*  functia afiseaza in fereastra 'footer' unul dintre cele 4 mesaje.
    parametrul state trebuie sa fie 0, 1, 2 sau 3 in functie de mesajul dorit */
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
    }
    wattroff(footer, COLOR_PAIR(10));
    wrefresh(footer);
}

/*  functia afiseaza in fereastra 'footer' o intrebare legata de continuarea
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

/*  functia genereaza un numar aleatoriu 'random_int' intre 0 si WORDCOUNT
    si returneaza prin parametrul 'word' cuvantul de pe linia 'random_int'*/
void pick_word(char *word) {
    srand(time(0));
    int random_int = rand() % WORDCOUNT;
    FILE *file = fopen("../cuvinte.txt", "rt");
    fseek(file, random_int * 7, 0);
    fscanf(file, "%s", word);
    fclose(file);
}

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

void guess_word(WINDOW *mainwindow, WINDOW *footer, char *word) {
    char written_word[5];
    int buffer;
    int count = 0;
    while(count < 6) {
        buffer = wgetch(mainwindow);
        // 127 e backspace si 10 e enter
        // vezi de ce nu merg \b \n sau KEY_etc
        while(buffer != 127 && buffer != 10 && 
              (buffer < 65 || buffer > 90) && (buffer < 97 || buffer > 122)) {
            print_footer(footer, 0);
            wprintw(footer, "%c %d", '\n', buffer);
            wrefresh(footer);
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
                // to do
            } else if(count == 5) {
                count++;
            }
        } else if(buffer == 127) {
            if(count > 0) {
                count--;
                mvwprintw(mainwindow, 4, 3 + 2 * count, "_");
            }
        } else {
            if(count < 5) {
                written_word[count] = buffer;
                mvwprintw(mainwindow, 4, 3 + 2 * count, "%c", buffer);
                count++;
            }
        }
    }
    for(int i = 0; i < 5; i++) {
        if(word[i] == written_word[i]) {
            wattron(mainwindow, COLOR_PAIR(2));
            mvwprintw(mainwindow, 4, 3 + 2 * i, "%c", written_word[i]);
            wattroff(mainwindow, COLOR_PAIR(2));
        } else if (strchr(word, written_word[i]) != NULL) {
            wattron(mainwindow, COLOR_PAIR(3));
            mvwprintw(mainwindow, 4, 3 + 2 * i, "%c", written_word[i]);
            wattroff(mainwindow, COLOR_PAIR(3));
        } else {
            wattron(mainwindow, COLOR_PAIR(4));
            mvwprintw(mainwindow, 4, 3 + 2 * i, "%c", written_word[i]);
            wattroff(mainwindow, COLOR_PAIR(4));
        }
    }
    // test
    mvwprintw(mainwindow, 2, 3, "%s", word);
    
}

void init_mainwindow(WINDOW *footer, char *word) {
    WINDOW *mainwindow = newwin(MAINWINDOW_HEIGHT, TERMINAL_WIDTH, HEADER_HEIGHT, 0);
    draw_lines(mainwindow);
    guess_word(mainwindow, footer, word);



    wrefresh(mainwindow);
}


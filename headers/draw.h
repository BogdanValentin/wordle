#pragma once

#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TERMINAL_HEIGHT 20
#define TERMINAL_WIDTH  40

#define HEADER_HEIGHT 4
#define FOOTER_HEIGHT 1
#define MAINWINDOW_HEIGHT 15

#define SUBTITLE 1

#define WORDCOUNT 50

void init_curses();

void uninit_curses();

void init_header();

void init_rules(WINDOW *mainwindow);

void print_footer(WINDOW *footer, int state);

int footer_playagain(WINDOW *footer);

void pick_word(char *word);

void draw_lines(WINDOW *mainwindow);

int guess_word(WINDOW *mainwindow, WINDOW *footer, char *word, int line);

void init_mainwindow(WINDOW *footer, char *word);
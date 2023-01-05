#pragma once

#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#define TERMINAL_HEIGHT 20
#define TERMINAL_WIDTH  40

#define HEADER_HEIGHT 4
#define FOOTER_HEIGHT 1
#define MAINWINDOW_HEIGHT 15

#define WORDCOUNT 27

void init_curses();

void uninit_curses();

void init_header();

void footer_error(WINDOW *footer, int state);

int footer_playagain(WINDOW *footer);

void pick_word(char *word);

void init_mainwindow();
#pragma once

#include <ncurses.h>

#define TERMINAL_HEIGHT 20
#define TERMINAL_WIDTH  40

#define HEADER_HEIGHT 4
#define FOOTER_HEIGHT 1

void init_curses();

void uninit_curses();

void init_header();

void footer_error(WINDOW *footer, int state);

int footer_playagain(WINDOW *footer);

void init_mainwindow();
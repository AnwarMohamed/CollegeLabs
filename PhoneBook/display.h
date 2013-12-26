/*
 *
 *  Copyright (C) 2013  Anwar Mohamed <anwarelmakrahy[at]gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to Anwar Mohamed
 *  anwarelmakrahy[at]gmail.com
 *
 */

#include <curses.h>
#include <menu.h>
#include "misc.h"

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

void
show_end_screen();

void
show_modify_dialog(PHONEBOOK_ENTRY ** entries, int * entries_size, int index, char* tmp);

void
show_add_dialog(PHONEBOOK_ENTRY ** entries, int * entries_size, char* path);

void
show_load_dialog(PHONEBOOK_ENTRY * entries, int * entries_size, char* path);

void
update_records_menu(MENU* menu, PHONEBOOK_ENTRY** entries, int entries_size);

void
show_search_dialog(PHONEBOOK_ENTRY *** filtered, int * filtered_size, PHONEBOOK_ENTRY* entries, int entries_size);

void
update_record(WINDOW *my_menu_win, int index, PHONEBOOK_ENTRY** entries, int size);

void
print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

void
show_splash_screen();

void
init_curses();

void
set_up_menu(WINDOW* menu_window, MENU* menu);

void
publish_main_window(WINDOW* my_menu_win, int entries_size);

void
update_total_entries(int t);

void
cleanup_items_menu(MENU* menu);

int
show_delete_dialog(WINDOW* menu_window, int index, PHONEBOOK_ENTRY** entries, int* entries_size);


void
publish_contact_details(WINDOW* my_menu_win);

void
show_menu_window(WINDOW* my_menu_win);


#endif // DISPLAY_H_INCLUDED

#include <curses.h>
#include <menu.h>
#include "misc.h"

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

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
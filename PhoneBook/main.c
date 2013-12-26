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
#include <string.h>
#include <stdio.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#include "logos.h"
#include "files.h"
#include "display.h"
#include "records.h"
#include "misc.h"

#define DEFAULT_PHONEBOOK_PATH "phonebook_test"


void refresh_window(WINDOW* my_menu_win, MENU* my_menu, PHONEBOOK_ENTRY** filtered, int filtered_size, int entries_size)
{
    publish_main_window(my_menu_win, entries_size);
    publish_contact_details(my_menu_win);
    set_up_menu(my_menu_win, my_menu);

    update_record(my_menu_win, 0, filtered, filtered_size);
    wrefresh(my_menu_win);
    refresh();
}

int main()
{
    char tmp[100];
    char path[100];
    strcpy(path, DEFAULT_PHONEBOOK_PATH);

    MENU *my_menu;
    WINDOW *my_menu_win;

    int i, c, entries_size = 0, filtered_size = 0;
    PHONEBOOK_ENTRY
    *entries = (PHONEBOOK_ENTRY*)malloc(entries_size * sizeof(PHONEBOOK_ENTRY)),
    **filtered = (PHONEBOOK_ENTRY**)malloc(filtered_size * sizeof(PHONEBOOK_ENTRY*));

    strings_table_size = 0;
    strings_table = (char**)malloc(0);

    load_record_file(path, &entries_size, &entries);
    sort_records(entries, entries_size);
    filtered_size = entries_size;
    filtered = (PHONEBOOK_ENTRY**)realloc(filtered, filtered_size * sizeof(PHONEBOOK_ENTRY*));

    for (i=0; i<entries_size; i++)
        filtered[i] = &entries[i];

    init_curses();

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_RED);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    show_splash_screen();

    my_menu = new_menu(NULL);
    update_records_menu(my_menu, filtered, filtered_size);

    my_menu_win = newwin(17, 78, 2, 1);
    publish_main_window(my_menu_win, filtered_size);
    publish_contact_details(my_menu_win);

    set_up_menu(my_menu_win, my_menu);
    post_menu(my_menu);
    update_record(my_menu_win, 0, filtered, filtered_size);
    wrefresh(my_menu_win);
    refresh();

    while((c = getch()) != KEY_F(8))
    {
        switch(c)
        {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        case KEY_NPAGE:
            menu_driver(my_menu, REQ_SCR_DPAGE);
            break;
        case KEY_PPAGE:
            menu_driver(my_menu, REQ_SCR_UPAGE);
            break;

        case KEY_F(1):
            show_add_dialog(&entries, &entries_size, tmp);
            sort_records(entries, entries_size);

            filtered = (PHONEBOOK_ENTRY**)realloc(filtered, entries_size * sizeof(PHONEBOOK_ENTRY*));
            filtered_size = entries_size;

            for (i=0; i<entries_size; i++)
                filtered[i] = &entries[i];

            unpost_menu(my_menu);
            update_records_menu(my_menu, filtered, filtered_size);
            wrefresh(my_menu_win);
            post_menu(my_menu);
            wrefresh(my_menu_win);

            refresh_window(my_menu_win, my_menu, filtered, filtered_size, entries_size);
            break;

        case KEY_F(2):
            if (entries_size > 0)
            {
                show_modify_dialog(&entries, &entries_size, (*filtered[item_index(current_item(my_menu))]).index, tmp);
                sort_records(entries, entries_size);

                filtered = (PHONEBOOK_ENTRY**)realloc(filtered, entries_size * sizeof(PHONEBOOK_ENTRY*));
                filtered_size = entries_size;

                for (i=0; i<entries_size; i++)
                filtered[i] = &entries[i];

                unpost_menu(my_menu);
                update_records_menu(my_menu, filtered, filtered_size);
                wrefresh(my_menu_win);
                post_menu(my_menu);
                wrefresh(my_menu_win);
            }

            refresh_window(my_menu_win, my_menu, filtered, filtered_size, entries_size);
            break;

        case KEY_F(3):
            if (filtered_size > 0 && entries_size > 0 &&
                show_delete_dialog(my_menu_win, (*filtered[item_index(current_item(my_menu))]).index, &entries, &entries_size))
            {
                if (entries_size > 0) sort_records(entries, entries_size);
                filtered = (PHONEBOOK_ENTRY**)realloc(filtered, entries_size * sizeof(PHONEBOOK_ENTRY*));
                filtered_size = entries_size;

                for (i=0; i<entries_size; i++)
                    filtered[i] = &entries[i];

                unpost_menu(my_menu);
                update_records_menu(my_menu, filtered, filtered_size);
                wrefresh(my_menu_win);
                post_menu(my_menu);
                wrefresh(my_menu_win);
            }

            refresh_window(my_menu_win, my_menu, filtered, filtered_size, entries_size);
            break;

        case KEY_F(4):
            if (entries_size > 0)
            {
                show_search_dialog(&filtered, &filtered_size, entries, entries_size);
                unpost_menu(my_menu);
                update_records_menu(my_menu, filtered, filtered_size);
                wrefresh(my_menu_win);
                post_menu(my_menu);
                wrefresh(my_menu_win);
                refresh_window(my_menu_win, my_menu, filtered, filtered_size, entries_size);
            }
            break;

        case KEY_F(5):
            if (strlen(file_name) > 0)
                save_records(file_name, entries, entries_size);
            break;

        case KEY_F(6):
            show_load_dialog(entries, &entries_size, path);
            load_record_file(path, &entries_size, &entries);
            sort_records(entries, entries_size);

            filtered = (PHONEBOOK_ENTRY**)realloc(filtered, entries_size * sizeof(PHONEBOOK_ENTRY*));
            filtered_size = entries_size;

            for (i=0; i<entries_size; i++)
                filtered[i] = &entries[i];

            unpost_menu(my_menu);
            update_records_menu(my_menu, filtered, filtered_size);
            wrefresh(my_menu_win);
            post_menu(my_menu);
            wrefresh(my_menu_win);

            refresh_window(my_menu_win, my_menu, filtered, filtered_size, entries_size);
            break;
        case KEY_F(7):
            if (entries_size > 0)
            {
                filtered = (PHONEBOOK_ENTRY**)realloc(filtered, entries_size * sizeof(PHONEBOOK_ENTRY*));
                filtered_size = entries_size;

                for (i=0; i<entries_size; i++)
                    filtered[i] = &entries[i];

                unpost_menu(my_menu);
                update_records_menu(my_menu, filtered, filtered_size);
                wrefresh(my_menu_win);
                post_menu(my_menu);
                wrefresh(my_menu_win);
            }

            break;
        }

        if (filtered_size > 0)
            update_record(my_menu_win, item_index(current_item(my_menu)), filtered, filtered_size);
        wrefresh(my_menu_win);
    }

    unpost_menu(my_menu);
    free_menu(my_menu);
    cleanup_items_menu(my_menu);
    show_end_screen();
    endwin();

    //unload_record_file();
    return 0;
}

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


#include "display.h"
#include <curses.h>
#include <menu.h>
#include <form.h>
#include "records.h"
#include "misc.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define DEL_WIDTH 20
#define DEL_HEIGHT 7
#define SEARCH_WIDTH 50
#define SEARCH_HEIGHT 7
#define ADD_WIDTH 60
#define ADD_HEIGHT 15

void
show_add_error(WINDOW* win, char* err)
{
    mvwprintw(win, 13, 2, "ERROR: %s", err);
}

void
show_modify_dialog(PHONEBOOK_ENTRY ** entries, int * entries_size, int index, char* tmp)
{
    WINDOW *add_window;
    int offsetx, offsety, c, i;

    offsetx = (COLS - ADD_WIDTH) / 2;
    offsety = (LINES - ADD_HEIGHT) / 2 - 2;

    add_window = newwin(ADD_HEIGHT,
                        ADD_WIDTH,
                        offsety,
                        offsetx);

    keypad(add_window, TRUE);
    box(add_window, 0 , 0);

    int rows, cols;
    FIELD *field[6];
    FORM  *add_form;

    field[0] = new_field(1, 25, 2, 0, 0, 100);
    field[1] = new_field(1, 25, 2, 28, 0, 100);
    field[2] = new_field(1, 25, 5, 0, 0, 100);
    field[3] = new_field(1, 25, 5, 28, 0, 100);
    field[4] = new_field(1, 25, 8, 0, 0, 100);
    field[5] = NULL;

    for (i=0; i<5; i++) {
        set_field_back(field[i], A_UNDERLINE);
        field_opts_off(field[i], O_AUTOSKIP);
    }

    add_form = new_form(field);
    scale_form(add_form, &rows, &cols);
    set_form_sub(add_form, derwin(add_window, rows, cols, 2, 2));
    post_form(add_form);

    ITEM **my_items = (ITEM **)calloc(3, sizeof(ITEM *));
    my_items[0] = new_item("  MODIFY ", "");
    my_items[1] = new_item(" CANCEL ", "");
    my_items[2] = (ITEM *)NULL;

    MENU *my_menu = new_menu((ITEM **)my_items);
    set_menu_win(my_menu, add_window);
    set_menu_sub(my_menu, derwin(add_window, 0, 0,13, 39));
    set_menu_format(my_menu, 1, 2);
    set_menu_mark(my_menu, "");

    attron(COLOR_PAIR(1));
    mvprintw(offsety + 1, offsetx + 1, "\t\t\tModify a Contact\t\t     ");
    attroff(COLOR_PAIR(1));

    mvwprintw(add_window, 3, 2, "First Name:");
    mvwprintw(add_window, 3, 30, "Last Name:");
    mvwprintw(add_window, 6, 2, "Phone Number:");
    mvwprintw(add_window, 6, 30, "City:");
    mvwprintw(add_window, 9, 2, "Address:");

    set_field_buffer(field[0], 0, (*entries)[index].fname);
    set_field_buffer(field[1], 0, (*entries)[index].lname);
    set_field_buffer(field[2], 0, (*entries)[index].phone);
    set_field_buffer(field[3], 0, (*entries)[index].city);
    set_field_buffer(field[4], 0, (*entries)[index].address);

    post_menu(my_menu);
    wrefresh(add_window);
    refresh();

    int is_shown = 1, tlen;
    while(is_shown && (c = wgetch(add_window)))
    {
        switch(c)
        {
        case 9:
            if (!item_index(current_item(my_menu)))
                menu_driver(my_menu, REQ_RIGHT_ITEM);
            else
                menu_driver(my_menu, REQ_LEFT_ITEM);
            break;
        case KEY_DOWN:
            form_driver(add_form, REQ_NEXT_FIELD);
            form_driver(add_form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(add_form, REQ_PREV_FIELD);
            form_driver(add_form, REQ_END_LINE);
            break;
        case '\n':
            if (!item_index(current_item(my_menu)))
            {
                form_driver(add_form, REQ_VALIDATION);

                //firstname
                snprintf(tmp, 99, "%s", trim(remove_comma(field_buffer(field[0], 0))));
                f_capital(tmp);
                if ((tlen = strlen(tmp)) == 0) {
                    show_add_error(add_window, "Enter Firstname\t");
                    break;
                }
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, tmp, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[index].fname = tmp_string;

                //lastname
                snprintf(tmp, 99, "%s", trim(remove_comma(field_buffer(field[1], 0))));
                f_capital(tmp);
                if ((tlen = strlen(tmp)) == 0) {
                    show_add_error(add_window, "Enter Lastname\t");
                    break;
                }
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, tmp, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[index].lname = tmp_string;

                //phonenumber
                snprintf(tmp, 99, "%s", trim(remove_comma(field_buffer(field[2], 0))));
                if ((tlen = strlen(tmp)) > 0 && !is_numeric(tmp)) {
                    show_add_error(add_window, "Enter a valid Phone Number\t");
                    break;
                }
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, tmp, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[index].phone = tmp_string;

                //city
                snprintf(tmp, 99, "%s", trim(remove_comma(field_buffer(field[3], 0))));
                tlen = strlen(tmp);
                f_capital(tmp);
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, tmp, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[index].city = tmp_string;

                //address
                snprintf(tmp, 99, "%s", trim(remove_comma(field_buffer(field[4], 0))));
                tlen = strlen(tmp);
                f_capital(tmp);
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, tmp, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[index].address = tmp_string;

                is_shown = 0;
            }
            else
                is_shown = 0;

            break;
        case KEY_DC:
            form_driver(add_form, REQ_DEL_CHAR);
            break;
        case KEY_BACKSPACE:
            form_driver(add_form, REQ_DEL_PREV);
            break;
        case KEY_LEFT:
            form_driver(add_form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(add_form, REQ_NEXT_CHAR);
            break;
        default:
            form_driver(add_form, c);
            break;
        }
        curs_set(1);
        wrefresh(add_window);
    }

    unpost_menu(my_menu);
    delwin(add_window);
    refresh();

    free_item(my_items[0]);
    free_item(my_items[1]);
    free_menu(my_menu);
    curs_set(0);
}

void
show_add_dialog(PHONEBOOK_ENTRY ** entries, int * entries_size, char* path)
{
    WINDOW *add_window;
    int offsetx, offsety, c, i;

    offsetx = (COLS - ADD_WIDTH) / 2;
    offsety = (LINES - ADD_HEIGHT) / 2 - 2;

    add_window = newwin(ADD_HEIGHT,
                        ADD_WIDTH,
                        offsety,
                        offsetx);

    keypad(add_window, TRUE);
    box(add_window, 0 , 0);

    int rows, cols;
    FIELD *field[6];
    FORM  *add_form;

    field[0] = new_field(1, 25, 2, 0, 0, 100);
    field[1] = new_field(1, 25, 2, 28, 0, 100);
    field[2] = new_field(1, 25, 5, 0, 0, 100);
    field[3] = new_field(1, 25, 5, 28, 0, 100);
    field[4] = new_field(1, 25, 8, 0, 0, 100);
    field[5] = NULL;

    for (i=0; i<5; i++) {
        set_field_back(field[i], A_UNDERLINE);
        field_opts_off(field[i], O_AUTOSKIP);
    }

    add_form = new_form(field);
    scale_form(add_form, &rows, &cols);
    set_form_sub(add_form, derwin(add_window, rows, cols, 2, 2));
    post_form(add_form);

    ITEM **my_items = (ITEM **)calloc(3, sizeof(ITEM *));
    my_items[0] = new_item("  ADD ", "");
    my_items[1] = new_item(" CANCEL ", "");
    my_items[2] = (ITEM *)NULL;

    MENU *my_menu = new_menu((ITEM **)my_items);
    set_menu_win(my_menu, add_window);
    set_menu_sub(my_menu, derwin(add_window, 0, 0,13, 41));
    set_menu_format(my_menu, 1, 2);
    set_menu_mark(my_menu, "");

    attron(COLOR_PAIR(1));
    mvprintw(offsety + 1, offsetx + 1, "\t\t\t   Add a Contact\t\t     ");
    attroff(COLOR_PAIR(1));

    mvwprintw(add_window, 3, 2, "First Name:");
    mvwprintw(add_window, 3, 30, "Last Name:");
    mvwprintw(add_window, 6, 2, "Phone Number:");
    mvwprintw(add_window, 6, 30, "City:");
    mvwprintw(add_window, 9, 2, "Address:");

    post_menu(my_menu);
    wrefresh(add_window);
    refresh();

    (*entries) = (PHONEBOOK_ENTRY*)realloc((*entries),((*entries_size) + 1)* sizeof(PHONEBOOK_ENTRY));

    int is_shown = 1, tlen, d=0;
    while(is_shown && (c = wgetch(add_window)))
    {
        switch(c)
        {
        case 9:
            if (!item_index(current_item(my_menu)))
                menu_driver(my_menu, REQ_RIGHT_ITEM);
            else
                menu_driver(my_menu, REQ_LEFT_ITEM);
            break;
        case KEY_DOWN:
            form_driver(add_form, REQ_NEXT_FIELD);
            form_driver(add_form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(add_form, REQ_PREV_FIELD);
            form_driver(add_form, REQ_END_LINE);
            break;
        case '\n':
            if (!item_index(current_item(my_menu)))
            {
                form_driver(add_form, REQ_VALIDATION);

                //firstname
                snprintf(path, 99, "%s", trim(remove_comma(field_buffer(field[0], 0))));
                if ((tlen = strlen(path)) == 0) {
                    show_add_error(add_window, "Enter Firstname\t");
                    break;
                }
                f_capital(path);
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, path, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[*entries_size].fname = tmp_string;

                //lastname
                snprintf(path, 99, "%s", trim(remove_comma(field_buffer(field[1], 0))));
                f_capital(path);
                if ((tlen = strlen(path)) == 0) {
                    show_add_error(add_window, "Enter Lastname\t");
                    break;
                }
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, path, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[*entries_size].lname = tmp_string;

                //phonenumber
                snprintf(path, 99, "%s", trim(remove_comma(field_buffer(field[2], 0))));
                if ((tlen = strlen(path)) > 0 && !is_numeric(path)) {
                    show_add_error(add_window, "Enter a valid Phone Number\t");
                    break;
                }
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, path, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[*entries_size].phone = tmp_string;

                //city
                snprintf(path, 99, "%s", trim(remove_comma(field_buffer(field[3], 0))));
                tlen = strlen(path);
                f_capital(path);
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, path, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[*entries_size].city = tmp_string;

                //address
                snprintf(path, 99, "%s", trim(remove_comma(field_buffer(field[4], 0))));
                tlen = strlen(path);
                f_capital(path);
                tmp_string = (char*)malloc((tlen + 1) * sizeof(char));
                memcpy(tmp_string, path, (tlen+ 1) * sizeof(char));
                strings_table = (char**)realloc(strings_table, ++strings_table_size * sizeof(char*));
                strings_table[strings_table_size - 1] = tmp_string;
                (*entries)[*entries_size].address = tmp_string;
                (*entries)[*entries_size].index = *entries_size;
                ++(*entries_size);
                is_shown = 0;
            }
            else
                is_shown = 0;

            break;
        case KEY_DC:
            form_driver(add_form, REQ_DEL_CHAR);
            break;
        case KEY_BACKSPACE:
            form_driver(add_form, REQ_DEL_PREV);
            break;
        case KEY_LEFT:
            form_driver(add_form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(add_form, REQ_NEXT_CHAR);
            break;
        default:
            form_driver(add_form, c);
            break;
        }
        curs_set(1);
        wrefresh(add_window);
    }

    unpost_menu(my_menu);
    delwin(add_window);
    refresh();

    free_item(my_items[0]);
    free_item(my_items[1]);
    free_menu(my_menu);
    curs_set(0);
}

void
show_load_dialog(PHONEBOOK_ENTRY * entries, int * entries_size, char* path)
{
    WINDOW *load_window;
    int offsetx, offsety, c;

    offsetx = (COLS - SEARCH_WIDTH) / 2;
    offsety = (LINES - SEARCH_HEIGHT) / 2;

    load_window = newwin(SEARCH_HEIGHT,
                         SEARCH_WIDTH,
                         offsety,
                         offsetx);

    keypad(load_window, TRUE);

    box(load_window, 0 , 0);

    int rows, cols;
    FIELD *field[2];
    FORM  *load_form;

    field[0] = new_field(1, 46, 1, 0, 0, 0);
    field[1] = NULL;

    set_field_back(field[0], A_UNDERLINE);
    field_opts_off(field[0], O_AUTOSKIP);
    load_form = new_form(field);
    scale_form(load_form, &rows, &cols);
    set_form_sub(load_form, derwin(load_window, rows, cols, 2, 2));
    post_form(load_form);

    ITEM **my_items = (ITEM **)calloc(3, sizeof(ITEM *));
    my_items[0] = new_item(" LOAD ", "");
    my_items[1] = new_item(" CANCEL", "");
    my_items[2] = (ITEM *)NULL;

    MENU *my_menu = new_menu((ITEM **)my_items);
    set_menu_win(my_menu, load_window);
    set_menu_sub(my_menu, derwin(load_window, 0, 0,5, 17));
    set_menu_format(my_menu, 1, 2);
    set_menu_mark(my_menu, "");

    attron(COLOR_PAIR(1));
    mvprintw(offsety + 1, offsetx + 1, "\t\tLoad from File\t\t\t");
    attroff(COLOR_PAIR(1));

    post_menu(my_menu);
    wrefresh(load_window);
    refresh();

    int is_shown = 1;
    while(is_shown && (c = wgetch(load_window)) != 27)
    {
        switch(c)
        {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_RIGHT_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_LEFT_ITEM);
            break;
        case '\n':
            is_shown = 0;
            if (!item_index(current_item(my_menu)))
            {
                form_driver(load_form, REQ_VALIDATION);
                snprintf(path, 99, "%s", trim(field_buffer(field[0], 0)));
            }
            break;
        case KEY_DC:
            form_driver(load_form, REQ_DEL_CHAR);
            break;
        case KEY_BACKSPACE:
            form_driver(load_form, REQ_DEL_PREV);
            break;
        case KEY_LEFT:
            form_driver(load_form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(load_form, REQ_NEXT_CHAR);
            break;
        default:
            curs_set(1);
            form_driver(load_form, c);
            break;
        }

        wrefresh(load_window);
    }

    unpost_menu(my_menu);
    delwin(load_window);
    refresh();

    free_item(my_items[0]);
    free_item(my_items[1]);
    free_menu(my_menu);
    curs_set(0);
}

void
show_search_dialog(PHONEBOOK_ENTRY *** filtered, int * filtered_size, PHONEBOOK_ENTRY* entries, int entries_size)
{
    WINDOW *search_window;
    int offsetx, offsety, c;

    offsetx = (COLS - SEARCH_WIDTH) / 2;
    offsety = (LINES - SEARCH_HEIGHT) / 2;

    search_window = newwin(SEARCH_HEIGHT,
                           SEARCH_WIDTH,
                           offsety,
                           offsetx);

    keypad(search_window, TRUE);

    box(search_window, 0 , 0);

    int rows, cols;
    FIELD *field[2];
    FORM  *search_form;

    field[0] = new_field(1, 46, 1, 0, 0, 0);
    field[1] = NULL;

    set_field_back(field[0], A_UNDERLINE);
    field_opts_off(field[0], O_AUTOSKIP);
    search_form = new_form(field);
    scale_form(search_form, &rows, &cols);
    set_form_sub(search_form, derwin(search_window, rows, cols, 2, 2));
    post_form(search_form);

    ITEM **my_items = (ITEM **)calloc(3, sizeof(ITEM *));
    my_items[0] = new_item(" SEARCH ", "");
    my_items[1] = new_item(" CANCEL", "");
    my_items[2] = (ITEM *)NULL;

    MENU *my_menu = new_menu((ITEM **)my_items);
    set_menu_win(my_menu, search_window);
    set_menu_sub(my_menu, derwin(search_window, 0, 0,5, 17));
    set_menu_format(my_menu, 1, 2);
    set_menu_mark(my_menu, "");

    attron(COLOR_PAIR(1));
    mvprintw(offsety + 1, offsetx + 1, "\t\tSearch for Record\t\t");
    attroff(COLOR_PAIR(1));

    post_menu(my_menu);
    wrefresh(search_window);
    refresh();

    char* input = (char*)malloc(100 * sizeof(char));

    int is_shown = 1;
    while(is_shown && (c = wgetch(search_window)) != 27)
    {
        switch(c)
        {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_RIGHT_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_LEFT_ITEM);
            break;
        case '\n':
            is_shown = 0;
            if (!item_index(current_item(my_menu)))
            {
                form_driver(search_form, REQ_VALIDATION);
                snprintf(input, 100, "%s", field_buffer(field[0], 0));
                search_records(entries, entries_size, trim(input), filtered_size, filtered);
            }
            break;
        case KEY_DC:
            form_driver(search_form, REQ_DEL_CHAR);
            break;
        case KEY_BACKSPACE:
            form_driver(search_form, REQ_DEL_PREV);
            break;
        case KEY_LEFT:
            form_driver(search_form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(search_form, REQ_NEXT_CHAR);
            break;
        default:
            curs_set(1);
            form_driver(search_form, c);
            break;
        }

        wrefresh(search_window);
    }

    free(input);
    //unpost_menu(my_menu);
    delwin(search_window);
    refresh();

    free_item(my_items[0]);
    free_item(my_items[1]);
    free_menu(my_menu);
    curs_set(0);
}

int
show_delete_dialog(WINDOW* menu_window, int index, PHONEBOOK_ENTRY** entries, int* entries_size)
{

    WINDOW *del_window;
    int offsetx, offsety, c;

    offsetx = (COLS - DEL_WIDTH) / 2;
    offsety = (LINES - DEL_HEIGHT) / 2;

    del_window = newwin(DEL_HEIGHT,
                        DEL_WIDTH,
                        offsety,
                        offsetx);

    keypad(menu_window, FALSE);
    keypad(del_window, TRUE);

    box(del_window, 0 , 0);

    ITEM **my_items = (ITEM **)calloc(3, sizeof(ITEM *));
    my_items[0] = new_item(" YES ", "");
    my_items[1] = new_item(" NO", "");
    my_items[2] = (ITEM *)NULL;

    MENU *my_menu = new_menu((ITEM **)my_items);
    set_menu_win(my_menu, del_window);
    set_menu_sub(my_menu, derwin(del_window, 0, 0,5, 5));
    set_menu_format(my_menu, 1, 2);
    set_menu_mark(my_menu, "");

    attron(COLOR_PAIR(1));
    mvprintw(offsety + 1, offsetx + 1, "  Delete Record   ");
    attroff(COLOR_PAIR(1));

    post_menu(my_menu);
    wrefresh(del_window);
    refresh();

    int is_shown = 1, del = 0;
    while(is_shown && (c = wgetch(del_window)) != 27)
    {
        switch(c)
        {
        case KEY_RIGHT:
            menu_driver(my_menu, REQ_RIGHT_ITEM);
            break;
        case KEY_LEFT:
            menu_driver(my_menu, REQ_LEFT_ITEM);
            break;
        case '\n':
            is_shown = 0;
            if (!item_index(current_item(my_menu)))
            {
                del = 1;
                delete_record(entries, entries_size, index);
            }
            break;
        }

        wrefresh(del_window);
    }

    unpost_menu(my_menu);
    delwin(del_window);
    keypad(menu_window, TRUE);
    refresh();

    free_item(my_items[0]);
    free_item(my_items[1]);
    free_menu(my_menu);
    return del;
}

void
update_total_entries(int t)
{
    mvprintw(LINES - 3, 0, " Total Contacts: %d\t\t", t);
}

void
publish_main_window(WINDOW* my_menu_win, int entries_size)
{
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, " PhoneBook Project ~ Anwar Mohamed & Yasmine El-Habashi                         ");
    mvprintw(LINES - 1, 0, " F1:Add  F2:Modify  F3:Delete  F4:Query  F5:Save  F6:Load  F7:Refresh  F8:Quit\t");
    attroff(COLOR_PAIR(1));
    update_total_entries(entries_size);
}

void
publish_contact_details(WINDOW* my_menu_win)
{
    wattron(my_menu_win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(my_menu_win, 3, 27, "%s", "First Name:");
    mvwprintw(my_menu_win, 5, 27, "%s", "Last Name:");
    mvwprintw(my_menu_win, 7, 27, "%s", "Street Address:");
    mvwprintw(my_menu_win, 9, 27, "%s", "City:");
    mvwprintw(my_menu_win, 11, 27, "%s", "Phone Number:");
    wattroff(my_menu_win, COLOR_PAIR(2)|A_BOLD);
}

void
set_up_menu(WINDOW* my_menu_win, MENU* my_menu)
{
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 15, 24, 1, 1));
    set_menu_format(my_menu, 15, 1);
    set_menu_mark(my_menu, "");
    show_menu_window(my_menu_win);
}

void
show_menu_window(WINDOW* my_menu_win)
{
    box(my_menu_win, 0, 0);

    print_in_middle(my_menu_win, 1, 26, 52, "  Contact Details  ", COLOR_PAIR(0));

    int i;
    for (i=1; i<16; i++)
        mvwaddch(my_menu_win, i, 25, ACS_VLINE);
}

void
cleanup_items_menu(MENU* menu)
{
    int i, items_size = item_count(menu);
    ITEM **menu_items_array = menu_items(menu);

    for(i = 0; i <items_size; ++i)
        free_item(menu_items_array[i]);
}
int k=0;

void
update_records_menu(MENU* menu, PHONEBOOK_ENTRY** entries, int entries_size)
{
    int i, items_size = item_count(menu);
    ITEM **menu_items_array = menu_items(menu);
    for(i = 0; i <items_size; ++i)
        free_item(menu_items_array[i]);
    //free(menu_items_array);
    menu_items_array = (ITEM **)malloc(/*menu_items_array,*/((entries_size > 0? entries_size: 1) +1)* sizeof(ITEM *));

    if (entries_size == 0)
    {
        menu_items_array[0] = new_item("No", "Records");
        menu_items_array[1] = (ITEM *)NULL;
    }
    else
    {
        for(i = 0; i < entries_size; ++i)
            menu_items_array[i] = new_item((*entries[i]).lname, (*entries[i]).fname);

        menu_items_array[entries_size] = (ITEM *)NULL;
    }

    set_menu_items(menu, menu_items_array);
}

void
print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
    int length, x, y;
    float temp;

    if(win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if(startx != 0)
        x = startx;
    if(starty != 0)
        y = starty;
    if(width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length)/ 2;
    x = startx + (int)temp;
    wattron(win, color| A_BOLD);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color | A_BOLD);
    refresh();
}

const char logo[][80] =
{
    " _____  _                      ____              _    ",
    "|  __ \\| |                    |  _ \\            | |   ",
    "| |__) | |__   ___  _ __   ___| |_) | ___   ___ | | __",
    "|  ___/| '_ \\ / _ \\| '_ \\ / _ \\  _ < / _ \\ / _ \\| |/ /",
    "| |    | | | | (_) | | | |  __/ |_) | (_) | (_) |   < ",
    "|_|    |_| |_|\\___/|_| |_|\\___|____/ \\___/ \\___/|_|\\_\\"
};

void
show_splash_screen()
{

    int i;
    attron(COLOR_PAIR(4) | A_BOLD);
    for (i=0; i<ARRAY_SIZE(logo); i++)
        mvaddstr(8 + i, 13, logo[i]);
    attroff(COLOR_PAIR(4) | A_BOLD);
    attron(A_BOLD);
    mvaddstr(i + 9, 13, "Anwar Mohamed - 2491\t Yasmine El-Habashi - 2083");
    attroff(A_BOLD);
    refresh();
    sleep(2);
    clear();
}

void
show_end_screen()
{
    clear();
    mvaddstr(9, 13, "Thank you for using");
    int i;
    attron(COLOR_PAIR(4) | A_BOLD);
    for (i=0; i<ARRAY_SIZE(logo); i++)
        mvaddstr(10 + i, 13, logo[i]);
    attroff(COLOR_PAIR(4) | A_BOLD);
    refresh();
    sleep(2);
    clear();
}

void
init_curses()
{
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
}

void
update_record(WINDOW *my_menu_win, int index, PHONEBOOK_ENTRY** entries, int size)
{

    wattron(my_menu_win, A_BOLD);

    wmove(my_menu_win, 3, 39);
    wclrtoeol(my_menu_win);
    mvwprintw(my_menu_win, 3, 39, "%s", index > size-1 ? "\t\t\t\t" : (*entries[index]).fname != 0? (*entries[index]).fname : "[NotSet]");

    wmove(my_menu_win, 5, 38);
    wclrtoeol(my_menu_win);
    mvwprintw(my_menu_win, 5, 38, "%s", index > size-1 ? "\t\t\t\t" : (*entries[index]).lname != 0? (*entries[index]).lname : "[NotSet]");

    wmove(my_menu_win, 7, 43);
    wclrtoeol(my_menu_win);
    mvwprintw(my_menu_win, 7, 43, "%s", index > size-1 ? "\t\t\t\t" : (*entries[index]).address != 0? (*entries[index]).address : "[NotSet]");

    wmove(my_menu_win, 9, 33);
    wclrtoeol(my_menu_win);
    mvwprintw(my_menu_win, 9, 33, "%s", index > size-1 ? "\t\t\t\t" : (*entries[index]).city != 0? (*entries[index]).city : "[NotSet]");

    wmove(my_menu_win, 11, 41);
    wclrtoeol(my_menu_win);
    mvwprintw(my_menu_win, 11, 41, "%s", index > size-1 ? "\t\t\t\t" : (*entries[index]).phone != 0? (*entries[index]).phone : "[NotSet]");
    wattroff(my_menu_win, A_BOLD);

    int i;
    for (i=3; i<=11; i+=2)
        mvwaddch(my_menu_win, i, 77, ACS_VLINE);
}

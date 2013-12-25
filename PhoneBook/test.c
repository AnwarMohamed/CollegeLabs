/*
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

char *choices[] =
{
    "Choice 1",
    "Choice 2",
    "Choice 3",
    "Choice 4",
    "Choice 5",
    "Choice 6",
    "Choice 7",
    "Exit",
};

int main()
{
    ITEM **my_items;
    int c;
    MENU *my_menu;
    int n_choices, i;
    ITEM *cur_item;
    WINDOW *my_menu_win;


    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);


    n_choices = ARRAY_SIZE(choices);
    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for(i = 0; i < n_choices; ++i)
        my_items[i] = new_item(choices[i], choices[i]);
    my_items[n_choices] = (ITEM *)NULL;
    item_opts_off(my_items[3], O_SELECTABLE);
    item_opts_off(my_items[6], O_SELECTABLE);


    my_menu = new_menu((ITEM **)my_items);


    set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
    set_menu_back(my_menu, COLOR_PAIR(2));
    set_menu_grey(my_menu, COLOR_PAIR(3));

    my_menu_win = newwin(10, 40, 4, 4);
    keypad(my_menu_win, TRUE);

        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));


    attron(COLOR_PAIR(1));
    mvprintw(0, 0, " PhoneBook Project ~ Anwar Mohamed & Yasmine El-Habashi                          ");
    mvprintw(LINES - 1, 0, " <F1> Add New - <F2> Edit - <F3> Delete - <F4> Search - <F5> Load - <F6> Quit   ");
    attroff(COLOR_PAIR(1));

    post_menu(my_menu);
    wrefresh(my_menu_win);
    //refresh();

    while((c = getch()) != KEY_F(6))
    {
        switch(c)
        {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        case 10:
            move(20, 0);
            clrtoeol();
            mvprintw(20, 0, "Item selected is : %s",
                     item_name(current_item(my_menu)));
            pos_menu_cursor(my_menu);
            break;
        }
    }
    //unpost_menu(my_menu);
    for(i = 0; i < n_choices; ++i)
        free_item(my_items[i]);
    free_menu(my_menu);
    endwin();
}
*/

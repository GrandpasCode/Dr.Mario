#include "info.h"

void newstart()
{
    c[0] = 'A';                 /* Game pieces */
    c[1] = 'L';
    c[2] = 'S';

    p2 = &p1[1];                /*pointers not really necessary here
                                   for that matter neither is array */

    w = subwin(stdscr, 18, 10, 2, 32);  /*make gameboard */
    if (w == (WINDOW *) NULL)
        fatal("memory error");  /*trouble- no memory! */

    box(w, 124, 34);

    mvwprintw(w, 0, 0, "___!  !___");   /*change box a little */
    mvwaddch(w, 17, 0, 34);
    mvwaddch(w, 17, 9, 34);
}

void start()                    /*call this every new level */
{
    int i, x, y, z, ch;
    for (y = 1; y < 17; y++)    /*zap board and a[][] */
        for (x = 1; x < 9; x++)
            mvwaddch(w, y, x, BLANK), a[y][x] = 0;
    wrefresh(w);
    wstandout(w);               /*reverse all characters added till standend() */
    for (i = 0; i < v; i++)
    {
        do
        {
            z = rx(3, 0);       /*which char? */
            x = rx(8, 1);
            y = rx(14, 4);
            ch = mvwinch(w, y, x);
            if ((mvwinch(w, y + 3, x) == c[z] || mvwinch(w, y - 3, x) == c[z]) || (mvwinch(w, y, x + 3) == c[z] || mvwinch(w, y, x - 3) == c[z]))       /* can't have 4 in a row or col to start */
                ch = 0;

        }
        while (ch != BLANK);
        mvwaddch(w, y, x, c[z]);        /*place on board */
        a[y][x] = 3;            /* show bug in a[][] */
        wrefresh(w);
    }
    wstandend(w);
    nextpiece();                /*show next piece */
}

void nextpiece()
{
    static chtype na, nb;       /*remember every time */
    p1->c = c[na];              /*put old chars in play */
    p2->c = c[nb];
    na = rx(3, 0);              /* make new next piece & display it */
    nb = rx(3, 0);
    mvprintw(1, 36, "%c%c", c[na], c[nb]);
    refresh();
}

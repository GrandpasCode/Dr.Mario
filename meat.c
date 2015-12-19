#include "info.h"
#include "meat.h"

struct pos t[48];               /*terminate array - could have used
                                   linked lists, wouldn't be worth
                                   time spent in malloc each time */

void meat()                     /*checks and removes */
{
    int remove = 0, i;
    /*remove is flag that we've erased something */

    tcount = 0;                 /*haven't killed anything yet */

    remove = row(p1->y) + cols(p1->x);

    if (cp & 1)
        remove += row(p2->y);   /*we're vertical */

    if (!(cp & 1))
        remove += cols(p2->x);  /*we're sideways;horizontal if you wish */

    if (terminate())
        return;                 /*zap characters and move down pieces if possible
                                   terminate returns 0 unless v==0 */

    while (remove)
    {                           /*did something check if any fallen pieces
                                   can remove anything */

        remove = 0;             /*reset - we might have to do this more than once */

        /*check everywhere */
        for (i = 1; i < 17; i++)
            remove += row(i);
        for (i = 1; i < 9; i++)
            remove += cols(i);
        if (remove)             /*anything happen? */
            if (terminate())    /*yup. */
                return;
    }
}

int terminate()                 /*I'll be back! */
{
    struct pos *tp = t;         /*tp points to t[0] */
    unsigned int i;
    wrefresh(w);

    if (v == 0)
        return ERR;             /*we've killed all the bugs! */

    /*walk through t[48] */
    for (i = 0; tcount > i; tp++, i++)
        while (tp->y < 18)      /*till we're at the bottom */
            if (mvwinch(w, tp->y, tp->x) != BLANK)
                break;          /*nothing there */
            else
                down(tp->y++, tp->x);   /*pull down one if we can, then
                                           see if we can do it again */

    tcount = 0;                 /*no more in t[] */
    wrefresh(w);
    return 0;                   /*still got bugs left */
}

void down(int y, int x)
{
    chtype ad;
    int b;

    /*if current pos is blank and up is not & not  a bug & we're not at 
       the top,(whew!) we're ok */
    while (mvwinch(w, y, x) == BLANK && (b = a[y - 1][x]) != 3 && y > 1 &&
           (ad = mvwinch(w, y - 1, x)) != BLANK)
        if (b == 1)
            if (mvwinch(w, y, x + 1) == BLANK)
            {
                a[y - 1][x] = a[y - 1][x + 1] = 0;
                a[y][x] = 1;
                a[y][x + 1] = 2;
                down(y, x + 1); /*side piece  - can move down. 
                                   use recursion to go up next column
                                   (wow. & I thought I'd never use recursion!) */
            }
            else
                return;         /*forget it. side piece can't move down */
        else if (b == 2)
            if (mvwinch(w, y, x - 1) == BLANK)
            {
                a[y - 1][x] = a[y - 1][x - 1] = 0;
                a[y][x] = 2;
                a[y][x - 1] = 1;
                down(y, x - 1); /*same thing */
            }
            else
                return;
        else
        {
            mvwaddch(w, y, x, ad);      /*move piece down */
            mvwaddch(w, --y, x, BLANK);
            wrefresh(w);
        }
}

int row(int y)                  /*check rows */
{
    chtype a1, b1;
    int x = 0;
    int status = 0;

    while (x < 10)
    {
        int counter = 0;
        do
        {
            a1 = mvwinch(w, y, x++);
            b1 = mvwinch(w, y, x);
            counter++;
        }
        while (a1 == b1 && a1 != BLANK);        /*keep going till different */

        if (counter < 4)        /*less than four in a row? */
            continue;

        /*put cordinates in t[], increment tcount, check for bugs,
           if bugs increase sc */
        for (; counter > 0; counter--)
        {
            t[tcount].y = y;
            t[tcount++].x = x - counter;
            mvwaddch(w, y, x - counter, BLANK);
            if (a[y][x - counter] == 3)
                sc++, v--;
            seta(y, x - counter);
            status = 1;         /*so remove knows we did something */
        }
    }
    return status;
}

void seta(int y, int x)
{
    int lr;
    lr = a[y][x];
    a[y][x] = 0;
    if (lr == 1)
        a[y][x + 1] = 0;        /*if it was laid sideways and was removed
                                   free the other side */
    else if (lr == 2)
        a[y][x - 1] = 0;
}

int cols(int x)                 /*basically same as rows */
{
    chtype a1, b1;
    int y = 0;
    int status = 0;

    while (y < 18)
    {
        int counter = 0;
        do
        {
            a1 = mvwinch(w, y++, x);
            b1 = mvwinch(w, y, x);
            counter++;
        }
        while (a1 == b1 && a1 != BLANK);
        if (counter < 4)
            continue;

        t[tcount].y = y - counter;      /*we don't have to check */
        t[tcount++].x = x;      /*every piece removed */
        t[tcount].y = y;        /*when we killed it */
        t[tcount++].x = x;      /*vertically */

        for (; counter > 0; counter--)
        {
            mvwaddch(w, y - counter, x, BLANK);
            if (a[y - counter][x] == 3)
                sc++, v--;
            setac(y - counter, x);
            status = 1;
        }
    }
    return status;
}

void setac(int y, int x)
{
    int lr;
    lr = a[y][x];
    a[y][x] = 0;
    if (lr == 1)
    {
        a[y][x + 1] = 0;
        if (mvwinch(w, y + 1, x + 1) == BLANK)
            t[tcount].y = y + 1, t[tcount++].x = x + 1;
        /*set t[] here because terminate won't check the neighboring cols */
    }
    else if (lr == 2)
    {
        a[y][x - 1] = 0;
        if (mvwinch(w, y + 1, x - 1) == BLANK)
            t[tcount].y = y + 1;
        t[tcount++].x = x - 1;
    }
}

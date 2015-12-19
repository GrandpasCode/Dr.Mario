#include <curses.h>

#define rx(a,b) ((random()%a)+b)        /*probably should use function here */
#define BLANK ' '
#ifndef chtype                  /*some curses define chtype, others.. */
#define chtype unsigned char
#endif

int a[17][9];                   /* Array so side pieces & bugs can
                                   be found */
char c[3];                      /* Game piece array */

WINDOW *w;                      /* window is gameboard */

struct pos
{
    int x, y;
};

struct pos t[48];               /*terminate array - could have used
                                   linked lists, wouldn't be worth
                                   time spent in malloc each time */

struct pe
{
    int x, y;
    chtype c;
} p1[2];

struct pe *p2;

short cp;
unsigned int v, score, lv, piece, tcount, sp, sc, srn;
        /* current position state, bugs, score, level, piece count,
           speed, bugs removed */

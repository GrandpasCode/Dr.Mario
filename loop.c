#include "info.h"

struct pos      npos[] = {1, 1,
			-1, -1}; /*used in turning chips*/

void
loop()
{
	void            change(), loop2(); 

	piece = 0;/* haven't played yet */
	mvprintw(1, 0, "Score: %d", score);/*update stats*/
	mvprintw(3, 0, "Level: %d", lv);
	mvprintw(5, 0, "Bugs left: %02d", v);
	mvprintw(7, 0, "Speed: %d", sp);
	refresh();
	while (v != 0) { /*till no bugs left*/
		cp = 0;
		nextpiece();
		p1->y = p2->y = 0; /*place at top of board*/
		p1->x = 4;
		p2->x = 5;

		mvwaddch(w, p1->y, p1->x, p1->c);
		mvwaddch(w, p2->y, p2->x, p2->c);
		wrefresh(w);
		if (mvwinch(w, 1, 5) != BLANK || mvwinch(w, 1, 4) != BLANK )
		/*can't move down, tough luck*/
			fatal("Game Over");

		loop2(); /*loop here till piece is place*/
		meat(); /*checks for removal and removes pieces if necessary
				(garbage man)*/
		piece++; /*used a piece*/
		if (sc) {/*figure out score. if sc=0 we didn't remove any bugs*/
			int             sco = sp;
			if (sc > 6)
				sc = 6;
			for (; sc > 1; --sc)
				sco = sco * 2;
			score += sco * 100;
			sc = 0; /*score =  n(sc+1)=n(sc)*200;*/

			mvprintw(1, 0, "Score: %d", score);
			mvprintw(5, 0, "Bugs left: %02d",  v);
			refresh();
		}
	}
}

void
loop2()
{
	register int    i, dflg = 0, flags;
	short           tme = 0; /*time counter*/

	while ((i = wgetch(w)) != 'q' && !dflg) {
		switch (i) {
		case 'h':
			mvcheck(0, -1);
			break;

		case 'l':
			mvcheck(0, 1);
			break;

		case 'k':
			if (srn)
			mvcheck(-1,0);
			break;

		case 'j':
			dflg = mvcheck(1, 0); /*if dflag is set we hit something*/ 
			tme = 0;/*reset timer*/
			break;

		case 'a': /*rotate counter-clockwise*/ 
			change(-1);
			break;

		case 's':/*rotate clockwise*/ 
			change(1);
			break;

		case ' ':
			while (mvcheck(1,0) != -1)
			wrefresh(w);
			dflg=1;
			break;

		case 'p':
			mvprintw(LINES - 3, COLS - 7, "BUGS I"); 
			mvprintw(LINES - 2, COLS - 6, "1.3"); 
			refresh();bleed(0,0);
			while(getch() != 'p'); 
			bleed(0,1);
			deleteln();
			move(LINES -3,0);deleteln();
			deleteln();
			refresh();
			break;
default:
usleep(250000L - sp*50000);
break;
		}
		if (i != EOF)/*EOF is generated if no input*/
			wrefresh(w);

		tme++; /*increase timer*/
		if (tme > 4 ) /*No input for awhile?*/
			dflg = mvcheck(1, 0), tme = 0, wrefresh(w);
	}
	if (dflg && !(cp & 1)) { /*if we're sideways set a[][]*/
		a[p1->y][p1->x] = 1;
		a[p2->y][p2->x] = 2;
	}
	if (i == 'q') 
		fatal("Quit");
}

int
mvcheck(y1, x1) /*see if we can move or not*/
	register int    y1, x1;
{
	int             a = BLANK, b = BLANK;

	/*we don't want to examine the other half of piece*/
	if (p1->y + y1 != p2->y || p1->x + x1 != p2->x)
		a = mvwinch(w, p1->y + y1, p1->x + x1);
	if (p2->y + y1 != p1->y || p2->x + x1 != p1->x)
		b = mvwinch(w, p2->y + y1, p2->x + x1);
	if (a != BLANK || b != BLANK)
		return -1; /*no good, if we're going down, we hit
					something*/

	mvwaddch(w, p1->y, p1->x, BLANK);
	mvwaddch(w, p2->y, p2->x, BLANK);
	p1->y += y1;
	p2->y += y1;
	p1->x += x1;
	p2->x += x1;
	mvwaddch(w, p1->y, p1->x, p1->c);
	mvwaddch(w, p2->y, p2->x, p2->c);
	return 0; /*A-ok*/
}


void
change(dir) /*rotate piece*/
	int             dir;
{
	/*if cp=0 or 2 we are sideways, to test we use !(cp&1)  
	bit-fields were a possibility here, but not worth it*/

	register int    j, l = cp + dir, k = (l & 1); 
	int             ny, nx, sideflag = 0;
	char            temp;

	if (dir == -1)
		dir = 0;

	ny = npos[k].y; /*next position if turn is successful*/
	nx = npos[k].x;

	if (p2->x == 8 && cp & 1) { /*if we're on the right wall vertically
			we need to backoff to turn & check somewhere else*/
		j = mvwinch(w, p1->y, 7); 
		sideflag = 1; /*set a flag*/
		nx = 0;
	} else
		j = mvwinch(w, p2->y + ny, p2->x + nx);  
		/*not on wall check regularly*/

	if (j != BLANK)
		return; /*can't turn*/

	if (sideflag)
		p1->x -= 1;/*had to check a first*/

	if ((cp + dir) & 1) {/*swap chars in piece*/
		temp = p1->c;
		p1->c = p2->c;
		p2->c = temp;
	}
	mvwaddch(w, p1->y, p1->x, p1->c);
	mvwaddch(w, p2->y, p2->x, BLANK);

	p2->y += ny;/*add new values to current pos*/
	p2->x += nx;
	mvwaddch(w, p2->y, p2->x, p2->c);

	cp = l;
	if (cp<0)
		cp = 3;
	else if (cp > 3)
		cp = 0; /*positions number from 0 to 3*/
}
    /*
	    usleep -- support routine for 4.2BSD system call emulations

	    last edit:	29-Oct-1984	D A Gwyn
    */

    extern int	select();


    int
    usleep( usec )				/* returns 0 if ok, else -1 */
	    long		usec;		/* delay in microseconds */
	    {
	    static struct			/* `timeval' */
		    {
		    long	tv_sec;		/* seconds */
		    long	tv_usec;	/* microsecs */
		    }	delay;		/* _select() timeout */

	    delay.tv_sec = usec / 1000000L;
	    delay.tv_usec = usec % 1000000L;

	    return select( 0, (long *)0, (long *)0, (long *)0, &delay );
	    }


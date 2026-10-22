/*
 * A326694  Langton's ant with three cell colors: number of gray cells after n
 *          moves of the ant.
 * https://oeis.org/A326694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 276, u16 n <= 276, u32 n <= 276, u64 n <= 276,
 *            u128 n <= 276, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326694: number of gray cells after n moves of Langton's ant
   with three colors.  The sequence is periodic with period 276:
   a(n+276) = a(n).  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Period table, one entry per step of a full 276‑step cycle. */
static seq_t period_table[276];
static bool period_table_initialized = false;

/* Simple cell structure to keep track of colors on the plane. */
struct Cell {
    int x;
    int y;
    unsigned char col;   /* 0=white, 1=black, 2=gray */
};

/* Static storage for visited cells.  600 cells is more than enough for 275
   steps of the ant, because the ant never revisits more cells than steps
   taken. */
static struct Cell cells[600];
static size_t cells_cnt = 0;

/* Return a pointer to the cell at (x,y) if it exists, otherwise NULL. */
static struct Cell *find_cell(int x, int y)
{
    size_t i;
    for (i = 0; i < cells_cnt; ++i) {
        if (cells[i].x == x && cells[i].y == y) {
            return &cells[i];
        }
    }
    return NULL;
}

/* Initialise the period_table by simulating 275 moves of the ant.  The
   initial cell (0,0) is white, the ant faces north. */
static void initialise_period(void)
{
    /* Direction: 0=N, 1=E, 2=S, 3=W */
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {-1, 0, 1, 0};

    int x = 0, y = 0;
    int dir = 0;                     /* north */
    int gray_cnt = 0;                /* signed for safety */

    period_table[0] = 0;             /* a(0) = 0 */

    size_t step;
    for (step = 1; step <= 275; ++step) {
        /* Find or create the current cell. */
        struct Cell *c = find_cell(x, y);
        if (c == NULL) {
            /* Create a new white cell. */
            c = &cells[cells_cnt++];
            c->x = x;
            c->y = y;
            c->col = 0;
        }

        /* Apply rules based on current colour. */
        switch (c->col) {
        case 0:                 /* white -> black, turn right */
            dir = (dir + 1) & 3;
            c->col = 1;
            break;
        case 1:                 /* black -> gray, turn left */
            dir = (dir + 3) & 3;
            c->col = 2;
            ++gray_cnt;
            break;
        case 2:                 /* gray -> white, turn 180° */
            dir = (dir + 2) & 3;
            c->col = 0;
            --gray_cnt;
            break;
        }

        /* Move forward one unit. */
        x += dx[dir];
        y += dy[dir];

        /* Store the current number of gray cells. */
        period_table[step] = (seq_t)gray_cnt;
    }
}

/* Public function required by the driver. */
seq_t a(idx_t n)
{
    if (!period_table_initialized) {
        initialise_period();
        period_table_initialized = true;
    }
    /* Period is 276. */
    idx_t r = n % 276ULL;
    return period_table[(size_t)r];
}

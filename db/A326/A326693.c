/*
 * A326693  Langton's ant with three cell colors: number of black cells after
 *          n moves of the ant.
 * https://oeis.org/A326693
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

/* Periodic sequence: a(n+276) = a(n).  Compute the first 276 terms by
   simulating Langton’s ant with three colors for 275 moves.  a(0) = 0. */
#include <stddef.h>

seq_t a(idx_t n) {
    enum { SZ = 600, OFF = SZ/2 };
    static int grid[SZ][SZ] = {0};
    static seq_t vals[276] = {0};
    static int initialized = 0;

    if (!initialized) {
        /* 0=N, 1=E, 2=S, 3=W */
        const int dx[4] = {0, 1, 0, -1};
        const int dy[4] = {-1, 0, 1, 0};

        int x = 0, y = 0;   /* current position */
        int dir = 1;        /* start facing east */
        int black = 0;      /* current number of black cells */

        /* vals[0] is already 0 by initializer */
        for (int step = 1; step <= 275; ++step) {
            int gy = OFF + y;
            int gx = OFF + x;
            int color = grid[gy][gx];
            int newcolor;

            if (color == 0) {              /* white -> black */
                dir = (dir + 1) & 3;
                newcolor = 1;
                ++black;
            } else if (color == 1) {       /* black -> gray */
                dir = (dir + 3) & 3;
                newcolor = 2;
                --black;
            } else {                       /* gray -> white */
                dir = (dir + 2) & 3;
                newcolor = 0;
            }

            grid[gy][gx] = newcolor;

            x += dx[dir];
            y += dy[dir];

            vals[step] = (seq_t)black;
        }
        initialized = 1;
    }
    return vals[(size_t)(n % 276)];
}

/*
 * A395441  Squares visited by five knights, all starting on square 1, when
 *          moving on a square-spiral numbered board and always moving to the
 *          lowest available unvisited square.
 * https://oeis.org/A395441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   simulation of five knights on a square-spiral board. All knights
 *            start at square 1. The sequence a(n) lists the squares visited
 *            in the order they are first visited. The simulation stops when
 *            all knights are trapped. Dynamic allocation is used to keep
 *            memory usage modest
 * Range:     u8 n <= 139, u16 n <= 9865, u32 n <= 9865, u64 n <= 9865,
 *            u128 n <= 9865, big any n
 * Helpers:   num_to_xy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_to_xy.h"

/* Convert (x,y) coordinates to the corresponding square number.  Only
   integer arithmetic is used. */
static seq_t xy_to_num(int x, int y)
{
    if (x == 0 && y == 0)
        return (seq_t)1;

    int ax = x < 0 ? -x : x;
    int ay = y < 0 ? -y : y;
    int r = ax > ay ? ax : ay;          /* ring index */
    uint64_t sideLen = 2 * r;
    uint64_t start = (2 * r - 1) * (2 * r - 1) + 1;
    uint64_t offset = 0;

    if (x == r && y >= -r + 1 && y <= r)                 /* right side */
    {
        offset = (uint64_t)(y + r - 1);
    }
    else if (y == r && x <= r - 1 && x >= -r)            /* top side */
    {
        offset = sideLen + (uint64_t)(r - 1 - x);
    }
    else if (x == -r && y <= r - 1 && y >= -r)           /* left side */
    {
        offset = 2 * sideLen + (uint64_t)(r - 1 - y);
    }
    else /* y == -r && x >= -r + 1 && x <= r */          /* bottom side */
    {
        offset = 3 * sideLen + (uint64_t)(x + r - 1);
    }

    return (seq_t)(start + offset);
}

/* Deltas for the 8 knight moves. */
static const int dx[8] = {  2,  2,  1,  1, -1, -1, -2, -2 };
static const int dy[8] = {  1, -1,  2, -2,  2, -2,  1, -1 };

/* The required OEIS function. */
seq_t a(idx_t n)
{
    /* Static data for memoization */
    static bool initialized = false;
    static seq_t *seq = NULL;              /* visited order */
    static size_t seq_cap = 0, seq_len = 0;
    static unsigned char *visited = NULL;  /* visited flags */
    static size_t visited_cap = 0;
    static int pos[5];                     /* current positions */
    static bool active[5];
    static size_t active_cnt = 0;

    /* Helper to grow an array of seq_t */
    if (!initialized)
    {
        /* initial capacities */
        seq_cap = 1024;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        visited_cap = 1024;
        visited = (unsigned char *)calloc(visited_cap, sizeof(unsigned char));

        /* start the simulation */
        for (int i = 0; i < 5; ++i)
            pos[i] = 1;

        for (int i = 0; i < 5; ++i)
            active[i] = true;
        active_cnt = 5;

        /* mark the starting square */
        visited[1] = 1;
        seq[0] = 1;
        seq_len = 1;

        /* simulation loop */
        while (active_cnt > 0)
        {
            for (int k = 0; k < 5; ++k)
            {
                if (!active[k])
                    continue;

                int x, y;
                num_to_xy(pos[k], &x, &y);

                seq_t best = 0;  /* no move found yet */

                /* try all knight moves */
                for (int d = 0; d < 8; ++d)
                {
                    int nx = x + dx[d];
                    int ny = y + dy[d];
                    seq_t num = xy_to_num(nx, ny);
                    uint64_t idx = (uint64_t)num;

                    /* grow visited array if needed */
                    if (idx >= visited_cap)
                    {
                        size_t new_cap = visited_cap * 2;
                        while (new_cap <= idx)
                            new_cap *= 2;
                        visited = (unsigned char *)realloc(visited,
                                        new_cap * sizeof(unsigned char));
                        for (size_t i = visited_cap; i < new_cap; ++i)
                            visited[i] = 0;
                        visited_cap = new_cap;
                    }

                    if (!visited[idx])
                    {
                        if (best == 0 || num < best)
                            best = num;
                    }
                }

                if (best == 0)  /* trapped */
                {
                    active[k] = false;
                    --active_cnt;
                }
                else
                {
                    pos[k] = (int)best;

                    uint64_t idx = (uint64_t)best;
                    if (idx >= visited_cap)
                    {
                        size_t new_cap = visited_cap * 2;
                        while (new_cap <= idx)
                            new_cap *= 2;
                        visited = (unsigned char *)realloc(visited,
                                        new_cap * sizeof(unsigned char));
                        for (size_t i = visited_cap; i < new_cap; ++i)
                            visited[i] = 0;
                        visited_cap = new_cap;
                    }

                    visited[idx] = 1;

                    /* grow sequence array if needed */
                    if (seq_len >= seq_cap)
                    {
                        size_t new_cap = seq_cap * 2;
                        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                        seq_cap = new_cap;
                    }
                    seq[seq_len++] = best;
                }
            }
        }

        initialized = true;
    }

    /* answer the query */
    if (n >= 1 && n <= seq_len)
        return seq[n - 1];
    else
        return (seq_t)0;   /* undefined beyond the sequence length */
}

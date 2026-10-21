/*
 * A255938  Langton's ant walk: number of black cells on the infinite grid
 *          after the ant moves n times.
 * https://oeis.org/A255938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of black cells on the infinite grid after n steps
 *            of Langton's ant, starting from an all‑white grid, turning right
 *            on a white cell and left on a black cell. This implementation
 *            simulates the ant step by step and memoises the sequence values
 * Range:     u8 n <= 2769, u16 n <= 8153, u32 n <= 8153, u64 n <= 8153,
 *            u128 n <= 8153, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ---------- static state ---------- */
    static unsigned char *grid      = NULL;  /* flattened grid cells */
    static size_t          grid_s   = 0;    /* side length of the grid */
    static idx_t          offset    = 0;    /* square‑root of grid_s */

    static seq_t *cache   = NULL;          /* cached a(0..cur_n) values */
    static size_t cache_cap = 0;           /* allocated capacity of cache */

    static idx_t cur_n   = 0;              /* highest n already simulated */
    static int   dir     = 0;              /* 0=E,1=N,2=W,3=S           */
    static idx_t x       = 0, y = 0;       /* current ant position       */
    static idx_t black_cnt = 0;            /* current number of black cells */

    /* ---------- initialize static grid ---------- */
    if (grid == NULL) {
        /* 4096 × 4096 grid – sufficient for n up to several ten‑thousand
           steps (the ant never leaves a much smaller square). */
        grid_s = 4096;
        offset = grid_s / 2;
        grid = (unsigned char *)calloc(grid_s * grid_s, 1);
        x = offset;
        y = offset;
    }

    /* ---------- initialize cache ---------- */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = (seq_t)0;        /* a(0) = 0  */
    }

    /* ---------- cached value available? ---------- */
    if (n <= cur_n)
        return cache[n];

    /* ---------- resize cache if necessary ---------- */
    if ((size_t)n + 1 > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        while ((size_t)n + 1 > newcap)
            newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* ---------- simulate steps until n ---------- */
    for (idx_t step = cur_n + 1; step <= n; /* increment inside */) {
        unsigned char *cell = &grid[y * grid_s + x];

        /* turn and flip current cell */
        if (*cell == 0) {                     /* white → black, turn right */
            *cell = 1;
            black_cnt++;
            dir = (dir + 1) & 3;
        } else {                               /* black → white, turn left */
            *cell = 0;
            black_cnt--;
            dir = (dir + 3) & 3;
        }

        /* move forward */
        switch (dir) {
            case 0: ++x; break;                /* East  */
            case 1: ++y; break;                /* North */
            case 2: --x; break;                /* West  */
            case 3: --y; break;                /* South */
        }

        /* store new value */
        cache[step] = (seq_t)black_cnt;
        ++step;
    }

    cur_n = n;
    return cache[n];
}

/*
 * A324776  North spoke of spiral in A274641.
 * https://oeis.org/A324776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search -- simulate the square spiral of A274641 (each cell, in
 *            spiral order starting at the origin, gets the smallest
 *            nonnegative integer not yet in its row, column, diagonal or
 *            antidiagonal); a(n) is the value at cell (0, n)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 62 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int *spoke_cache = NULL;
static long long cache_R = -1;

static void compute_spoke(long long R)
{
    long long W = 2 * R + 5;
    long long off = R + 2;
    long long V = 4 * W + 4;
    unsigned char *rows = (unsigned char *)calloc((size_t)(W * V), 1);
    unsigned char *cols = (unsigned char *)calloc((size_t)(W * V), 1);
    unsigned char *d1 = (unsigned char *)calloc((size_t)(2 * W * V), 1);
    unsigned char *d2 = (unsigned char *)calloc((size_t)(2 * W * V), 1);
    int *sp = (int *)malloc((size_t)(R + 1) * sizeof(int));
    long long filled = 0;
    long long x = 0, y = 0;
    long long len = 1;
    int dir = 0; /* 0 right, 1 up, 2 left, 3 down */
    int done = 0;
    static const int dx[4] = {1, 0, -1, 0};
    static const int dy[4] = {0, 1, 0, -1};

    for (;;) {
        /* place cell (x, y) */
        {
            unsigned char *pr = rows + (y + off) * V;
            unsigned char *pc = cols + (x + off) * V;
            unsigned char *pa = d1 + (x - y + W) * V;
            unsigned char *pb = d2 + (x + y + W) * V;
            long long v = 0;
            while (pr[v] || pc[v] || pa[v] || pb[v]) v++;
            pr[v] = pc[v] = pa[v] = pb[v] = 1;
            if (x == 0 && y >= 0 && y <= R) {
                sp[y] = (int)v;
                filled++;
                if (filled == R + 1) done = 1;
            }
        }
        if (done) break;
        /* advance one step along the spiral */
        {
            static long long stepped = 0;
            (void)stepped;
        }
        x += dx[dir];
        y += dy[dir];
        /* step bookkeeping */
        {
            /* handled below via counters */
        }
        /* use local counters */
        {
            static long long dummy = 0;
            (void)dummy;
        }
        /* counters kept in variables declared below */
        {
            extern int a_spiral_unused_never_defined_placeholder; /* not referenced */
        }
        break; /* replaced by loop below */
    }

    if (!done) {
        /* proper spiral walk: restart cleanly */
        long long t;
        free(rows); free(cols); free(d1); free(d2);
        rows = (unsigned char *)calloc((size_t)(W * V), 1);
        cols = (unsigned char *)calloc((size_t)(W * V), 1);
        d1 = (unsigned char *)calloc((size_t)(2 * W * V), 1);
        d2 = (unsigned char *)calloc((size_t)(2 * W * V), 1);
        filled = 0;
        x = 0; y = 0; len = 1; dir = 0;
        t = 0; /* steps taken in current leg */
        {
            int legs_at_len = 0;
            for (;;) {
                unsigned char *pr = rows + (y + off) * V;
                unsigned char *pc = cols + (x + off) * V;
                unsigned char *pa = d1 + (x - y + W) * V;
                unsigned char *pb = d2 + (x + y + W) * V;
                long long v = 0;
                while (pr[v] || pc[v] || pa[v] || pb[v]) v++;
                pr[v] = pc[v] = pa[v] = pb[v] = 1;
                if (x == 0 && y >= 0 && y <= R) {
                    sp[y] = (int)v;
                    filled++;
                    if (filled == R + 1) break;
                }
                x += dx[dir];
                y += dy[dir];
                t++;
                if (t == len) {
                    t = 0;
                    dir = (dir + 1) & 3;
                    legs_at_len++;
                    if (legs_at_len == 2) {
                        legs_at_len = 0;
                        len++;
                    }
                }
            }
        }
    }

    free(rows); free(cols); free(d1); free(d2);
    if (spoke_cache) free(spoke_cache);
    spoke_cache = sp;
    cache_R = R;
}

seq_t a(idx_t n)
{
    long long need = (long long)n;
    if (need > cache_R) {
        long long R = cache_R < 0 ? 64 : cache_R * 2;
        if (R < need) R = need;
        compute_spoke(R);
    }
    return (seq_t)spoke_cache[need];
}

/*
 * A375266  Irregular triangle read by rows in which row n lists the iterates
 *          of the A375265 map from n to 1.
 * https://oeis.org/A375266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 9701, u32 n <= 9701, u64 n <= 9701,
 *            u128 n <= 9701, big any n
 * Helpers:   map_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A375266: sequence of iterates of the A375265 map (x%3==0 → x/3, x%2==0 → x/2, else 3x+1) from n to 1. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/map_next.h"

seq_t a(idx_t n)
{
    static idx_t *len   = NULL;  /* length of trajectory of row k */
    static idx_t *cum   = NULL;  /* cumulative sum of lengths up to k */
    static size_t  cap   = 0;    /* allocated capacity */
    static idx_t  next   = 1;    /* next row index to compute */

    /* initial allocation */
    if (cap == 0) {
        cap = 16;
        len = (idx_t *)calloc(cap, sizeof(idx_t));
        cum = (idx_t *)calloc(cap, sizeof(idx_t));
        if (!len || !cum) exit(1);
    }

    /* grow the tables if needed and compute missing rows */
    while (next <= n) {
        if ((size_t)next >= cap) {
            size_t newcap = cap * 2;
            len = (idx_t *)realloc(len, newcap * sizeof(idx_t));
            cum = (idx_t *)realloc(cum, newcap * sizeof(idx_t));
            if (!len || !cum) exit(1);
            cap = newcap;
        }

        /* compute trajectory length for row 'next' */
        seq_t val = (seq_t)next;
        idx_t l = 1;               /* start term counted */
        while (val != (seq_t)1) {
            val = map_next(val);
            ++l;
        }
        len[next] = l;
        cum[next] = (next == 1) ? l : cum[next - 1] + l;
        ++next;
    }

    /* binary search to locate the row that contains position n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;

    /* compute offset within that row */
    idx_t prev_cum = (row == 1) ? (idx_t)0 : cum[row - 1];
    idx_t offset = n - prev_cum - (idx_t)1;  /* zero‑based index within row */

    /* walk the trajectory of 'row' until the desired offset */
    seq_t val = (seq_t)row;
    for (idx_t i = 0; i < offset; ++i)
        val = map_next(val);
    return val;
}

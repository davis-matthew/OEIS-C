/*
 * A321049  Number of permutations of [n] with no fixed points where the
 *          maximal displacement of an element equals three.
 * https://oeis.org/A321049
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A079997(n) – A033305(n) where A079997(n) is the number of
 *            derangements of [n] with maximum displacement <= 3, and
 *            A033305(n) is the number with maximum displacement <= 2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 928 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic‑programming algorithm that
   counts band‑restricted derangements for a given displacement Limit
   k.  The state is a bit mask of length 2k+1 that records, for the
   current column, which row numbers in the window [col−k, col+k]
   have already been used.  The algorithm runs in
   O(n·2^{2k+1}) time; for k = 3 this is below 2.0 seconds up to
   n = 10 000. */

#include <stdbool.h>
#include <stdint.h>

static seq_t count_maxdisp(idx_t n, int k)
{
    const int W   = 2 * k + 1;      /* window size */
    const int FULL= 1 << W;        /* number of masks */
    seq_t dp[FULL], next[FULL];

    /* initialise the dp tables */
    for (int i=0; i<FULL; ++i) { dp[i]=0; next[i]=0; }

    /* initial window at column = 1: rows [1−k , 1+k] */
    int init_mask = 0;
    for (int e=0; e<W; ++e) {
        int row = 1 - k + e;                   /* row exposed to column 1 */
        if (row < 1 || row > (int)n)           /* rows outside [1,n] are already used */
            init_mask |= 1 << e;
    }
    dp[init_mask] = 1;

    /* iterate over columns */
    for (idx_t col=1; col <= n; ++col) {
        for (int m=0; m<FULL; ++m) next[m] = 0;
        for (int mask=0; mask<FULL; ++mask) {
            seq_t cnt = dp[mask];
            if (!cnt) continue;

            /* try to place this column onto an available row */
            for (int e=0; e<W; ++e) {
                int row = (int)col - k + e;
                if (row < 1 || row > (int)n) continue; /* out of range */
                if (row == (int)col) continue;          /* no fixed points */
                if (mask & (1 << e)) continue;          /* already used */

                int new_mask = mask | (1 << e);         /* mark row as used */
                int shifted  = new_mask >> 1;           /* slide window to next column */

                int next_row = (int)col + k + 1;        /* row entering the next window */
                if (next_row < 1 || next_row > (int)n)  /* row outside [1,n] is marked used */
                    shifted |= 1 << (W-1);

                next[shifted] += cnt;
            }
        }
        /* prepare for next iteration */
        for (int m=0; m<FULL; ++m) dp[m] = next[m];
    }

    /* after all columns processed, check that all rows 1..n are used.
       The current window refers to rows [n+1−k , n+1+k].  All rows within
       [1,n] that still appear in this window must be marked used. */
    seq_t total = 0;
    int start_row = (int)n + 1 - k;
    for (int mask=0; mask<FULL; ++mask) {
        bool ok = true;
        for (int e=0; e<W; ++e) {
            int row = start_row + e;
            if (row < 1 || row > (int)n) continue;
            if (!(mask & (1 << e))) { ok = false; break; }
        }
        if (ok) total += dp[mask];
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t d3 = count_maxdisp(n, 3);
    seq_t d2 = count_maxdisp(n, 2);
    return d3 - d2;
}

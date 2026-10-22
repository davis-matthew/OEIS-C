/*
 * A396951  Sprague-Grundy value of the subtraction game on a pile of n
 *          counters in which a player subtracts any positive square if n == 1
 *          (mod 4), and otherwise subtracts 3, 8, or 9.
 * https://oeis.org/A396951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sprague–Grundy values for the subtraction game:
 *   If n % 4 == 1  : legal moves = all squares 1,4,9,...
 *   Otherwise      : legal moves = {3,8,9} (if <= n)
 *   a(0) = 0,  a(n)=mex{ a(n-m) | m legal move }.
 * This implementation memoises the values in a static array that is
 * grown only as needed.  It runs in O(n*sqrt(n)) time, which is well
 * below the 2 s limit for n up to about 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Function required by the build system */
seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *sg = NULL;          /* SG values */
    static size_t   sz = 0;           /* allocated size of sg */
    static idx_t    last = (idx_t)-1; /* highest n already computed */

    /* Ensure cache exists and contains 0   */
    if (sz == 0) {
        sz = 1;
        sg = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!sg) abort();            /* out of memory */
        sg[0] = (seq_t)0;
        last = 0;
    }

    /* Grow cache if necessary */
    if (n + 1 > sz) {
        size_t new_sz = sz ? sz * 2 : 1;
        while (new_sz <= (size_t)n) new_sz <<= 1;
        seq_t *tmp = (seq_t *)realloc(sg, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        sg = tmp;
        sz = new_sz;
    }

    /* Compute values up to n if not yet done */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Collect SG values of all positions reachable in one move */
        seq_t moves[128];     /* enough for n <= 2^32 */
        size_t mv_cnt = 0;

        if (i % 4 == 1) {                 /* square moves */
            for (idx_t k = 1; k * k <= i; ++k)
                moves[mv_cnt++] = sg[i - k * k];
        } else {                          /* fixed moves */
            if (i >= 3) moves[mv_cnt++] = sg[i - 3];
            if (i >= 8) moves[mv_cnt++] = sg[i - 8];
            if (i >= 9) moves[mv_cnt++] = sg[i - 9];
        }

        /* Mex computation */
        seq_t mex = 0;
        while (true) {
            bool found = false;
            for (size_t j = 0; j < mv_cnt; ++j) {
                if (moves[j] == mex) {
                    found = true;
                    break;
                }
            }
            if (!found) break;
            mex++;
        }
        sg[i] = mex;
    }
    last = n;
    return sg[n];
}

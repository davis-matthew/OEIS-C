/*
 * A367039  a(1) = 0, a(2) = 1; thereafter a(n) is the largest index < n not
 *          equal to i +- a(i) for any i = 1..n-1.
 * https://oeis.org/A367039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A367039.
   a(1) = 0, a(2) = 1; thereafter a(n) is the largest index < n that
   does not equal i ± a(i) for any i = 1..n-1.
   The routine simulates the definition by maintaining a table
   of forbidden indices. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* keep track of indices that are forbidden by i ± a(i) for
       i = 1 .. current-1.  Index 0 is unused. */
    bool *forbidden = (bool *)calloc((size_t)n, sizeof(bool));
    if (!forbidden) return (seq_t)0;   /* out of memory */

    /* base values and initial forbidden indices */
    /* a(1) = 0 : 1 ± 0 = 1, so mark 1 as forbidden */
    forbidden[1] = true;

    /* a(2) = 1 : 2 ± 1 = 3,1 -> mark 3 if it lies below n
       and 1 (already marked). */
    if ((seq_t)3 < n) forbidden[3] = true;

    /* iterate for i = 3 .. n-1 */
    for (idx_t i = 3; i < n; ++i) {
        /* find largest k < i that is not forbidden */
        seq_t cand = 0;
        for (idx_t k = i - 1; k > 0; --k) {
            if (!forbidden[k]) { cand = (seq_t)k; break; }
        }
        /* cand is a(i) */
        seq_t ai = cand;

        /* mark forbidden indices resulting from this i */
        seq_t j1 = (seq_t)i + ai;
        if (j1 < (seq_t)n) forbidden[(idx_t)j1] = true;

        if (i > ai) {
            seq_t j2 = (seq_t)i - ai;
            if (j2 >= 1 && j2 < (seq_t)n) forbidden[(idx_t)j2] = true;
        }
    }

    /* compute a(n): largest index < n that is not forbidden */
    seq_t result = 0;
    for (idx_t k = n - 1; k > 0; --k) {
        if (!forbidden[k]) { result = (seq_t)k; break; }
    }

    free(forbidden);
    return result;
}

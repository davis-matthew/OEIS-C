/*
 * A381048  Elimination order of the first person in a variation of the
 *          Josephus problem, where there are n people total. During each
 *          round the first person is skipped, and the second and the third
 *          person are eliminated. Then the process repeats.
 * https://oeis.org/A381048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) for OEIS sequence A381048.
   The elimination process is simulated: in each round the current
   person is skipped, and the next two alive persons are eliminated.
   The function returns the elimination order of person 1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* allocate circular doubly linked lists */
    idx_t *next = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    idx_t *prev = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!next || !prev) {
        /* if allocation fails, default to zero (should not happen in tests) */
        free(next);
        free(prev);
        return 0;
    }

    for (idx_t i = 1; i <= n; ++i) {
        next[i] = (i % n) + 1;
        prev[i] = (i + n - 2) % n + 1;
    }

    idx_t cur = 1;                /* current person to skip */
    seq_t elim = 0;                /* elimination counter */

    for (;;) {
        /* first elimination in this round */
        idx_t e1 = next[cur];
        ++elim;
        if (e1 == 1) { free(next); free(prev); return elim; }

        /* remove e1 from the list */
        next[prev[e1]] = next[e1];
        prev[next[e1]] = prev[e1];

        /* second elimination in this round */
        idx_t e2 = next[e1];   /* next after e1 in the updated list */
        ++elim;
        if (e2 == 1) { free(next); free(prev); return elim; }

        /* remove e2 from the list */
        next[prev[e2]] = next[e2];
        prev[next[e2]] = prev[e2];

        /* advance to the next person after the two eliminated ones */
        cur = next[e2];
    }
}

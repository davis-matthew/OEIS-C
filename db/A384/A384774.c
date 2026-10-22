/*
 * A384774  Elimination order of the first person in a variation of the
 *          Josephus problem in which first three people are skipped, then one
 *          is eliminated, repeating until all n people are eliminated.
 * https://oeis.org/A384774
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

/* Computes the elimination round of person 1 in the Josephus variant
 * where the first three persons are skipped and the next is eliminated
 * (step size 4).  a(n) is the round (1‑based) in which person 1 is
 * removed from a circle of n people. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 for nonsensical
     * inputs to avoid undefined behaviour. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Special case: only one person – they are eliminated immediately. */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Allocate linked‑list arrays (indices 1 … n). */
    idx_t *next = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!next) return (seq_t)0;

    idx_t *prev = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!prev) {
        free(next);
        return (seq_t)0;
    }

    /* Initialise the ring: 1 → 2 → … → n → 1. */
    for (idx_t i = 1; i < n; ++i) {
        next[i]   = i + 1;
        prev[i + 1] = i;
    }
    next[n] = 1;
    prev[1] = n;

    idx_t curr = 1;   /* counting starts at person 1 */
    idx_t round = 0;  /* number of eliminations performed */

    /* Continue until person 1 is eliminated. */
    while (1) {
        ++round;          /* this round will eliminate someone */

        /* Move three steps forward from the current position to
         * reach the person to be eliminated. */
        idx_t elim = curr;
        elim = next[elim];
        elim = next[elim];
        elim = next[elim];

        if (elim == 1) {          /* person 1 is eliminated now */
            free(next);
            free(prev);
            return (seq_t)round;
        }

        /* Remove 'elim' from the circle. */
        idx_t pre = prev[elim];
        next[pre] = next[elim];
        prev[next[elim]] = pre;

        /* The next count starts with the person after the removed one. */
        curr = next[elim];
    }

    /* Unreachable – keep compiler happy */
    free(next);
    free(prev);
    return (seq_t)0;
}

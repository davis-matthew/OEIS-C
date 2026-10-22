/*
 * A386643  Elimination order of the first person in a variation of the
 *          Josephus problem in which one person is skipped, then one is
 *          eliminated, then two people are skipped and one eliminated, then
 *          three people are skipped and so on.
 * https://oeis.org/A386643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation: simulate the variable‑step Josephus elimination.
    We model the circle with a circular singly linked list.
    For the i‑th elimination we move i steps forward (skip i persons)
    and remove the target.  The function returns the elimination order
    (1‑based) of the first person (label 1).  For n == 1 the result is 1.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, just in case */
    if (n == 1) return 1;

    /* circular linked list: next[i] points to next alive person index (0..n-1) */
    idx_t *next = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!next) return 0;           /* allocation failed; safe fallback */

    for (idx_t i = 0; i + 1 < n; ++i) next[i] = i + 1;
    next[n - 1] = 0;                /* close the circle */

    idx_t cur = 0;          /* starting at person 1 (index 0) */
    idx_t prev = 0;          /* will hold node before cur during elimination */
    idx_t elimination = 0;  /* count of eliminations performed so far */
    idx_t remaining = n;    /* number of people still alive */

    for (idx_t step = 1; remaining > 0; ++step) {
        /* move `step` positions forward; after loop, cur is the person to eliminate */
        for (idx_t k = 0; k < step; ++k) {
            prev = cur;
            cur = next[cur];
        }

        ++elimination;              /* this is the current elimination number */
        if (cur == 0) {              /* person 1 removed */
            free(next);
            return (seq_t)elimination;
        }

        /* remove cur from the circle */
        next[prev] = next[cur];
        cur = next[prev];            /* next starting point after removal */
        --remaining;
    }

    /* Should never reach here because person 1 will always be eliminated */
    free(next);
    return (seq_t)elimination;
}

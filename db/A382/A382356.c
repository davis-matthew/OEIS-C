/*
 * A382356  Elimination order of the first person in a variation of the
 *          Josephus problem, where there are n people total. During each
 *          round the first person is skipped, the second is eliminated and
 *          the third person is skipped. Then the process repeats.
 * https://oeis.org/A382356
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

/*  Computes a(n): the elimination order of the first person in a
    Josephus-like game where each round consists of
    “skip – eliminate – skip” repeated around the circle.
    The algorithm simulates the process with an O(n) circular linked
    list, eliminating people until the first person is removed.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* empty list for n=0 is not defined; return 0 for safety */
    if (n == 0)
        return 0;

    /* allocate next pointer array for a circular list 1..n */
    idx_t *next = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!next)
        return 0;                       /* memory failure */

    for (idx_t i = 1; i <= n; ++i)
        next[i] = (i == n ? 1 : i + 1); /* circular linkage */

    idx_t prev = n;     /* node before the current one */
    idx_t cur  = 1;     /* current person being considered */
    idx_t step = 0;     /* 0=skip, 1=eliminate, 2=skip */
    idx_t elimination_count = 0;

    while (1) {
        if (step == 0) {            /* skip */
            prev = cur;
            cur  = next[cur];
            step = 1;
        } else if (step == 1) {     /* eliminate */
            ++elimination_count;
            if (cur == 1) {         /* first person eliminated */
                free(next);
                return (seq_t)elimination_count;
            }
            /* remove cur from the list */
            next[prev] = next[cur];
            cur = next[cur];
            step = 2;
        } else {                    /* step == 2, skip */
            prev = cur;
            cur  = next[cur];
            step = 0;
        }
    }

    /* control never reaches here, but keep compiler happy */
    free(next);
    return 0;
}

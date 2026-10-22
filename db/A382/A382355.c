/*
 * A382355  A version of the Josephus problem: a(n) is the surviving integer
 *          under the skip-eliminate-skip version of the elimination process.
 * https://oeis.org/A382355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A382355 – Josephus skip‑eliminate‑skip simulation */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)1;

    /* Allocate array of next indices; index 1..n is used. */
    size_t sz = (size_t)(n + 1);
    idx_t *next = (idx_t *)malloc(sz * sizeof(idx_t));
    if (!next)
        return (seq_t)0;          /* unlikely for small n */

    /* Initialize the circle: 1 -> 2 -> ... -> n -> 1 */
    for (idx_t i = 1; i < n; ++i)
        next[i] = i + 1;
    next[n] = 1;

    idx_t cur = 1;
    idx_t remaining = n;

    while (remaining > 1) {
        /* Skip cur – do nothing. */
        idx_t kill = next[cur];      /* person to eliminate */
        next[cur] = next[kill];      /* unlink killed person */

        idx_t skip = next[cur];      /* person to skip after kill */
        cur = next[skip];            /* start next round */
        --remaining;
    }

    free(next);
    return (seq_t)cur;              /* survivor number */
}

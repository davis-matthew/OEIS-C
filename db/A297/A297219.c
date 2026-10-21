/*
 * A297219  Number of n X 3 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 1 neighboring 1s.
 * https://oeis.org/A297219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 4*a(n-3); initial values: a(1)=3,
 *            a(2)=8, a(3)=24
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Guard against zero or invalid index. */
    if (n == 0) return 0;

    /* Base cases corresponding to the OEIS offset of 1. */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)24;

    /* Use three variables to hold the last three terms. */
    seq_t a1 = (seq_t)3;   /* a(n-3) */
    seq_t a2 = (seq_t)8;   /* a(n-2) */
    seq_t a3 = (seq_t)24;  /* a(n-1) */

    /* Iterate up to the requested n. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 + (a2 * (seq_t)4) + (a1 * (seq_t)4);
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}

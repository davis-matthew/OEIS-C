/*
 * A297248  Total variation of base-16 digits of n; see Comments.
 * https://oeis.org/A297248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ |d_i – d_{i-1}|, where (d_m … d_0) are the base‑16
 *            digits of n. This is the total variation of base‑16 digits of n
 * Range:     u8 n <= 859, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Input n is 1‑based per OEIS offset.  Compute the variation. */
    seq_t x  = (seq_t)n;   /* n in seq_t for arbitrary‑precision support */
    seq_t tot = 0;

    if (x == (seq_t)0) {   /* Should never happen for n>=1, but keep safe */
        return 0;
    }

    /* least significant hex digit */
    seq_t prev = x % (seq_t)16;
    x /= (seq_t)16;

    while (x > (seq_t)0) {
        seq_t curr = x % (seq_t)16;
        /* absolute difference of two non‑negative digits */
        seq_t diff = (prev > curr) ? (prev - curr) : (curr - prev);
        tot += diff;
        prev = curr;
        x /= (seq_t)16;
    }

    return tot;
}

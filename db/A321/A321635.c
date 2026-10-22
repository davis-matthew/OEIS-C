/*
 * A321635  Number of (5-n)-faces of the 1_22 polytope.
 * https://oeis.org/A321635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 54 + 648*C(n,1) + 810*C(n,2) - 2268*C(n,3) + 2286*C(n,4)
 *            - 72*C(n,5)
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(n,k) = binom(n,k).  This is the Newton forward-difference
   expansion of the 5‑th degree polynomial that satisfies the first six
   terms of the sequence. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* positive contributions */
    seq_t pos =
        (seq_t)54
        + (seq_t)648 * binom(n, 1)
        + (seq_t)810 * binom(n, 2)
        + (seq_t)2286 * binom(n, 4);

    /* negative contributions */
    seq_t neg =
        (seq_t)2268 * binom(n, 3)
        + (seq_t)72 * binom(n, 5);

    return pos - neg;
}
